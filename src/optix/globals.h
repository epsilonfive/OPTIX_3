#ifndef GLOBALS_H
#define GLOBALS_H

//CE stuff
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
#define BUTTON_COLOR  255
#define OUTLINE_COLOR  74

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


struct optix_gui_stack {
    //the stack will have some items on it, and when it needs to render, it will loop through and use each callback to render everything
    //probably just read until we hit a null
    void *ptr;
    int type;
};



#endif