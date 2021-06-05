#include "shortcuts.h"

void optix_HandleShortcuts(struct optix_widget *stack[]) {
    if (kb_Data[1] & kb_Yequ) {
        dbg_sprintf(dbgout, "Checking shortcut...%d %d\n", current_context->data->can_press, (kb_Data[5] & kb_Tan));
        if (current_context->data->can_press) {
            if (kb_Data[5] & kb_Tan) {
                dbg_sprintf(dbgout, "Alt-tabbing...\n");
                optix_AltTab(stack);
                current_context->data->can_press = false;
            }
        } //else current_context->data->can_press = !kb_AnyKey();
    }
}

void optix_AltTab(struct optix_widget *stack[]) {
    optix_CycleSelectedElement(stack);
}
