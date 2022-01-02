#include "shortcuts.h"

void optix_HandleShortcuts(struct optix_widget *stack[]) {
    if (kb_Data[1] & kb_Yequ) {
        if (true) {
            if (kb_Data[5] & kb_Tan) {
                optix_AltTab(stack);
            }
        } //else current_context->data->can_press = !kb_AnyKey();
    }
}

void optix_AltTab(struct optix_widget *stack[]) {
    optix_CycleSelectedElement(stack);
}
