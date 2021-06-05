#ifndef BUTTON_H
#define BUTTON_H

//includes
#include <stdbool.h>
#include <stdint.h>
#include <graphx.h>
#include "../gui_control.h"
#include "../shapes.h"
#include "../cursor.h"
#include "../util.h"

//typedefs and things
struct optix_button {
    struct optix_widget widget;
    //the idea with this part is that you could pass whatever kind of arguments you want into that
    //probably a struct
    void (*click_action)(void *);
    void *click_args;
    //whether or not it has been pressed (falling edge)
    bool pressed;
};

//functions
//default update/render functions for the buttons
void optix_UpdateButton_default(struct optix_widget *widget);
void optix_RenderButton_default(struct optix_widget *widget);

#endif