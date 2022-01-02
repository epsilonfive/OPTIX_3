#ifndef BUTTON_H
#define BUTTON_H

//includes
#include <stdbool.h>
#include "../gui_control.h"

//typedefs and things
struct optix_button {
    struct optix_widget widget;
    //the idea with this part is that you could pass whatever kind of arguments you want into that
    //probably a struct
    struct optix_click_action click_action;
    //whether or not it has been pressed (falling edge)
    bool pressed;
    //will be activated if this key is pressed too
    uint8_t alternate_key;
    //if this option is enabled, there will not be a rectangle drawn in the unselected BG color when unselected
    //so the background will show behind it
    bool transparent_background;
    bool hide_selection_box;
    //saves the last selection status
    bool selected_save;
};

//functions
//default update/render functions for the buttons
void optix_UpdateButton_default(struct optix_widget *widget);
void optix_RenderButton_default(struct optix_widget *widget);

#endif