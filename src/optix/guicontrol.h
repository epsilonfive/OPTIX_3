#ifndef GUI_CONTROL_H
#define GUI_CONTROL_H

//includes
//#include "globals.h"

//definitions
#define OPTIX_NUM_TYPES             6
#define OPTIX_TEXT_TYPE             0
#define OPTIX_SPRITE_TYPE           1
#define OPTIX_BUTTON_TYPE           2
#define OPTIX_MENU_TYPE             3
#define OPTIX_WINDOW_TYPE           4
#define OPTIX_WINDOW_TITLE_BAR_TYPE 5
//centering types
//always the same
#define OPTIX_CENTERING_CENTERED 1
#define OPTIX_CENTERING_LEFT     0
#define OPTIX_CENTERING_RIGHT    2
#define OPTIX_CENTERING_TOP      0
#define OPTIX_CENTERING_BOTTOM   2

//functions
void optix_InitializeGUIState(void);

#endif