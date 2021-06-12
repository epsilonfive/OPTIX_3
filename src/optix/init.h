#ifndef INIT_H
#define INIT_H

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "gui_control.h"
#include "elements/text.h"
#include "elements/button.h"
#include "elements/menu.h"
#include "elements/window.h"
#include "elements/sprite.h"
#include "elements/divider.h"
#include "elements/rectangle.h"
#include "elements/input_box.h"
#include "elements/scroll_bar.h"
#include "elements/check_box.h"
#include "elements/slider.h"

//functions
void optix_InitializeWidget(struct optix_widget *widget, uint8_t type);
uint16_t optix_GetSize(struct optix_widget *widget);
void optix_CopyElementHandleSpecialCase(struct optix_widget **widget, struct optix_widget *reference);
void optix_CopyElement(struct optix_widget **widget, struct optix_widget *reference);

#endif