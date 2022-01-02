#include "loop.h"

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <keypadc.h>
#include <tice.h>
#include <string.h>
#include <debug.h>

#include "shortcuts.h"
#include "cursor.h"
#include "input.h"

#include "elements/window.h"

//takes an array of optix_widgets as an argument
//please have a NULL as the last entry in this array, so we'll know when to stop
//this should be a pointer to an array of pointers
void optix_UpdateGUI(void) {
    //we're assuming this is called in a loop
    //so this is the time since it was last called
    current_context->data->ticks = (long) timer_2_Counter;
    timer_2_Counter = 0;
    //GUI is not frozen at this point
    current_context->cursor->gui_frozen = false;
    //input things
    optix_UpdateInput();
    //we need this here unfortunately
    //start with this I suppose
    current_context->cursor->widget.update((struct optix_widget *) current_context->cursor);
    if (!current_context->cursor->gui_frozen) {
        optix_HandleShortcuts(current_context->stack);
        if (current_context->settings->cursor_active) {
            optix_SetCurrentSelection(NULL);
            optix_SetCurrentSelection(optix_GetCurrentSelection(current_context->stack));
        }
        optix_UpdateStack(current_context->stack);
        optix_HandleNearestElement();
        //we do need to do this as well (set the whole GUI to need redraw)
        if (current_context->data->gui_needs_full_redraw) optix_RecursiveSetNeedsRedraw(current_context->stack);
    }
}

void optix_UpdateStack(struct optix_widget *stack[]) {
    int i = 0;
    while (stack && stack[i]) {    
        if (stack[i]->state.visible && stack[i]->update) stack[i]->update(stack[i]);
        //account for the edge case that an element may have destroyed itself while updating
        if (stack[i]) {
            //dbg_sprintf(dbgout, "Updating type %d\n", stack[i]->type);
            //handle this case, just in case-window title bars/windows should only be active if they're the last one
            if (stack[i + 1]) {
                if (stack[i]->type == OPTIX_WINDOW_TYPE || stack[i]->type == OPTIX_WINDOW_TITLE_BAR_TYPE) {
                    struct optix_window *window = stack[i]->type == OPTIX_WINDOW_TYPE ? (struct optix_window *) stack[i] : ((struct optix_window_title_bar *) stack[i])->window;
                    window->active = false;
                }
            }
            if (stack[i]->state.needs_redraw && stack[i]->child) {
                optix_RecursiveSetNeedsRedraw(stack[i]->child);
                if (stack[i]->type == OPTIX_WINDOW_TITLE_BAR_TYPE) optix_RecursiveSetNeedsRedraw(((struct optix_window_title_bar *) stack[i])->window->widget.child);
            }
        }
        i++;
    }
}

//takes an array of optix_widgets
//please have a NULL as the last entry in this array, so we'll know when to stop
void optix_RenderGUI(void) {
    //do this first
    optix_RenderCursorBackground((struct optix_widget *) current_context->cursor);
    if (!current_context->cursor->gui_frozen) {
        optix_RenderStack(current_context->stack);
        //blit if necessary
        if (current_context->data->needs_blit) gfx_BlitBuffer();
        current_context->data->gui_needs_full_redraw = false;
        current_context->data->needs_blit = false;
    }
    //the cursor should be on top of everything else
    current_context->cursor->widget.render((struct optix_widget *) current_context->cursor);

}

void optix_RenderStack(struct optix_widget *stack[]) {
    int i = 0;
    while (stack && stack[i]) {
        if (stack[i]->state.visible && stack[i]->render) {
            //dbg_sprintf(dbgout, "Rendering type %d...\n", stack[i]->type);
            //make sure we blit when needed
            if (stack[i]->state.needs_redraw) current_context->data->needs_blit = true;
            stack[i]->render(stack[i]);
            stack[i]->state.needs_redraw = false;
        }
        i++;
    }
}