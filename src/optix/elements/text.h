#ifndef TEXT_H
#define TEXT_H

#include "../globals.h"

struct optix_text {
    struct optix_widget widget;
    const char *text;
};

//functions
void optix_RenderText_default(struct optix_widget *widget);
void optix_InitializeTextTransform(struct optix_text *text);

#endif