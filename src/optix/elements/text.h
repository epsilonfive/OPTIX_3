#ifndef TEXT_H
#define TEXT_H

#include "../globals.h"

struct optix_text {
    struct optix_widget widget;
    const char *text;
    //probably should have these in here too
    uint8_t x_centering;
    uint8_t y_centering;
};

//functions
void optix_RenderText_default(struct optix_widget *widget);
void optix_InitializeTextTransform(struct optix_text *text);

#endif