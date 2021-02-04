#ifndef BUTTON_H
#define BUTTON_H

//includes
#include "../globals.h"
#include "../shapes.h"
#include "../cursor.h"

//typedefs and things
struct optix_button {
    struct optix_transform transform;
    struct optix_state state;
    //let's have the update function pass a pointer to an optix_button in as an argument
    void (*update)(void *);
    //the other callback, for rendering
    void (*render)(void *);
    //the idea with this part is that you could pass whatever kind of arguments you want into that
    //probably a struct
    void (*click_action)(void *);
    void *click_args;
    //whether or not it has been pressed (falling edge)
    bool pressed;
};

//functions
void optix_SetButtonState(struct optix_button *button, struct optix_state state);
void optix_SetButtonTransform(struct optix_button *button, struct optix_transform transform);
void optix_SetButtonCallbacks(struct optix_button *button, void (*update)(struct optix_button *), void (*render)(struct optix_button *), void (*click_action)(void *), void *click_args);
//default update/render functions for the buttons
void optix_UpdateButton_default(struct optix_button *button);
void optix_RenderButton_default(struct optix_button *button);

#endif