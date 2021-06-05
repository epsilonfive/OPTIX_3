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
    if (current_context->cursor->state == OPTIX_CURSOR_NORMAL && optix_CheckTransformOverlap(&current_context->cursor->widget, widget)) {
        if (!widget->state.selected) needs_redraw = true;
        widget->state.selected = true;
        current_context->cursor->state = OPTIX_CURSOR_OVER_ITEM;
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
                BUTTON_BG_COLOR_PRESSED_INDEX, WINDOW_BORDER_BEVEL_DARK_INDEX, WINDOW_BORDER_BEVEL_LIGHT_INDEX);                               //color
                optix_SetTextColor(BUTTON_TEXT_FG_COLOR_PRESSED_INDEX, BUTTON_TEXT_BG_COLOR_PRESSED_INDEX);
            } else {
                optix_OutlinedRectangle_WithBevel(widget->transform.x, widget->transform.y, widget->transform.width, widget->transform.height, //transform
                BUTTON_BG_COLOR_SELECTED_INDEX, WINDOW_BORDER_BEVEL_LIGHT_INDEX, WINDOW_BORDER_BEVEL_DARK_INDEX);                              //color
                optix_SetTextColor(BUTTON_TEXT_FG_COLOR_SELECTED_INDEX, BUTTON_TEXT_BG_COLOR_SELECTED_INDEX);
            }
        } else {
            //for partial redraw later on
            gfx_SetColor(BUTTON_BG_COLOR_UNSELECTED_INDEX);
            gfx_FillRectangle(widget->transform.x, widget->transform.y, widget->transform.width, widget->transform.height);
            optix_SetTextColor(BUTTON_TEXT_FG_COLOR_UNSELECTED_INDEX, BUTTON_TEXT_BG_COLOR_SELECTED_INDEX);
        }
        //cool, recursion
        if (widget->child) optix_RenderStack(widget->child);
        //set this back
        optix_SetTextColor(TEXT_FG_COLOR_INDEX, TEXT_BG_COLOR_INDEX);
    }
}