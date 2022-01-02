#ifndef GUI_CONTROL_H
#define GUI_CONTROL_H

//includes
#include <stdint.h>
#include <stdbool.h>
#include "input.h"

//definitions
//this is needed
#define TIMER_FREQUENCY             32768
//everything else
#define OPTIX_NUM_TYPES             12
#define OPTIX_TEXT_TYPE             0
#define OPTIX_SPRITE_TYPE           1
#define OPTIX_BUTTON_TYPE           2
#define OPTIX_MENU_TYPE             3
#define OPTIX_WINDOW_TYPE           4
#define OPTIX_WINDOW_TITLE_BAR_TYPE 5
#define OPTIX_DIVIDER_TYPE          6
#define OPTIX_RECTANGLE_TYPE        7
#define OPTIX_INPUT_BOX_TYPE        8
#define OPTIX_SCROLL_BAR_TYPE       9
#define OPTIX_CHECK_BOX_TYPE        10
#define OPTIX_SLIDER_TYPE           11
//centering types
//always the same
#define OPTIX_CENTERING_CENTERED    1
#define OPTIX_CENTERING_LEFT        0
#define OPTIX_CENTERING_RIGHT       2
#define OPTIX_CENTERING_TOP         0
#define OPTIX_CENTERING_BOTTOM      2

//structs
//core structs
//transform, will be used for a lot of things
struct optix_centering {
    uint8_t x_centering;
    uint8_t y_centering;
    int8_t x_offset;
    int8_t y_offset;
};

//eventual addition: resizing "rules"-elements can hide themselves/scale differently when different scales
struct optix_resize_info {
    bool resizable;
    //set to true to lock x or y to the original x/y
    //if this is set, size will not be changed
    bool x_lock;
    bool y_lock;
    //both are floats from 0-1, and are relative to the size of the parent
    float relative_width;
    float relative_height;
    //floats from 0-1, relative position within the parent object itself
    //so with right centering and a relative size of 0.5, you'd want this to be -0.5
    //with left centering and a relative size of 0.5, if you wanted it at the left edge it would be 0
    //this is basically intelligent offset setting, as it'll shift things in a direction relative to the width/height of the parent
    //you can use it with the x and y offsets in optix_centering as well, if you want it to be constant
    //again, this is relative to the centering dictated by optix_centering
    float relative_x_shift;
    float relative_y_shift;
    //simply the minimum width and height an object can have, and it will not go lower than
    //probably only useful for windows, as this would break positioning otherwise
    uint16_t min_width;
    uint8_t min_height;
    //and a constant number of pixels the resulting width and height should be changed by
    //possibly useful if there's a constant size sidebar or something similar
    int16_t x_size_change;
    int8_t y_size_change;
};

struct optix_transform {
    int x;
    int y;
    uint16_t width;
    uint8_t height;
    //centering information, doesn't necessary have to be used
};

struct optix_state {
    bool selected;
    bool visible;
    bool selectable;
    bool needs_redraw;
    //sizing info (override size in bytes, if so desired)
    //mostly intended for the CopyStack function, once that's made
    bool override_size;
    uint16_t size;
};

struct optix_widget {
    uint8_t type;
    struct optix_transform transform;
    struct optix_centering centering;
    struct optix_state state;
    struct optix_resize_info resize_info;
    void (*render)(struct optix_widget *);
    //the callback for updating
    void (*update)(struct optix_widget *);
    //array of child pointers (basically just substacks)
    //please make NULL if it isn't used
    struct optix_widget **child;
};

//for buttons and menus, and maybe others later as well
struct optix_click_action {
    void (*click_action)(void *);
    void *click_args;
};

//so this will probably be in an array within the button/menu, and every loop, if it is selected,
//the button/menu will check to see if any of these were pressed and if so run the action
//should be fairly fast, I hope
struct optix_alternate_key_click_action {
    struct optix_click_action click_action;
    uint8_t alternate_key;
};

//other structs
struct optix_settings {
    //whether or not we should use the Windows-style cursor instead of the box-based navigation mode
    bool cursor_active;
    //whether or not the whole GUI should be redrawn every frame
    bool constant_refresh;
};

struct optix_gui_data {
    bool gui_needs_full_redraw;
    bool needs_blit;
    //if the font was loaded correctly, if not then the library should just use graphx for the text rendering
    bool font_valid;
    //skKey
    uint8_t key;
    //number of ticks
    long ticks;
};

//do this later
struct optix_context {
    struct optix_settings *settings;
    struct optix_gui_data *data;
    struct optix_colors *colors;
    struct optix_cursor *cursor;
    struct optix_input *input;
    struct optix_widget **stack;
};

//functions
//functions
void optix_InitializeGUIState(void);
void optix_SetContext(struct optix_context *context);
void optix_InitializeSettings(struct optix_settings *settings);
void optix_InitializeData(struct optix_gui_data *data);


//globals (sorry Mateo)
extern struct optix_context *current_context;

#endif