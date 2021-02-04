#ifndef LOOP_H
#define LOOP_H

#include "globals.h"
#include "elements/button.h"
#include "elements/text.h"

void optix_UpdateGUI(struct optix_gui_stack *stack, int numentries);
void optix_RenderGUI(struct optix_gui_stack *stack, int numentries);

#endif

