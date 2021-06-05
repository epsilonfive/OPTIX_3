#ifndef GLOBALS_H
#define GLOBALS_H

//CE stuff
//most things are definitely going to need this
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
#include "gui_control.h"
#include "colors.h"
//#include "util.h"

//defines
#define BUTTON_COLOR          255
#define BUTTON_SELECTED_COLOR 224
#define OUTLINE_COLOR         255

//transform, will be used for a lot of things
struct optix_centering {
    uint8_t x_centering;
    uint8_t y_centering;
    int8_t x_offset;
    int8_t y_offset;
};

//I'll just keep this in the window and menu objects for now
struct optix_resize_info {
    bool resizable;
    //set to true to lock x or y to the original x/y
    bool x_lock;
    bool y_lock;
    //a reference transform, width will be subtracted to determine new scale (maybe if you wanted to have a fixed-size sidebar or something)
    struct optix_transform *x_reference_transform;
    struct optix_transform *y_reference_transform;
    uint16_t min_width;
    uint8_t min_height;
};

struct optix_transform {
    int x;
    int y;
    uint16_t width;
    uint8_t height;
    //centering information, doesn't necessary have to be used
};

struct optix_state {
    bool selected;
    bool visible;
    bool selectable;
    bool needs_redraw;
};

struct optix_widget {
    uint8_t type;
    struct optix_transform transform;
    struct optix_centering centering;
    struct optix_state state;
    void (*render)(struct optix_widget *);
    //the callback for updating
    void (*update)(struct optix_widget *);
    //array of child pointers (basically just substacks)
    //please make NULL if it isn't used
    struct optix_widget **child;
};

#endif