#ifndef UTIL_H
#define UTIL_H

#include "globals.h"
#include "elements/text.h"
#include "elements/button.h"
#include "elements/menu.h"
#include "elements/window.h"
#include "elements/sprite.h"


void optix_InitializeWidget(struct optix_widget *widget, uint8_t type);
void optix_SetObjectTransform(struct optix_widget *widget, int x, int y, uint16_t width, uint8_t height);
void optix_SetObjectCallbacks(struct optix_widget *widget, void (*render)(struct optix_widget *), void (*update)(struct optix_widget *));
void optix_SetObjectChildren(struct optix_widget *widget, struct optix_widget **children);
void optix_SetPosition(struct optix_widget *widget, int x, int y);
void optix_AlignTransformToTransform(struct optix_widget *transform, struct optix_widget *reference, uint8_t x_centering, uint8_t y_centering);
bool optix_CheckTransformOverlap(struct optix_widget *test, struct optix_widget *reference);

#endif