#include "input_box.h"

void optix_UpdateInputBox_default(struct optix_widget *widget) {
    struct optix_input_box *input_box = (struct optix_input_box *) widget;
    const char *keys[3] = {"\0\0\0\0\0\0\0\0\0\0\0WRMH\0\0?\0VQLG\0\0.ZUPKFC\0 YTOJEB\0\0XSNIDA\0\0\0\0\0\0\0\0\0",
                           "\0\0\0\0\0\0\0\0\0\0\0wrmh\0\0?\0vqlg\0\0.zupkfc\0 ytojeb\0\0xsnida\0\0\0\0\0\0\0\0\0",
                           "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0-\x33\x36\x39)\0\0\0.\x32\x35\x38(\0\0\0\x30\x31\x34\x37,\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
                          };
    char *text = input_box->text->text;
    size_t length = strlen(text) + 1;
    //conditions for return
    if (optix_CheckTransformOverlap(&optix_cursor.widget, widget)) {
        optix_cursor.state = OPTIX_CURSOR_TEXT;
        if (kb_Data[6] & kb_Enter || kb_Data[1] & kb_2nd) widget->state.selected = true;
    } else if (kb_Data[6] & kb_Enter || kb_Data[1] & kb_2nd) widget->state.selected = false;
    if (widget->state.selected) //set the state of the cursor to indicate we're inputting text
        optix_cursor.state = input_box->mode + OPTIX_CURSOR_TEXT_UPPER;
    if (!widget->state.selected || !optix_gui_data.key || !text) return;
    switch (optix_gui_data.key) {
        case sk_Add:
            if (input_box->cursor_offset < length - 1) input_box->cursor_offset++;
            break;
        case sk_Sub:
            if (input_box->cursor_offset > 0) input_box->cursor_offset--;
            break;
        case sk_Clear:
            //clear the whole string
            input_box->cursor_offset = 0;
            text[0] = '\0';
            break;
        case sk_Alpha:
            input_box->mode = input_box->mode < INPUT_BOX_MODE_MATH ? input_box->mode + 1 : 0;
            break;
        case sk_Del:
            if (input_box->cursor_offset > 0) {
                //shift everything back one, starting at the cursor offset
                memmove(text + input_box->cursor_offset - 1, text + input_box->cursor_offset, length - input_box->cursor_offset);
                input_box->cursor_offset--;
            }
            break;
        default:
            if (keys[input_box->mode][optix_gui_data.key] && (!input_box->has_max_length || length < input_box->max_length)) {
                //maybe this will work (shift everything over one)
                memmove(text + input_box->cursor_offset + 1, text + input_box->cursor_offset, length - input_box->cursor_offset);
                text[input_box->cursor_offset] = keys[input_box->mode][optix_gui_data.key];
                input_box->cursor_offset++;
            }
            break;
    }
    //we need to update some offsets now, of course
    input_box->text->needs_offset_update = true;
    //REMOVE THIS LATER
    optix_InitializeTextTransform(input_box->text);
    optix_RecursiveAlign(widget);
    input_box->text->widget.state.needs_redraw = widget->state.needs_redraw = true;
    dbg_sprintf(dbgout, "New is %s\n", text);
}

void optix_RenderInputBox_default(struct optix_widget *widget) {
    struct optix_input_box *input_box = (struct optix_input_box *) widget;    
    //we'll use the button colors for this I guess
    if (widget->state.visible && widget->state.needs_redraw) {
        //make the outside
        optix_OutlinedRectangle_WithBevel(widget->transform.x, widget->transform.y, widget->transform.width, widget->transform.height, //x, y, width, height
        optix_colors.button_bg_unselected, optix_colors.border_bevel_dark, optix_colors.border_bevel_light);                           //bevel (make it look depressed)
        //set the color
        optix_SetTextColor(optix_colors.button_text_fg_unselected, optix_colors.button_text_bg_unselected);
        //binary search maybe
        if (input_box->text && input_box->text->widget.render) {
            struct optix_text *text = (struct optix_text *) input_box->text;
            text->widget.render((struct optix_widget *) text);
            //do the blinking line thing I guess
            if (widget->state.selected) {
                gfx_SetColor(optix_colors.button_text_fg_unselected);
                gfx_VertLine(text->widget.transform.x + optix_GetStringWidthL(text->text, input_box->cursor_offset) - 1, text->widget.transform.y, 10);
            }
        }
        if (widget->child) optix_RenderStack(widget->child);
    }
}

//other methods
//gets the index of a given string, given its offset in pixels
//this is mostly useful for finding where the text cursor should be for the input box routines
//also recursive so that's cool
size_t optix_GetStringIndexByOffset(char *str, int offset) {
    int start_index = 0, end_index = strlen(str);
    while (start_index <= end_index) {
        int middle = start_index + (end_index - start_index ) / 2;
        if (abs(optix_GetStringWidthL(str, middle) - offset) <= INPUT_BOX_CURSOR_TOLERANCE) return middle;
        if (optix_GetStringWidthL(str, middle) < offset) start_index = middle + 1;
        else end_index = middle - 1;
    }
    return -1;
}