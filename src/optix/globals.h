#ifndef GLOBALS_H
#define GLOBALS_H

//CE stuff
#include <debug.h>
#include <tice.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <graphx.h>
#include <fileioc.h>
#include <keypadc.h>
#include <intce.h>
#include <math.h>
//most things are definitely going to need this
#include "guicontrol.h"

//defines
#define BUTTON_COLOR          255
#define BUTTON_SELECTED_COLOR 8
#define OUTLINE_COLOR         74

//transform, will be used for a lot of things
struct optix_transform {
    int x;
    int y;
    uint16_t width;
    uint8_t height;
};

struct optix_state {
    bool selected;
    bool visible;
};

//what Jacobly suggested
struct optix_widget {
    uint8_t type;
    struct optix_transform transform;
    struct optix_state state;
    void (*render)(struct optix_widget *);
    //the callback for updating
    void (*update)(struct optix_widget *);
    //array of child pointers (basically just substacks)
    //please make NULL if it isn't used
    struct optix_widget **child;
};

#endif