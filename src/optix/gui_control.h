#ifndef GUI_CONTROL_H
#define GUI_CONTROL_H

//includes
#include "globals.h"

//definitions
#define OPTIX_NUM_TYPES             9
#define OPTIX_TEXT_TYPE             0
#define OPTIX_SPRITE_TYPE           1
#define OPTIX_BUTTON_TYPE           2
#define OPTIX_MENU_TYPE             3
#define OPTIX_WINDOW_TYPE           4
#define OPTIX_WINDOW_TITLE_BAR_TYPE 5
#define OPTIX_DIVIDER_TYPE          6
#define OPTIX_RECTANGLE_TYPE        7
#define OPTIX_INPUT_BOX_TYPE        8
//centering types
//always the same
#define OPTIX_CENTERING_CENTERED    1
#define OPTIX_CENTERING_LEFT        0
#define OPTIX_CENTERING_RIGHT       2
#define OPTIX_CENTERING_TOP         0
#define OPTIX_CENTERING_BOTTOM      2

//functions
void optix_InitializeGUIState(void);

//structs
//and yes, these do really need to be global Mateo
typedef struct {
    //whether or not we should use the Windows-style cursor instead of the box-based navigation mode
    bool cursor_active;
    //whether or not the whole GUI should be redrawn every frame
    bool constant_refresh;
} optix_settings_t;
extern optix_settings_t optix_settings;

typedef struct {
    bool gui_needs_full_redraw;
    //if the font was loaded correctly, if not then the library should just use graphx for the text rendering
    bool font_valid;
    bool can_press;
    //skKey
    uint8_t key;
} optix_gui_data_t;
extern optix_gui_data_t optix_gui_data;

//do this later
/*struct optix_context_t {
    struct optix_settings_t settings;
    struct optix_gui_data_t data;
    struct optix_widget **stack;
};*/


#endif