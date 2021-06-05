#ifndef SHORTCUTS_H
#define SHORTCUTS_H

#include <stdbool.h>
#include <keypadc.h>
#include "gui_control.h"
#include "util.h"

void optix_HandleShortcuts(struct optix_widget *stack[]);
void optix_AltTab(struct optix_widget *stack[]);

#endif