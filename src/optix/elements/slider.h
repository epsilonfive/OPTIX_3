#ifndef SLIDER_H
#define SLIDER_H

#include <stdint.h>
#include <stdbool.h>
#include <graphx.h>
#include "../gui_control.h"
#include "../cursor.h"
#include "text.h"

struct optix_slider {
    struct optix_widget widget;
    int min;
    int max;
    int *value;
    bool disp_current_value;
    //this will pretty much just be a box that displays the current value
    uint16_t current_value_box_size;
};

//functions
void optix_UpdateSlider_default(struct optix_widget *widget);
void optix_RenderSlider_default(struct optix_widget *widget);

#endif