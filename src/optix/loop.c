#include "loop.h"

//takes an array of optix_widgets as an argument
//please have a NULL as the last entry in this array, so we'll know when to stop
void optix_UpdateGUI(struct optix_widget *stack[]) {
    int i = 0;
    kb_Scan();
    while (stack[i] != NULL) {
        if (stack[i]->update != NULL) stack[i]->update(stack[i]);
        //if this has been selected, we want to loop through and make sure nothing else is selected
        //this is so that what's on top will be selected, or what is rendered last
        if (stack[i]->state.selected) for (int j = 0; j < i; j++) stack[j]->state.selected = false;
        i++;
    }
    optix_cursor.update();
}

//takes an array of optix_widgets
//please have a NULL as the last entry in this array, so we'll know when to stop
void optix_RenderGUI(struct optix_widget *stack[]) {
    int i = 0;
    while (stack[i] != NULL) {
        if (stack[i]->render != NULL) stack[i]->render(stack[i]);
        i++;
    }
    //the cursor should be on top of everything else
    optix_cursor.render();
}