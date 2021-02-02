#include "button.h"

optix_transform_t optix_transform_temp;
optix_state_t optix_state_temp;
optix_button_t optix_button[MAX_NUM_BUTTONS];

//functions
//add button
optix_button_t *optix_AddButton(optix_transform_t transform, optix_state_t state, void (*update)(optix_button_t *), void (*render)(optix_button_t *), void (*click_action)(void *), void *click_args) {
    optix_button_t *temp = &optix_button[optix_gui_control.num_buttons++];
    temp->transform = transform;
    //temp->state = state;
    temp->update = update;
    temp->render = render;
    temp->click_action = click_action;
    temp->click_args = click_args;
    //initialize the other things
    temp->pressed = false;
    //initialize the state
    //if (temp->state == NULL) {
        temp->state.color = 255;
        temp->state.selected = false;
        temp->state.visible = true;
    //}
    return temp;
}

//update button
//just change the color to red if pressed and run the click action for now
void optix_UpdateButton_default(optix_button_t *button) {
    //kb_Scan will be called elsewhere
    if (kb_Data[6] & kb_Enter || kb_Data[1] & kb_2nd) {
        if (!button->pressed) {
            button->click_action(button->click_args);
            button->state.color = 224;
            button->pressed = true;
        }
    } else {
        button->pressed = false;
        //the color should change back
        button->state.color = 255;
    }
}

//render button
void optix_RenderButton_default(optix_button_t *button) {
    if (button->state.visible) {
        if (button->state.selected) optix_OutlinedRectangle(button->transform.x, button->transform.y, button->transform.width, button->transform.height, BUTTON_COLOR, OUTLINE_COLOR);
        else {
            gfx_SetColor(OUTLINE_COLOR);
            gfx_Rectangle(button->transform.x, button->transform.y, button->transform.width, button->transform.height);
        }
    }
}