#include "util.h"


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

//Aligns a transform to another transform. Use OPTIX_CENTERING_LEFT, RIGHT, etc.
//transform is aligned to reference based on its width and height
void optix_AlignTransformToTransform(struct optix_widget *transform, struct optix_widget *reference, uint8_t x_centering, uint8_t y_centering) {
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
    if (widget->type == OPTIX_WINDOW_TITLE_BAR_TYPE) optix_RecursiveAlign(((struct optix_window_title_bar *) widget)->window);
    else if (widget->type == OPTIX_INPUT_BOX_TYPE) {
        struct optix_input_box *input_box = (struct optix_input_box *) widget;
        optix_AlignTransformToTransform(input_box->text, input_box, input_box->text->widget.centering.x_centering, input_box->text->widget.centering.y_centering);
    }
    if (widget->child) {
        int i = 0;
        while (widget->child[i]) {
            struct optix_widget *child = widget->child[i];
            optix_AlignTransformToTransform(child, widget, child->centering.x_centering, child->centering.y_centering);
            //also do this if it's an input box
            if (child->type == OPTIX_INPUT_BOX_TYPE) {
                struct optix_input_box *input_box = (struct optix_input_box *) child;
                optix_AlignTransformToTransform(input_box->text, input_box, input_box->text->widget.centering.x_centering, input_box->text->widget.centering.y_centering);
            }
            if (child->child) {
                //if (child->type == OPTIX_MENU_TYPE) optix_AlignMenu((struct optix_menu *) child, 0);
                //else optix_RecursiveAlign(child);
                optix_RecursiveAlign(child);
            }
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
            if (stack[i]->type == OPTIX_WINDOW_TYPE || stack[i]->type == OPTIX_WINDOW_TITLE_BAR_TYPE) {
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

