#include "gui_control.h"

//globals
//only one so it's okay Mateo
struct optix_context *current_context;

//functions
void optix_InitializeGUIState(void) {

}

void optix_InitializeSettings(struct optix_settings *settings) {
    settings->cursor_active = false;
    settings->constant_refresh = false;
}

void optix_InitializeData(struct optix_gui_data *data) {
    data->can_press = true;
    data->font_valid = false;
    data->gui_needs_full_redraw = true;
    data->key = 0;
}

void optix_SetContext(struct optix_context *context) {
    current_context = context;
}