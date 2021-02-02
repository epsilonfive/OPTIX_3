#include "guicontrol.h"

optix_gui_control_t optix_gui_control;

//functions
void optix_InitializeGUIState(void) {
    optix_gui_control.num_buttons = 0;
    optix_gui_control.num_windows = 0;
    optix_gui_control.num_scrollbars = 0;
}

void optix_InitializeCursor(void) {
    optix_cursor.x = LCD_WIDTH / 2;
    optix_cursor.y = LCD_HEIGHT / 2;
    optix_cursor.update = optix_UpdateCursor_default;
    optix_cursor.render = optix_RenderCursor_default;
    optix_cursor.active = true;
}
