#ifndef GUI_CONTROL_H
#define GUI_CONTROL_H

//includes
//#include "globals.h"
#include "cursor.h"

//definitions
#define OPTIX_TEXT_TYPE   0
#define OPTIX_SPRITE_TYPE 1
#define OPTIX_BUTTON_TYPE 2
#define OPTIX_MENU_TYPE   3
#define OPTIX_WINDOW_TYPE 4
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