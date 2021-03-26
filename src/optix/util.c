#include "util.h"


//helpful GUI functions

//initializes a widget, and also sets its state to non-selected and visible (which will be updated elsewhere in the GUI)
//initializes the callbacks and things too
void optix_InitializeWidget(struct optix_widget *widget, uint8_t type) {
    struct optix_window_title_bar *window_title_bar = (struct optix_window_title_bar *) widget;
    struct optix_window *window = (struct optix_window *) window_title_bar->window;
    void (*update[OPTIX_NUM_TYPES])(struct optix_widget *) = {
        //text
        NULL,
        //sprite
        NULL,
        //button
        optix_UpdateButton_default,
        //menu
        optix_UpdateMenu_default,
        //window
        optix_UpdateWindow_default,
        //window title bar
        optix_UpdateWindowTitleBar_default,
        //divider
        NULL,
        //rectangle
        NULL,
    };
    void (*render[OPTIX_NUM_TYPES])(struct optix_widget *) = {
        //text
        optix_RenderText_default,
        //sprite
        optix_RenderSprite_default,
        //button
        optix_RenderButton_default,
        //menu
        optix_RenderMenu_default,
        //window
        optix_RenderWindow_default,
        //window title bar
        optix_RenderWindowTitleBar_default,
        //divider
        optix_RenderDivider_default,
        //rectangle
        optix_RenderRectangle_default,
    };
    //if we're adding stuff we probably want this
    optix_gui_data.gui_needs_full_redraw = true;
    widget->type = type;
    widget->state.selected = false;
    widget->state.needs_redraw = true;
    widget->state.visible = true;
    widget->update = update[type];
    widget->render = render[type];
    //element-specific things
    switch (type) {
        case OPTIX_TEXT_TYPE:
            widget->child = NULL;
            optix_InitializeTextTransform((struct optix_text *) widget);
            //no break here, we want it to fall through
        case OPTIX_BUTTON_TYPE:
        case OPTIX_SPRITE_TYPE:
            widget->centering.x_centering = OPTIX_CENTERING_CENTERED;
            widget->centering.y_centering = OPTIX_CENTERING_CENTERED;
            break;
        case OPTIX_WINDOW_TITLE_BAR_TYPE:
            //initialize the transform for this as well
            widget->transform.x = window->widget.transform.x;
            widget->transform.y = window->widget.transform.y - TITLE_BAR_HEIGHT;
            widget->transform.width = window->widget.transform.width;
            widget->transform.height = TITLE_BAR_HEIGHT;
            break;
    }
}

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
    int i = 0;
    //we need to align this as well
    if (widget->type == OPTIX_WINDOW_TITLE_BAR_TYPE) {
        //we're also going to refresh the title bar
        struct optix_window_title_bar *window_title_bar = (struct optix_window_title_bar *) widget;
        if (widget->child) {
            i = 0;
            while (widget->child[i]) {
                struct optix_widget *child = widget->child[i];
                optix_AlignTransformToTransform(child, widget, child->centering.x_centering, child->centering.y_centering);
                if (child->child) {
                    if (child->type == OPTIX_MENU_TYPE) optix_AlignMenu((struct optix_menu *) child, 0);
                    else optix_RecursiveAlign(child);
                }
                i++;
            }
        }
        //make us align the window instead
        widget = (struct optix_widget *) window_title_bar->window;
    }
    if (widget->child) {
        i = 0;
        while (widget->child[i]) {
            struct optix_widget *child = widget->child[i];
            optix_AlignTransformToTransform(child, widget, child->centering.x_centering, child->centering.y_centering);
            if (child->child) {
                if (child->type == OPTIX_MENU_TYPE) optix_AlignMenu((struct optix_menu *) child, 0);
                else optix_RecursiveAlign(child);
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
        while (stack[i]) {
            if (stack[i]->state.selected) {
                selected_index = i;
                break;
            }
            i++;
        }
        //deselect the currently selected one
        stack[selected_index]->state.selected = false;
        if (stack[selected_index]->type == OPTIX_WINDOW_TITLE_BAR_TYPE)
            ((struct optix_window_title_bar *) stack[selected_index])->window->widget.state.selected = false;
        //check if the next one is eligible
        new_selection = stack[selected_index + 1] ? stack[selected_index + 1] : stack[0];
        new_selection->state.selected = new_selection->state.needs_redraw = true;
        //apparently this could cause issues?
        if (new_selection->type == OPTIX_WINDOW_TITLE_BAR_TYPE) {
            struct optix_window *window = ((struct optix_window_title_bar *) new_selection)->window;
            window->widget.state.needs_redraw = window->widget.state.selected = true;
        }
        if (new_selection->child) {
            //just have it be the first child I guess
            optix_cursor.widget.transform.x = new_selection->child[0]->transform.x;
            optix_cursor.widget.transform.y = new_selection->child[0]->transform.y;
        } else {
            optix_cursor.widget.transform.x = new_selection->transform.x;
            optix_cursor.widget.transform.y = new_selection->transform.y;
        }
    }
}

