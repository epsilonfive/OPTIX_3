#ifndef TEXT_H
#define TEXT_H

#include "../globals.h"

struct optix_text {
    struct optix_transform transform;
    struct optix_state state;
    const char *text;
    void (*render)(struct optix_text *);
};

//functions
void optix_RenderText_default(struct optix_text *text);

#endif