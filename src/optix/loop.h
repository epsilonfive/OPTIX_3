#ifndef LOOP_H
#define LOOP_H

#include "gui_control.h"

void optix_UpdateGUI(void);
void optix_RenderGUI(void);
void optix_UpdateStack(struct optix_widget *stack[]);
void optix_UpdateStack_TopLevel(struct optix_widget *(*stack)[]);
void optix_RenderStack(struct optix_widget *stack[]);

#endif

