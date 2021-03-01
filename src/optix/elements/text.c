#include "text.h"

//sets a text object's width and height according to the text's width and height
void optix_InitializeTextTransform(struct optix_text *text) {
    text->widget.transform.x = 0;
    text->widget.transform.y = 0;
    text->widget.transform.width = gfx_GetStringWidth(text->text);
    text->widget.transform.height = 8;
    text->widget.centering.x_centering = OPTIX_CENTERING_CENTERED;
    text->widget.centering.y_centering = OPTIX_CENTERING_CENTERED;
}


void optix_RenderText_default(struct optix_widget *widget) {
    struct optix_text *text = (struct optix_text *) widget;
    if (widget->state.visible)
        gfx_PrintStringXY(text->text, widget->transform.x, widget->transform.y);
}




