#include "text.h"

void optix_RenderText_default(struct optix_text *text) {
    if (text->state.visible)
        gfx_PrintStringXY(text->text, text->transform.x, text->transform.y);
}




