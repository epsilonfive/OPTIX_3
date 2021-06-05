#include "shortcuts.h"

void optix_HandleShortcuts(struct optix_widget *stack[]) {
    if (kb_Data[1] & kb_Yequ) {
        if (current_context->data->can_press) {
            if (kb_Data[5] & kb_Tan) {
                optix_AltTab(stack);
                current_context->data->can_press = false;
            }
        } //else current_context->data->can_press = !kb_AnyKey();
    }
}

void optix_AltTab(struct optix_widget *stack[]) {
    optix_CycleSelectedElement(stack);
}
