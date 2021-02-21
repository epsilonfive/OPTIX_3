#include "loop.h"

//takes an array of optix_widgets as an argument
//please have a NULL as the last entry in this array, so we'll know when to stop
//this should be a pointer to an array of pointers
void optix_UpdateGUI(struct optix_widget *(*stack)[]) {
    kb_Scan();
    optix_cursor.widget.update(NULL);
    optix_UpdateStack_TopLevel(stack);
}

void optix_UpdateStack(struct optix_widget *stack[]) {
    int i = 0;
    while (stack[i]) {
        if (stack[i]->update) stack[i]->update(stack[i]);
        //if this has been selected, we want to loop through and make sure nothing else is selected
        //this is so that what's on top will be selected, or what is rendered last
        if (stack[i]->state.selected) for (int j = 0; j < i; j++) stack[j]->state.selected = false;
        i++;
    }
}

//use only for the top level stack, it will reorganize the windows in the render queue and things
void optix_UpdateStack_TopLevel(struct optix_widget *(*stack)[]) {
    int i = 0;
    struct optix_widget *curr_window = NULL;
    int curr_window_index = 0;
    bool found_window = false;
    while ((*stack)[i]) {
        //if this has been selected, we want to loop through and make sure nothing else is selected
        //this is so that what's on top will be selected, or what is rendered last
        //only one window can be selected at a time
        if ((*stack)[i]->update != NULL) (*stack)[i]->update((*stack)[i]);
        if ((*stack)[i]->state.selected && ((*stack)[i]->type == OPTIX_WINDOW_TITLE_BAR_TYPE || (*stack)[i]->type == OPTIX_WINDOW_TYPE)) {
            found_window = true;
            curr_window_index = i;
            curr_window = (*stack)[i];
        }
        i++;
    }
    //if it's already the last entry don't bother
    if (!found_window) return;
    for (int j = 0; j < i; j++) 
        if ((*stack)[j]->type == OPTIX_WINDOW_TITLE_BAR_TYPE || (*stack)[j]->type == OPTIX_WINDOW_TYPE) (*stack)[j]->state.selected = (j == curr_window_index);
    if (i == curr_window_index + 1) return;
    dbg_sprintf(dbgout, "Moving (%d - %d) stack elements...\n", i, curr_window_index);
    memmove((void *) stack + (curr_window_index * sizeof(struct optix_widget ***)), (void *) stack + ((curr_window_index + 1) * sizeof(struct optix_widget ***)),
    (i - curr_window_index) * sizeof(struct optix_widget ***));
    (*stack)[i - 1] = curr_window;
}


//takes an array of optix_widgets
//please have a NULL as the last entry in this array, so we'll know when to stop
void optix_RenderGUI(struct optix_widget *stack[]) {
    optix_RenderStack(stack);
    //the cursor should be on top of everything else
    optix_cursor.widget.render(NULL);
}

void optix_RenderStack(struct optix_widget *stack[]) {
    int i = 0;
    while (stack[i] != NULL) {
        if (stack[i]->render != NULL) stack[i]->render(stack[i]);
        i++;
    }
}

