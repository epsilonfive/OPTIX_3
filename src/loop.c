#include "loop.h"

void optix_UpdateGUI(void) {
    int i;
    optix_cursor.update();
    for (i = 0; i < optix_gui_control.num_buttons; i++) optix_button[i].update(&optix_button[i]);
}

void optix_RenderGUI(void) {
    int i;
    for (i = 0; i < optix_gui_control.num_buttons; i++) optix_button[i].render(&optix_button[i]);
    optix_cursor.render();
}