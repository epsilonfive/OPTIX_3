#include "sprite.h"

#include <debug.h>

#include "../loop.h"

void optix_RenderSprite_default(struct optix_widget *widget) {
    struct optix_sprite *sprite = (struct optix_sprite *) widget;
    if (widget->state.visible && widget->state.needs_redraw) {
        if (sprite->transparent) {
            gfx_SetTransparentColor(sprite->transparent_color);
            gfx_ScaledTransparentSprite_NoClip(sprite->spr, widget->transform.x, widget->transform.y, sprite->x_scale, sprite->y_scale);
        } else gfx_ScaledSprite_NoClip(sprite->spr, widget->transform.x, widget->transform.y, sprite->x_scale, sprite->y_scale);
        if (widget->child) optix_RenderStack(widget->child);
    }
}