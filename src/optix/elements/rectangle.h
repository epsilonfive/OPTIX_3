#ifndef RECTANGLE_H
#define RECTANGLE_H

#include <stdint.h>
#include <stdbool.h>
#include <graphx.h>
#include "../gui_control.h"

//structs
struct optix_rectangle {
    struct optix_widget widget;
    bool filled;
    uint8_t border_color;
    uint8_t fill_color;
};


//functions
void optix_RenderRectangle_default(struct optix_widget *widget);

#endif