#include "shortcuts.h"

void optix_HandleShortcuts(struct optix_widget *stack[]) {
    if (kb_Data[1] & kb_Yequ) {
        if (optix_gui_data.can_press) {
            if (kb_Data[5] & kb_Tan) {
                optix_AltTab(stack);
                optix_gui_data.can_press = false;
            }
        } else if (!(kb_Data[5] & kb_Tan)) optix_gui_data.can_press = true;
    }
}

void optix_AltTab(struct optix_widget *stack[]) {
    optix_CycleSelectedElement(stack);
}
