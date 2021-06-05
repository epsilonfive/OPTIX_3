#include "init.h"

//initializes a widget, and also sets its state to non-selected and visible (which will be updated elsewhere in the GUI)
//initializes the callbacks and things too
void optix_InitializeWidget(struct optix_widget *widget, uint8_t type) {
    struct optix_window_title_bar *window_title_bar = (struct optix_window_title_bar *) widget;
    struct optix_window *window = (struct optix_window *) window_title_bar->window;
    struct optix_menu *menu = (struct optix_menu *) widget;
    struct optix_text *text = (struct optix_text *) widget;
    struct optix_check_box *check_box = (struct optix_check_box *) widget;
    void (*update[OPTIX_NUM_TYPES])(struct optix_widget *) = {
        //text
        optix_UpdateText_default,
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
        //input box
        optix_UpdateInputBox_default,
        //scroll bar
        optix_UpdateScrollBar_default,
        //check box
        optix_UpdateButton_default,
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
        //input box
        optix_RenderInputBox_default,
        //scroll bar
        optix_RenderScrollBar_default,
        //check box
        optix_RenderCheckBox_default,
    };
    bool selectable[OPTIX_NUM_TYPES] = {
        //text
        true,
        //sprite
        false,
        //button
        true,
        //menu
        true,
        //window
        true,
        //window title bar
        true,
        //divider
        false,
        //rectangle
        false,
        //input box
        false,
        //scroll bar
        false,
        //check box
        true,
    };
    //if we're adding stuff we probably want this
    //current_context->data->gui_needs_full_redraw = true;
    widget->type = type;
    widget->state.selected = false;
    widget->state.needs_redraw = true;
    widget->state.visible = true;
    widget->update = update[type];
    widget->render = render[type];
    widget->state.selectable = selectable[type];
    //element-specific things
    switch (type) {
        case OPTIX_MENU_TYPE:
            menu->min = menu->last_selection = menu->selection = menu->num_options = 0;
            do menu->num_options++;
            while ((menu->spr && menu->spr[menu->num_options]) || (menu->text && menu->text[menu->num_options]));
            menu->num_options--;
            break;
        case OPTIX_TEXT_TYPE:
            widget->child = NULL;
            text->num_lines = 0;
            text->background_rectangle = true;
            //text->min = 0; 
            optix_WrapText(widget);
            //optix_InitializeTextTransform((struct optix_text *) widget);
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
        case OPTIX_CHECK_BOX_TYPE:
            check_box->button.click_action = optix_CheckBoxValueXOR;
            check_box->button.click_args = (void *) widget;
            break;
    }
}