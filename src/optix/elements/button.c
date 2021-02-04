#include "button.h"

//functions
//add button
void optix_SetButtonState(struct optix_button *button, struct optix_state state) {
    button->state = state;
}

void optix_SetButtonTransform(struct optix_button *button, struct optix_transform transform) {
    button->transform = transform;
}

void optix_SetButtonCallbacks(struct optix_button *button, void (*update)(struct optix_button *), void (*render)(struct optix_button *), void (*click_action)(void *), void *click_args) {
    button->update = update;
    button->render = render;
    button->click_action = click_action;
    button->click_args = click_args;
}

//update button
//just change the color to red if pressed and run the click action for now
void optix_UpdateButton_default(struct optix_button *button) {
    //check if the cursor overlaps with it
    if (!button->state.visible) return;
    if (gfx_CheckRectangleHotspot(button->transform.x, button->transform.y, button->transform.width, button->transform.height,
        optix_cursor.x, optix_cursor.y, OPTIX_CURSOR_WIDTH, OPTIX_CURSOR_HEIGHT)) 
        button->state.selected = true;
    else button->state.selected = false;
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
void optix_RenderButton_default(struct optix_button *button) {
    if (button->state.visible) {
        if (button->state.selected) optix_OutlinedRectangle(button->transform.x, button->transform.y, button->transform.width, button->transform.height, BUTTON_COLOR, OUTLINE_COLOR);
        else {
            gfx_SetColor(OUTLINE_COLOR);
            gfx_Rectangle(button->transform.x, button->transform.y, button->transform.width, button->transform.height);
        }
    }
}