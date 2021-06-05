#ifndef SCROLL_BAR_H
#define SCROLL_BAR_H

//includes
#include "../globals.h"
#include "../shapes.h"
#include "menu.h"
#include "text.h"

//defines
#define SCROLL_BAR_HORIZ (bool) 0
#define SCROLL_BAR_VERT  (bool) 1

//structs
struct optix_scroll_bar {
    struct optix_widget widget;
    //the number of rows, the logic is that you can have this point to the rows field of a menu or something
    //will intelligently mirror
    struct optix_widget *mirror;
    //whether this should be a horizontal or vertical scroll bar
    bool direction;
};

void optix_UpdateScrollBar_default(struct optix_widget *widget);
void optix_RenderScrollBar_default(struct optix_widget *widget);

#endif