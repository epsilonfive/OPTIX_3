#include "util.h"
//includes
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <tice.h>
#include <graphx.h>
#include <debug.h>

#include "gui_control.h"
#include "cursor.h"

#include "elements/window.h"
#include "elements/input_box.h"
#include "elements/menu.h"

//helpful GUI functions
void optix_SetObjectTransform(struct optix_widget *widget, int x, int y, uint16_t width, uint8_t height) {
    widget->transform.x = x;
    widget->transform.y = y;
    widget->transform.width = width;
    widget->transform.height = height;
}

void optix_SetObjectCallbacks(struct optix_widget *widget, void (*render)(struct optix_widget *), void (*update)(struct optix_widget *)) {
    widget->render = render;
    widget->update = update;
}

//sets the children of an object
void optix_SetObjectChildren(struct optix_widget *widget, struct optix_widget **children) {
    widget->child = children;
}

//sets the position of an object, and also moves its children accordingly
void optix_SetPosition(struct optix_widget *widget, int x, int y) {
    if (!widget) return;
    //start by figuring out what the difference is between the new and old positions
    int x_shift = x - widget->transform.x;
    int y_shift = y - widget->transform.y;
    //shift the widget itself
    widget->transform.x = x;
    widget->transform.y = y;
    //move its children as well
    if (widget->child) {
        int i = 0;
        while (widget->child[i]) {
            struct optix_widget *child = widget->child[i];
            optix_SetPosition(child, child->transform.x + x_shift, child->transform.y + y_shift);
            i++;
        }
    }
}

//resize an element, according to the size of the parent
void optix_ResizeTransformToTransform(struct optix_widget *widget, struct optix_widget *parent) {
    if (!widget->resize_info.x_lock) widget->transform.width = (widget->resize_info.relative_width * parent->transform.width) + widget->resize_info.x_size_change;
    if (!widget->resize_info.y_lock) widget->transform.height = (widget->resize_info.relative_height * parent->transform.height) + widget->resize_info.y_size_change;
    if (widget->type == OPTIX_MENU_TYPE) {
        struct optix_menu *menu = (struct optix_menu *) widget;
        if (!menu->widget.resize_info.x_lock)
            menu->columns = widget->transform.width / menu->min_option_width;
        if (!menu->widget.resize_info.y_lock)
            menu->rows = widget->transform.height / menu->min_option_height;
        dbg_sprintf(dbgout, "Width %d Height %d Rows %d Columns %d", widget->transform.width, widget->transform.height, menu->rows, menu->columns);
    }
}

void optix_RecursiveResizeAndAlign(struct optix_widget *widget, uint16_t width, uint8_t height) {
    //set the width and height, taking into account the minimum width and height
    if (widget->resize_info.resizable) {
        uint16_t target_width = width + widget->resize_info.x_size_change;
        uint8_t target_height = height + widget->resize_info.y_size_change;
        dbg_sprintf(dbgout, "Resizable %d\n", widget->type);
        widget->transform.width = target_width >= widget->resize_info.min_width && !widget->resize_info.x_lock ? target_width : widget->resize_info.min_width;
        widget->transform.height = target_height >= widget->resize_info.min_height && !widget->resize_info.y_lock ? target_height : widget->resize_info.min_height;
        //loop through the widget's children, resizing each of them as well
        if (widget->child) {
            int i = 0;
            while (widget->child[i]) {
                struct optix_widget *child = widget->child[i];
                optix_ResizeTransformToTransform(child, widget);
                optix_AlignTransformToTransform(child, widget);
                //now finally, shift it over according to the relative x and y shift things
                child->transform.x += child->resize_info.relative_x_shift * widget->transform.width;
                child->transform.y += child->resize_info.relative_y_shift * widget->transform.height;
                //and then do the resizing of the child's children
                if (child->child && child->type != OPTIX_MENU_TYPE) optix_RecursiveResizeAndAlign(child, child->transform.width, child->transform.height);
                i++;
            }
        }
    } else dbg_sprintf(dbgout, "Window was not resizable\n");
}



//Aligns a transform to another transform. Use OPTIX_CENTERING_LEFT, RIGHT, etc.
//transform is aligned to reference based on its width and height
void optix_AlignTransformToTransform(struct optix_widget *transform, struct optix_widget *reference) {
    //if reference is NULL, use the screen
    struct optix_widget temp = {.transform = {.x = 0, .y = 0, .width = LCD_WIDTH, .height = LCD_HEIGHT}};
    if (!reference) reference = &temp;
    uint8_t x_centering = transform->centering.x_centering, y_centering = transform->centering.y_centering;
    transform->transform.x = reference->transform.x + ((reference->transform.width - transform->transform.width) / 2) * x_centering + transform->centering.x_offset;
    if (x_centering == OPTIX_CENTERING_RIGHT) transform->transform.x += ((reference->transform.width - transform->transform.width) % 2);
    transform->transform.y = reference->transform.y + ((reference->transform.height - transform->transform.height) / 2) * y_centering + transform->centering.y_offset;
    if (y_centering == OPTIX_CENTERING_BOTTOM) transform->transform.y += ((reference->transform.height - transform->transform.height) % 2);
}

bool optix_CheckTransformOverlap(struct optix_widget *test, struct optix_widget *reference) {
    return gfx_CheckRectangleHotspot(test->transform.x, test->transform.y, test->transform.width, test->transform.height,
        reference->transform.x, reference->transform.y, reference->transform.width, reference->transform.height);
}

//pass it in a widget, and it will recursively align all of its children
void optix_RecursiveAlign(struct optix_widget *widget) {
    //we need to align this as well
    //because that's not how this works
    if (widget->type == OPTIX_MENU_TYPE) return;
    if (widget->type == OPTIX_WINDOW_TITLE_BAR_TYPE) 
        optix_RecursiveAlign((struct optix_widget *) ((struct optix_window_title_bar *) widget)->window);
    if (widget->child) {
        int i = 0;
        while (widget->child[i]) {
            struct optix_widget *child = widget->child[i];
            optix_AlignTransformToTransform(child, widget);
            if (child->child && child->type != OPTIX_MENU_TYPE) optix_RecursiveAlign(child);
            i++;
        }
    }
}

//mark the entire GUI as needing a redraw
void optix_RecursiveSetNeedsRedraw(struct optix_widget *stack[]) {
    int i = 0;
    if (!stack) return;
    while (stack[i]) {
        stack[i]->state.needs_redraw = true;
        if (stack[i]->child) optix_RecursiveSetNeedsRedraw(stack[i]->child);
        if (stack[i]->type == OPTIX_WINDOW_TITLE_BAR_TYPE) {
            ((struct optix_window_title_bar *) stack[i])->window->widget.state.needs_redraw = true;
            optix_RecursiveSetNeedsRedraw(((struct optix_window_title_bar *) stack[i])->window->widget.child);
        }
        i++;
    }
}

//same thing but tests if it overlaps first
void optix_IntelligentRecursiveSetNeedsRedraw(struct optix_widget *stack[], struct optix_widget *reference) {
    //let's just unintelligently set redraw now
    current_context->data->gui_needs_full_redraw = true;
    /*int i = 0;
    if (!stack) return;
    while (stack[i]) {
        if (!stack[i]->state.needs_redraw && optix_CheckTransformOverlap(stack[i], reference)) {
            stack[i]->state.needs_redraw = true;
            optix_IntelligentRecursiveSetNeedsRedraw(stack, stack[i]);
        }
        if (stack[i]->child) optix_IntelligentRecursiveSetNeedsRedraw(stack[i]->child, reference);
        if (stack[i]->type == OPTIX_WINDOW_TITLE_BAR_TYPE) {
            ((struct optix_window_title_bar *) stack[i])->window->widget.state.needs_redraw = true;
            optix_IntelligentRecursiveSetNeedsRedraw(((struct optix_window_title_bar *) stack[i])->window->widget.child, reference);
        }
        i++;
    }*/
}


//handles the [alt][tab] kind of thing where holding down [y=] in box-based mode lets you cycle between things on the top level
void optix_CycleSelectedElement(struct optix_widget *stack[]) {
    //start by finding the currently selected element
    //just return if something went badly
    if (stack && stack[0]) {
        struct optix_widget *new_selection = NULL;
        int i = 0;
        int selected_index = 0;
        int new_index = 0;
        while (stack[i]) {
            if (stack[i]->state.selected) {
                selected_index = i;
                break;
            }
            i++;
        }
        //cycle through and find the new one
        i = stack[selected_index + 1] ? selected_index + 1 : 0;
        while (true) {
            if (stack[i]->state.selectable) {
                new_index = i;
                break;
            }
            i++;
            if (!stack[i]) i = 0;
        }
        //deselect the currently selected one
        stack[selected_index]->state.selected = false;
        if (stack[selected_index]->type == OPTIX_WINDOW_TITLE_BAR_TYPE)
            ((struct optix_window_title_bar *) stack[selected_index])->window->widget.state.selected = false;
        //check if the next one is eligible
        new_selection = stack[new_index];
        new_selection->state.selected = new_selection->state.needs_redraw = true;
        //apparently this could cause issues?
        if (new_selection->type == OPTIX_WINDOW_TITLE_BAR_TYPE) {
            struct optix_window *window = ((struct optix_window_title_bar *) new_selection)->window;
            window->widget.state.needs_redraw = window->widget.state.selected = true;
        }
        if (!current_context->settings->cursor_active) {
            //just have it be the first child I guess
            if (new_selection->type != OPTIX_WINDOW_TITLE_BAR_TYPE && new_selection->child && new_selection->child[0]) optix_SetCurrentSelection(new_selection->child[0]);
            else if (new_selection->type == OPTIX_WINDOW_TITLE_BAR_TYPE && ((struct optix_window_title_bar *) new_selection)->window->widget.child) {
                struct optix_window_title_bar *window_title_bar = (struct optix_window_title_bar *) new_selection;
                if (window_title_bar->window->widget.child && window_title_bar->window->widget.child[0]) optix_SetCurrentSelection(window_title_bar->window->widget.child[0]);
            } else optix_SetCurrentSelection(new_selection);
        }
    }
}

uint16_t optix_GetNumElementsInStack(struct optix_widget *stack[]) {
    int i = 0;
    while (stack && stack[i]) i++;
    return i;
}

//returns the index of the found element
int optix_GetElementInStackByAddress(struct optix_widget *stack[], struct optix_widget *ptr) {
    int num_elements = optix_GetNumElementsInStack(stack);
    for (int i = 0; i < num_elements; i++)
        if (stack[i] == ptr) return i;
    return -1;
}

void optix_RemoveElementInStack(struct optix_widget **stack, int index, int num_elements) {
    //if (index < 0) return; 
    dbg_sprintf(dbgout, "Before: stack[index]: %d stack[index + 1]: %d\n", stack[index], stack[index + 1]);
    dbg_sprintf(dbgout, "Index: %d Num elements: %d Sizeof data: %d\n", index, num_elements, (num_elements - index) * sizeof(struct optix_widget *));
    memmove(&stack[index], &stack[index + 1], (num_elements - index) * sizeof(struct optix_widget *));
    dbg_sprintf(dbgout, "After: stack[index]: %d stack[index + 1]: %d\n", stack[index], stack[index + 1]);
    stack[num_elements] = NULL;
}