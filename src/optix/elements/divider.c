#include "divider.h"

//functions
//no update here, we don't need one

void optix_RenderDivider_default(struct optix_widget *widget) {
    struct optix_divider *divider = (struct optix_divider *) widget;
    struct optix_transform *reference = (struct optix_transform *) &divider->reference->transform;
    //gfx_SetColor(optix_colors.divider);
    gfx_SetColor(255);
    dbg_sprintf(dbgout, "Rendering...%d %d %d %d\n", reference->x, reference->y, reference->width, reference->height);
    switch (divider->alignment) {
        case DIVIDER_ALIGNMENT_BOTTOM:
        case DIVIDER_ALIGNMENT_TOP:
            gfx_HorizLine(reference->x, reference->y - 1 + (divider->alignment == DIVIDER_ALIGNMENT_BOTTOM) * (reference->height + 1), reference->width + divider->width_increase);
            break;
        case DIVIDER_ALIGNMENT_LEFT:
        case DIVIDER_ALIGNMENT_RIGHT:
            gfx_VertLine(reference->x - 1 + (divider->alignment == DIVIDER_ALIGNMENT_RIGHT) * (reference->width + 1), reference->y, reference->height + divider->height_increase);
            break;
    }
}


