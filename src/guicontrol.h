#ifndef GUI_CONTROL_H
#define GUI_CONTROL_H

//includes
#include "globals.h"
#include "button.h"
#include "cursor.h"

typedef struct {
    int num_buttons;
    int num_scrollbars;
    int num_windows;
} optix_gui_control_t;
extern optix_gui_control_t optix_gui_control;

//functions
void optix_InitializeGUIState(void);
void optix_InitializeCursor(void);

#endif