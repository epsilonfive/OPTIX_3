#ifndef WINDOW_H
#define WINDOW_H

#include "../globals.h"
#include "../util.h"
#include "../loop.h"
#include "text.h"
#include "button.h"

//defines
#define TITLE_BAR_HEIGHT 14

struct optix_window {
    struct optix_widget widget;
};

//NOTE: this must have the window set as window, rather than being a child of the window, because it can control the position of the window, its open/close status, and so on
//the idea here is that you can set text and maybe some more buttons as children of the title bar, which could be used to minimize/close the window
struct optix_window_title_bar {
    struct optix_widget widget;
    struct optix_window *window;
    const char *title;
};

//functions
void optix_RenderWindow_default(struct optix_widget *widget);
void optix_UpdateWindow_default(struct optix_widget *widget);
void optix_RenderWindowTitleBar_default(struct optix_widget *widget);
void optix_UpdateWindowTitleBar_default(struct optix_widget *widget);


#endif