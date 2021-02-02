#ifndef BUTTON_H
#define BUTTON_H

//includes
#include "globals.h"
#include "shapes.h"

//defines
#define MAX_NUM_BUTTONS 50

//typedefs and things
//transform, will be used for a lot of things
typedef struct {
    int x;
    int y;
    uint16_t width;
    uint8_t height;
} optix_transform_t;
extern optix_transform_t optix_transform_temp;

typedef struct {
    bool selected;
    bool visible;
    uint8_t color;
} optix_state_t;
extern optix_state_t optix_state_temp;

typedef struct {
    optix_transform_t transform;
    optix_state_t state;
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
} optix_button_t;
extern optix_button_t optix_button[MAX_NUM_BUTTONS];

//functions
optix_button_t *optix_AddButton(optix_transform_t transform, optix_state_t state, void (*update)(optix_button_t *), void (*render)(optix_button_t *), void (*click_action)(void *), void *click_args);
void optix_UpdateButton_default(optix_button_t *button);
void optix_RenderButton_default(optix_button_t *button);


#endif