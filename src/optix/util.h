#ifndef UTIL_H
#define UTIL_H
//includes
#include <stdint.h>
#include "gui_control.h"

void optix_CycleSelectedElement(struct optix_widget *stack[]);
void optix_RecursiveResizeAndAlign(struct optix_widget *widget, uint16_t width, uint8_t height);
void optix_ResizeTransformToTransform(struct optix_widget *widget, struct optix_widget *parent);
void optix_SetObjectTransform(struct optix_widget *widget, int x, int y, uint16_t width, uint8_t height);
void optix_SetObjectCallbacks(struct optix_widget *widget, void (*render)(struct optix_widget *), void (*update)(struct optix_widget *));
void optix_SetObjectChildren(struct optix_widget *widget, struct optix_widget **children);
void optix_SetPosition(struct optix_widget *widget, int x, int y);
void optix_AlignTransformToTransform(struct optix_widget *transform, struct optix_widget *reference);
bool optix_CheckTransformOverlap(struct optix_widget *test, struct optix_widget *reference);
void optix_RecursiveAlign(struct optix_widget *widget);
void optix_RecursiveSetNeedsRedraw(struct optix_widget *stack[]);
void optix_IntelligentRecursiveSetNeedsRedraw(struct optix_widget *stack[], struct optix_widget *reference);
uint16_t optix_GetNumElementsInStack(struct optix_widget *stack[]);
int optix_GetElementInStackByAddress(struct optix_widget *stack[], struct optix_widget *ptr);
void optix_RemoveElementInStack(struct optix_widget **stack, int index, int num_elements);

#endif