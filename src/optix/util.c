#include "util.h"


//helpful GUI functions

//initializes a widget, and also sets its state to non-selected and visible (which will be updated elsewhere in the GUI)
//initializes the callbacks and things too
void optix_InitializeWidget(struct optix_widget *widget, uint8_t type) {
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
    };
    widget->type = type;
    if (type == OPTIX_TEXT_TYPE) widget->child = NULL;
    widget->state.selected = false;
    widget->state.visible = true;
    widget->update = update[type];
    widget->render = render[type];
    //element-specific things
    /*switch (type) {
        case OPTIX_BUTTON_TYPE:
            ((struct optix_button *) widget)->click_action = NULL;
            ((struct optix_button *) widget)->click_args = NULL;
            break;
    }*/
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
    optix_SetPosition(transform, reference->transform.x + ((reference->transform.width - transform->transform.width) / 2) * x_centering, 
    reference->transform.y + ((reference->transform.height - transform->transform.height) / 2) * y_centering);
    if (transform->type == OPTIX_TEXT_TYPE) {
        ((struct optix_text *) transform)->x_centering = x_centering;
        ((struct optix_text *) transform)->y_centering = y_centering;
    }
}

bool optix_CheckTransformOverlap(struct optix_widget *test, struct optix_widget *reference) {
    return gfx_CheckRectangleHotspot(test->transform.x, test->transform.y, test->transform.width, test->transform.height,
        reference->transform.x, reference->transform.y, reference->transform.width, reference->transform.height);
}