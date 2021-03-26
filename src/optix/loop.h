#ifndef LOOP_H
#define LOOP_H

#include "globals.h"
#include "shortcuts.h"
#include "elements/button.h"
#include "elements/text.h"

void optix_UpdateGUI(struct optix_widget *(*stack)[]);
void optix_RenderGUI(struct optix_widget *stack[]);
void optix_UpdateStack(struct optix_widget *stack[]);
void optix_UpdateStack_TopLevel(struct optix_widget *(*stack)[]);
void optix_RenderStack(struct optix_widget *stack[]);

#endif

