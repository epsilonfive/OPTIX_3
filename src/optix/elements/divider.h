#ifndef DIVIDER_H
#define DIVIDER_H

#include <stdint.h>
#include <stdbool.h>
#include <graphx.h>
#include "../gui_control.h"
#include "../colors.h"

//defines
#define DIVIDER_ALIGNMENT_TOP    0
#define DIVIDER_ALIGNMENT_BOTTOM 1
#define DIVIDER_ALIGNMENT_LEFT   2
#define DIVIDER_ALIGNMENT_RIGHT  3

//structs
struct optix_divider {
    struct optix_widget widget;
    struct optix_widget *reference;
    uint8_t alignment;
    //if you want to make it slightly longer or wider
    int8_t width_increase;
    int8_t height_increase;
};

//functions
void optix_RenderDivider_default(struct optix_widget *widget);

#endif