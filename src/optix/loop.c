#include "loop.h"

//takes an array of optix_widgets as an argument
//please have a NULL as the last entry in this array, so we'll know when to stop
//this should be a pointer to an array of pointers
void optix_UpdateGUI(struct optix_widget *(*stack)[]) {
    kb_Scan();
    //start with this I suppose
    optix_HandleShortcuts(*stack);
    optix_UpdateStack_TopLevel(stack);
    optix_cursor.widget.update(NULL);
}

void optix_UpdateStack(struct optix_widget *stack[]) {
    int i = 0;
    while (stack[i]) {
        if (stack[i]->update) stack[i]->update(stack[i]);
        if (stack[i]->state.needs_redraw && stack[i]->child) {
            optix_RecursiveSetNeedsRedraw(stack[i]->child);
            if (stack[i]->type == OPTIX_WINDOW_TITLE_BAR_TYPE) optix_RecursiveSetNeedsRedraw(((struct optix_window_title_bar *) stack[i])->window->widget.child);
        }
        //if this has been selected, we want to loop through and make sure nothing else is selected
        //this is so that what's on top will be selected, or what is rendered last
        if (stack[i]->state.selected) {
            for (int j = 0; j < i; j++) stack[j]->state.selected = false;
            break;
        }
        i++;
    }
}

//use only for the top level stack, it will reorganize the windows in the render queue and things
void optix_UpdateStack_TopLevel(struct optix_widget *(*stack)[]) {
    int i = 0;
    struct optix_widget *curr_window = NULL;
    int curr_window_index = 0;
    bool found_window = false;
    bool window_needs_focus = false;
    bool found_window_with_focus = false;
    //start things out by doing the thing
    //the value of i at the end will also be the number of elements in the stack
    while ((*stack)[i]) {
        //if this has been selected, we want to loop through and make sure nothing else is selected
        //this is so that what's on top will be selected, or what is rendered last
        //only one window can be selected at a time
        if ((*stack)[i]->update) (*stack)[i]->update((*stack)[i]);
        //if it needs to be redrawn handle that
        if ((*stack)[i]->state.needs_redraw && (*stack)[i]->child) {
            optix_RecursiveSetNeedsRedraw((*stack)[i]->child);
            if ((*stack)[i]->type == OPTIX_WINDOW_TITLE_BAR_TYPE) optix_RecursiveSetNeedsRedraw(((struct optix_window_title_bar *) (*stack)[i])->window->widget.child);
        }
        //if we've found a focused window, just continue
        if (found_window_with_focus) {
            i++;
            continue;
        }
        //if it's a window, and reports it needs to be moved to the top, stop everything and do that
        if ((*stack)[i]->type == OPTIX_WINDOW_TITLE_BAR_TYPE) {
            struct optix_window_title_bar *window_title_bar = (struct optix_window_title_bar *) (*stack)[i];
            window_needs_focus = window_title_bar->window->needs_focus;
            window_title_bar->window->needs_focus = false;
        } else if ((*stack)[i]->type == OPTIX_WINDOW_TYPE) {
            struct optix_window *window = (struct optix_window *) (*stack)[i];
            window_needs_focus = window->needs_focus;
            window->needs_focus = false;
        }
        if (window_needs_focus || ((*stack)[i]->state.selected && ((*stack)[i]->type == OPTIX_WINDOW_TITLE_BAR_TYPE || (*stack)[i]->type == OPTIX_WINDOW_TYPE))) {
            if (window_needs_focus) found_window_with_focus = true;
            found_window = true;
            curr_window_index = i;
            curr_window = (*stack)[i];
        }
        i++;
    }
    //handle this as well
    //start with this, because why not
    if (optix_gui_data.gui_needs_full_redraw) optix_RecursiveSetNeedsRedraw(*stack);
    //cursor stuff
    if (!optix_settings.cursor_active && optix_cursor.direction != OPTIX_CURSOR_NO_DIR) {
        struct optix_widget *possible_selection = NULL;
        struct optix_widget **temp = NULL;
        if (!optix_cursor.current_selection) optix_cursor.current_selection = (*stack)[0];
        possible_selection = optix_FindNearestElement(optix_cursor.direction, optix_cursor.current_selection,
        //LINE 69 HAHAHAHAHAAHAHAHAHAHA
        (temp = (curr_window->type == OPTIX_WINDOW_TYPE ? curr_window->child : ((struct optix_window_title_bar *) curr_window)->window->widget.child)) ? temp : *stack);
        //move the cursor to that position
        if (possible_selection) {
            optix_cursor.current_selection = possible_selection;
            optix_cursor.widget.transform.x = optix_cursor.current_selection->transform.x;
            optix_cursor.widget.transform.y = optix_cursor.current_selection->transform.y;
        }
    }
    //if it's already the last entry don't bother
    if (!found_window) return;
    //set everything in the array to unselected, except for the current window
    for (int j = 0; j < i; j++) {
        struct optix_window_title_bar *window_title_bar = (struct optix_window_title_bar *) (*stack)[j];
        if ((*stack)[j]->type == OPTIX_WINDOW_TITLE_BAR_TYPE) (*stack)[j]->state.selected = window_title_bar->window->widget.state.selected = (j == curr_window_index);
        else if ((*stack)[j]->type == OPTIX_WINDOW_TYPE) (*stack)[j]->state.selected = (j == curr_window_index);
    }
    if (i == curr_window_index + 1) return;
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
    optix_gui_data.gui_needs_full_redraw = false;
}

void optix_RenderStack(struct optix_widget *stack[]) {
    int i = 0;
    while (stack[i]) {
        if (stack[i]->render) {
            stack[i]->render(stack[i]);
            stack[i]->state.needs_redraw = false;
        }
        i++;
    }
}