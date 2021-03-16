#ifndef TEXT_H
#define TEXT_H

#include "../globals.h"
#include <fontlibc.h>

#define DEFAULT_FONT_PACK_NAME "DRSANS"

struct optix_text {
    struct optix_widget widget;
    const char *text;
};

//functions
void optix_RenderText_default(struct optix_widget *widget);
void optix_InitializeTextTransform(struct optix_text *text);
bool optix_InitializeFont(void);

#endif