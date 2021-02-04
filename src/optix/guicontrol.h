#ifndef GUI_CONTROL_H
#define GUI_CONTROL_H

//includes
#include "globals.h"
#include "cursor.h"

//definitions
#define OPTIX_TEXT_TYPE   0
#define OPTIX_SPRITE_TYPE 1
#define OPTIX_BUTTON_TYPE 2
#define OPTIX_MENU_TYPE   3

//functions
void optix_InitializeGUIState(void);
void optix_InitializeCursor(void);

#endif