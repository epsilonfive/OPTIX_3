#ifndef SPRITE_H
#define SPRITE_H

#include <stdint.h>
#include <stdbool.h>
#include <graphx.h>
#include "../gui_control.h"

//structs
struct optix_sprite {
    struct optix_widget widget;
    gfx_sprite_t *spr;
    //scaling
    uint8_t x_scale;
    uint8_t y_scale;
    //transparency settings
    bool transparent;
    uint8_t transparent_color;
};

//functions
void optix_RenderSprite_default(struct optix_widget *widget);

#endif