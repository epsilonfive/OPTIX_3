#ifndef GUI_CONTROL_H
#define GUI_CONTROL_H

//includes
#include <stdint.h>
#include <stdbool.h>

//definitions
#define OPTIX_NUM_TYPES             11
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

//I'll just keep this in the window and menu objects for now
struct optix_resize_info {
    bool resizable;
    //set to true to lock x or y to the original x/y
    bool x_lock;
    bool y_lock;
    //a reference transform, width will be subtracted to determine new scale (maybe if you wanted to have a fixed-size sidebar or something)
    struct optix_transform *x_reference_transform;
    struct optix_transform *y_reference_transform;
    uint16_t min_width;
    uint8_t min_height;
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
};

struct optix_widget {
    uint8_t type;
    struct optix_transform transform;
    struct optix_centering centering;
    struct optix_state state;
    void (*render)(struct optix_widget *);
    //the callback for updating
    void (*update)(struct optix_widget *);
    //array of child pointers (basically just substacks)
    //please make NULL if it isn't used
    struct optix_widget **child;
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
    bool can_press;
    //skKey
    uint8_t key;
};

//do this later
struct optix_context {
    struct optix_settings *settings;
    struct optix_gui_data *data;
    struct optix_colors *colors;
    struct optix_cursor *cursor;
    struct optix_widget ***stack;
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