#include "rectangle.h"

//functions
void optix_RenderRectangle_default(struct optix_widget *widget) {
    struct optix_rectangle *rectangle = (struct optix_rectangle *) widget;
    if (widget->state.visible) {
        if (rectangle->filled) {
            gfx_SetColor(rectangle->fill_color);
            gfx_FillRectangle(widget->transform.x, widget->transform.y, widget->transform.width, widget->transform.height);
        }
        gfx_SetColor(rectangle->border_color);
        gfx_Rectangle(widget->transform.x, widget->transform.y, widget->transform.width, widget->transform.height);
    }
}