#include "button.h"

//functions
//update button
//just change the color to red if pressed and run the click action for now
void optix_UpdateButton_default(struct optix_widget *widget) {
    struct optix_button *button = (struct optix_button *) widget;
    //check if the cursor overlaps with it
    if (!widget->state.visible) return;
    if (optix_cursor.state == OPTIX_CURSOR_NORMAL && optix_CheckTransformOverlap(&optix_cursor.widget, widget)) {
        widget->state.selected = true;
        optix_cursor.state = OPTIX_CURSOR_OVER_ITEM;
    } else widget->state.selected = false;
    //kb_Scan will be called elsewhere
    if (kb_Data[6] & kb_Enter || kb_Data[1] & kb_2nd) {
        if (!button->pressed && widget->state.selected) {
            if (button->click_action != NULL) button->click_action(button->click_args);
            //button->state.color = 224;
            button->pressed = true;
        }
    } else {
        button->pressed = false;
        //the color should change back
        //button->state.color = 255;
    }
}

//render button
void optix_RenderButton_default(struct optix_widget *widget) {
    struct optix_button *button = (struct optix_button *) widget;
    if (widget->state.visible) {
        if (widget->state.selected) {
            if (button->pressed) { 
                optix_OutlinedRectangle(widget->transform.x, widget->transform.y, widget->transform.width, widget->transform.height, //transform
                optix_colors.button_bg_pressed, optix_colors.button_border);                                                         //color
                optix_SetTextColor(optix_colors.button_text_fg_pressed, optix_colors.button_text_bg_pressed);
            } else {
                optix_OutlinedRectangle(widget->transform.x, widget->transform.y, widget->transform.width, widget->transform.height, //transform
                optix_colors.button_bg_selected, optix_colors.button_border);                                                        //color
                optix_SetTextColor(optix_colors.button_text_fg_selected, optix_colors.button_text_bg_selected);
            }
        } else {
            optix_SetTextColor(optix_colors.button_text_fg_unselected, optix_colors.button_text_bg_unselected);
            //for partial redraw later on
            //gfx_SetColor(BUTTON_COLOR);
            //gfx_FillRectangle(widget->transform.x, widget->transform.y, widget->transform.width, widget->transform.height);
        }
        //cool, recursion
        if (widget->child != NULL) optix_RenderStack(widget->child);
        //set this back
        optix_SetTextColor(optix_colors.text_fg, optix_colors.text_bg);
    }
}