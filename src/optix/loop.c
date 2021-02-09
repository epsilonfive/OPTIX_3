#include "loop.h"

//takes an array of optix_widgets as an argument
//please have a NULL as the last entry in this array, so we'll know when to stop
void optix_UpdateGUI(struct optix_widget *stack[]) {
    int i = 0;
    dbg_sprintf(dbgout, "Updating GUI...\n");
    optix_cursor.update();
    while (stack[i] != NULL) {
        dbg_sprintf(dbgout, "Calling...\n");
        if (stack[i]->update != NULL) stack[i]->update(stack[i]);
        i++;
    }
    dbg_sprintf(dbgout, "Updating finished after %d iterations.\n", i);
}

//takes an array of optix_widgets
//please have a NULL as the last entry in this array, so we'll know when to stop
void optix_RenderGUI(struct optix_widget *stack[]) {
    int i = 0;
    dbg_sprintf(dbgout, "Rendering GUI...\n");
    while (stack[i] != NULL) {
        if (stack[i]->render != NULL) stack[i]->render(stack[i]);
        //cool recursion
        if (stack[i]->child != NULL) optix_RenderGUI(stack[i]->child);
        i++;
    }
    //the cursor should be on top of everything else
    optix_cursor.render();
    dbg_sprintf(dbgout, "Rendering finished after %d iterations.\n", i);
}