#ifndef WINDOW_H
#define WINDOW_H

#include "../globals.h"
#include "../util.h"
#include "../loop.h"
#include "text.h"
#include "button.h"

struct optix_window {
    struct optix_widget widget;
    const char *title;
};

//functions
void optix_RenderWindow_default(struct optix_widget *widget);
void optix_UpdateWindow_default(struct optix_widget *widget);

#endif