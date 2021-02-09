#include "button.h"

//functions
//update button
//just change the color to red if pressed and run the click action for now
void optix_UpdateButton_default(struct optix_widget *widget) {
    struct optix_button *button = (struct optix_button *) widget;
    //check if the cursor overlaps with it
    if (!widget->state.visible) return;
    if (gfx_CheckRectangleHotspot(widget->transform.x, widget->transform.y, widget->transform.width, widget->transform.height,
        optix_cursor.x, optix_cursor.y, OPTIX_CURSOR_WIDTH, OPTIX_CURSOR_HEIGHT)) 
        widget->state.selected = true;
    else widget->state.selected = false;
    //kb_Scan will be called elsewhere
    if (kb_Data[6] & kb_Enter || kb_Data[1] & kb_2nd) {
        if (!button->pressed) {
            button->click_action(button->click_args);
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
    dbg_sprintf(dbgout, "Visible: %d, Selected %d\n", widget->state.visible, widget->state.selected);
    if (widget->state.visible) {
        if (widget->state.selected) {
            if (button->pressed) optix_OutlinedRectangle(widget->transform.x, widget->transform.y, widget->transform.width, widget->transform.height, BUTTON_SELECTED_COLOR, OUTLINE_COLOR);
            else optix_OutlinedRectangle(widget->transform.x, widget->transform.y, widget->transform.width, widget->transform.height, BUTTON_COLOR, OUTLINE_COLOR);
        } else {
            gfx_SetColor(OUTLINE_COLOR);
            gfx_Rectangle(widget->transform.x, widget->transform.y, widget->transform.width, widget->transform.height);
        }
    }
}