#ifndef LOOP_H
#define LOOP_H

#include "globals.h"
#include "elements/button.h"
#include "elements/text.h"

void optix_UpdateGUI(struct optix_widget *stack[]);
void optix_RenderGUI(struct optix_widget *stack[]);

#endif

