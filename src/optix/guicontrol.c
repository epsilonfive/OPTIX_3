#include "guicontrol.h"

//MAKE SURE THAT TRANSFORM ALWAYS COMES FIRST, FOLLOWED BY STATE

//functions
void optix_InitializeGUIState(void) {
    //optix_gui_control.num_buttons = 0;
    //optix_gui_control.num_windows = 0;
    //optix_gui_control.num_scrollbars = 0;
}

//helpful GUI functions

//initializes a widget, and also sets its state to non-selected and visible (which will be updated elsewhere in the GUI)
void optix_InitializeWidget(struct optix_widget *widget, uint8_t type) {
    widget->type = type;
    widget->child = NULL;
    widget->state.selected = false;
    widget->state.visible = true;
}


void optix_SetObjectTransform(struct optix_widget *widget, int x, int y, uint16_t width, uint8_t height) {
    widget->transform.x = x;
    widget->transform.y = y;
    widget->transform.width = width;
    widget->transform.height = height;
}

void optix_SetObjectCallbacks(struct optix_widget *widget, void (*render)(struct optix_widget *), void (*update)(struct optix_widget *)) {
    widget->render = render;
    widget->update = update;
}



//Aligns a transform to another transform. Use OPTIX_CENTERING_LEFT, RIGHT, etc.
//transform is aligned to reference based on its width and height
void optix_AlignTransformToTransform(struct optix_transform *transform, struct optix_transform *reference, uint8_t x_centering, uint8_t y_centering) {
    transform->x = (reference->x + reference->width / 2) - (transform->width / 2 * x_centering);
    transform->y = (reference->y + reference->height / 2) - (transform->height / 2 * y_centering);
}

//sets the children of an object
void optix_SetObjectChildren(struct optix_widget *widget, struct optix_widget **children) {
    widget->child = children;
}
