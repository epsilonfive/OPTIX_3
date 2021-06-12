#ifndef WINDOW_H
#define WINDOW_H

#include <stdint.h>
#include <stdbool.h>
#include <graphx.h>
#include "../gui_control.h"
#include "../loop.h"
#include "../util.h"
#include "menu.h"
#include "text.h"
#include "button.h"

//defines
#define TITLE_BAR_HEIGHT   14
//the height of the window when you snap it left or right
#define WINDOW_SNAP_HEIGHT 240
#define WINDOW_SNAP_MIN_Y  0


struct optix_window {
    struct optix_widget widget;
    struct optix_resize_info resize_info;
    //set to true to move the window to the top on the next update loop
    bool needs_focus;
};

//NOTE: this must have the window set as window, rather than being a child of the window, because it can control the position of the window, its open/close status, and so on
//the idea here is that you can set text and maybe some more buttons as children of the title bar, which could be used to minimize/close the window
struct optix_window_title_bar {
    struct optix_widget widget;
    struct optix_window *window;
};

//functions
void optix_RenderWindow_default(struct optix_widget *widget);
void optix_UpdateWindow_default(struct optix_widget *widget);
void optix_RenderWindowTitleBar_default(struct optix_widget *widget);
void optix_UpdateWindowTitleBar_default(struct optix_widget *widget);
void optix_ResizeWindow(struct optix_widget *widget, uint16_t width, uint8_t height);
void optix_RefreshWindowTitleBarTransform(struct optix_window_title_bar *window_title_bar);

#endif