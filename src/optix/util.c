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
    //start by figuring out what the difference is between the new and old positions
    int x_shift = x - widget->transform.x;
    int y_shift = y - widget->transform.y;
    //shift the widget itself
    widget->transform.x = x;
    widget->transform.y = y;
    //move its children as well
    if (widget->child != NULL) {
        int i = 0;
        while (widget->child[i] != NULL) {
            struct optix_widget *child = widget->child[i];
            optix_SetPosition(child, child->transform.x + x_shift, child->transform.y + y_shift);
            i++;
        }
    }
}

//Aligns a transform to another transform. Use OPTIX_CENTERING_LEFT, RIGHT, etc.
//transform is aligned to reference based on its width and height
void optix_AlignTransformToTransform(struct optix_widget *transform, struct optix_widget *reference, uint8_t x_centering, uint8_t y_centering) {
    transform->transform.x = reference->transform.x + ((reference->transform.width - transform->transform.width) / 2) * x_centering;
    if (x_centering == OPTIX_CENTERING_RIGHT) transform->transform.x += ((reference->transform.width - transform->transform.width) % 2);
    transform->transform.y = reference->transform.y + ((reference->transform.height - transform->transform.height) / 2) * y_centering;
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
        if (widget->child != NULL) {
            int i = 0;
            while (widget->child[i]) {
                struct optix_widget *child = widget->child[i];
                optix_AlignTransformToTransform(child, widget, child->centering.x_centering, child->centering.y_centering);
                optix_RecursiveAlign(widget->child[i]);
                i++;
            }
        }
        //make us align the window instead
        widget = (struct optix_widget *) window_title_bar->window;
    }
    while (widget->child[i] != NULL) {
        struct optix_widget *child = widget->child[i];
        optix_AlignTransformToTransform(child, widget, child->centering.x_centering, child->centering.y_centering);
        if (child->child != NULL) {
            if (child->type == OPTIX_MENU_TYPE) optix_AlignMenu((struct optix_menu *) child);
            else optix_RecursiveAlign(child);
        }
        i++;
    }
}
