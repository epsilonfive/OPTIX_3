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
//centering types
//always the same
#define OPTIX_CENTERING_CENTERED 1
#define OPTIX_CENTERING_LEFT     0
#define OPTIX_CENTERING_RIGHT    2
#define OPTIX_CENTERING_TOP      0
#define OPTIX_CENTERING_BOTTOM   2

//functions
void optix_InitializeGUIState(void);
void optix_InitializeWidget(struct optix_widget *widget, uint8_t type);
void optix_SetObjectTransform(struct optix_widget *widget, int x, int y, uint16_t width, uint8_t height);
void optix_SetObjectCallbacks(struct optix_widget *widget, void (*render)(struct optix_widget *), void (*update)(struct optix_widget *));
void optix_AlignTransformToTransform(struct optix_transform *transform, struct optix_transform *reference, uint8_t x_centering, uint8_t y_centering);
void optix_SetObjectChildren(struct optix_widget *widget, struct optix_widget **children);

#endif