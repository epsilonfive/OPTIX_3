#ifndef WINDOW_H
#define WINDOW_H

#include <stdbool.h>
#include "../gui_control.h"

//defines
#define TITLE_BAR_HEIGHT   14
//the height of the window when you snap it left or right
#define WINDOW_SNAP_HEIGHT 240
#define WINDOW_SNAP_MIN_Y  0

#define WINDOW_RESIZE_NONE   0
#define WINDOW_RESIZE_LEFT   1
#define WINDOW_RESIZE_RIGHT  2
#define WINDOW_RESIZE_BOTTOM 3

#define WINDOW_BORDER_HITBOX 4

//few quick notes: setting needs_focus without selecting will simply move the window to the top without selecting it
//selecting it and setting needs_focus will do both
struct optix_window {
    struct optix_widget widget;
    //if this is true we'll update the children and use it as the stack in box-based navigation mode
    bool active;
    //this will be for if we're resizing the window
    uint8_t resize_border;
    uint16_t resize_width;
    uint8_t resize_height;
    bool resize_active;
};

//NOTE: this must have the window set as window, rather than being a child of the window, because it can control the position of the window, its open/close status, and so on
//the idea here is that you can set text and maybe some more buttons as children of the title bar, which could be used to minimize/close the window
//may be a better idea to have it be just the struct instead of a pointer to one?
//can't think of a scenario where this would be needed
struct optix_window_title_bar {
    struct optix_widget widget;
    struct optix_window *window;
    bool active_save;
    //for moving the windows later
    int drag_start_x;
    int drag_start_y;
    //this will be set if a drag is currently active (clicked the window title bar, held, and then moved the cursor somewhere else and released) 
    //the new input system should make this fairly easy
    bool drag_active;
};

//functions
void optix_HandleWindowResizing(struct optix_window *window);
void optix_RenderWindow_default(struct optix_widget *widget);
void optix_UpdateWindow_default(struct optix_widget *widget);
void optix_RenderWindowTitleBar_default(struct optix_widget *widget);
void optix_UpdateWindowTitleBar_default(struct optix_widget *widget);
void optix_RefreshWindowTitleBarTransform(struct optix_window_title_bar *window_title_bar);
void optix_MoveWidgetToTop(struct optix_widget *widget);

#endif