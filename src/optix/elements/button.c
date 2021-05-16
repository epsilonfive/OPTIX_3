#include "button.h"

//functions
//update button
//just change the color to red if pressed and run the click action for now
void optix_UpdateButton_default(struct optix_widget *widget) {
    struct optix_button *button = (struct optix_button *) widget;
    bool needs_redraw = false;
    //check if the cursor overlaps with it
    if (!widget->state.visible) return;
    //kb_Scan will be called elsewhere
    if (kb_Data[6] & kb_Enter || kb_Data[1] & kb_2nd) {
        if (!button->pressed && widget->state.selected) {
            dbg_sprintf(dbgout, "Button pressed.\n");
            if (button->click_action) button->click_action(button->click_args);
            //button->state.color = 224;
            button->pressed = true;
        }
    } else {
        if (button->pressed) needs_redraw = true;
        button->pressed = false;
        //the color should change back
        //button->state.color = 255;
    }
    if (optix_cursor.state == OPTIX_CURSOR_NORMAL && optix_CheckTransformOverlap(&optix_cursor.widget, widget)) {
        if (!widget->state.selected) needs_redraw = true;
        widget->state.selected = true;
        optix_cursor.state = OPTIX_CURSOR_OVER_ITEM;
    } else {
        if (widget->state.selected) needs_redraw = true;
        widget->state.selected = button->pressed = false;
    }
    if (needs_redraw) {
        widget->state.needs_redraw = true;
        optix_RecursiveSetNeedsRedraw(widget->child);
        //this could cause some issues apparently
        //optix_RefreshCursorBackground();
    }
}

//render button
void optix_RenderButton_default(struct optix_widget *widget) {
    struct optix_button *button = (struct optix_button *) widget;
    if (widget->state.visible && widget->state.needs_redraw) {
        if (widget->state.selected) {
            if (button->pressed) {
                optix_OutlinedRectangle_WithBevel(widget->transform.x, widget->transform.y, widget->transform.width, widget->transform.height, //transform
                optix_colors.button_bg_pressed, optix_colors.border_bevel_dark, optix_colors.border_bevel_light);                              //color
                optix_SetTextColor(optix_colors.button_text_fg_pressed, optix_colors.button_text_bg_pressed);
            } else {
                optix_OutlinedRectangle_WithBevel(widget->transform.x, widget->transform.y, widget->transform.width, widget->transform.height, //transform
                optix_colors.button_bg_selected,  optix_colors.border_bevel_light, optix_colors.border_bevel_dark);                  //color
                optix_SetTextColor(optix_colors.button_text_fg_selected, optix_colors.button_text_bg_selected);
            }
        } else {
            //for partial redraw later on
            gfx_SetColor(optix_colors.button_bg_unselected);
            gfx_FillRectangle(widget->transform.x, widget->transform.y, widget->transform.width, widget->transform.height);
            optix_SetTextColor(optix_colors.button_text_fg_unselected, optix_colors.button_text_bg_unselected);
        }
        //cool, recursion
        if (widget->child) optix_RenderStack(widget->child);
        //set this back
        optix_SetTextColor(optix_colors.text_fg, optix_colors.text_bg);
    } //else dbg_sprintf(dbgout, "Button was not visible\n");
}