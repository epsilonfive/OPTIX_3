#include "loop.h"

void optix_UpdateGUI(struct optix_gui_stack *stack, int numentries) {
    int i;
    optix_cursor.update();
    for (i = 0; i < numentries; i++) {
        switch (stack[i].type) {
            case OPTIX_BUTTON_TYPE:
                ((struct optix_button *)stack[i].ptr)->update(stack[i].ptr);
                break;
            default:
                break;
        }
    }
}

void optix_RenderGUI(struct optix_gui_stack *stack, int numentries) {
    for (int i = 0; i < numentries; i++) {
        switch (stack[i].type) {
            case OPTIX_TEXT_TYPE:
                ((struct optix_text *)stack[i].ptr)->render(stack[i].ptr);
            case OPTIX_BUTTON_TYPE:
                ((struct optix_button *)stack[i].ptr)->render(stack[i].ptr);
                break;
            default:
                break;
        }
    }
    //the cursor should be on top of everything else
    optix_cursor.render();
}