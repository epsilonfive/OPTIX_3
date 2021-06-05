#ifndef UTIL_H
#define UTIL_H

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "gui_control.h"
#include "elements/window.h"
#include "elements/input_box.h"

void optix_CycleSelectedElement(struct optix_widget *stack[]);
void optix_SetObjectTransform(struct optix_widget *widget, int x, int y, uint16_t width, uint8_t height);
void optix_SetObjectCallbacks(struct optix_widget *widget, void (*render)(struct optix_widget *), void (*update)(struct optix_widget *));
void optix_SetObjectChildren(struct optix_widget *widget, struct optix_widget **children);
void optix_SetPosition(struct optix_widget *widget, int x, int y);
void optix_AlignTransformToTransform(struct optix_widget *transform, struct optix_widget *reference, uint8_t x_centering, uint8_t y_centering);
bool optix_CheckTransformOverlap(struct optix_widget *test, struct optix_widget *reference);
void optix_RecursiveAlign(struct optix_widget *widget);
void optix_RecursiveSetNeedsRedraw(struct optix_widget *stack[]);


#endif