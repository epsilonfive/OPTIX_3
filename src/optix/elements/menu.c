#include "menu.h"

//quick function to align 
void optix_AlignMenu(struct optix_menu *menu) {
    int i = 0;
    while (menu->widget.child[i] != NULL) {
        struct optix_widget *child = menu->widget.child[i];
        if (child->type == OPTIX_BUTTON_TYPE) {
            child->transform.width = menu->widget.transform.width / menu->columns;
            child->transform.height = menu->widget.transform.height / menu->rows;
            child->transform.x = menu->widget.transform.x + i % menu->columns * child->transform.width;
            child->transform.y = menu->widget.transform.y + i / menu->columns * child->transform.height;
            //realign it if necessary
            if (child->child != NULL) {
                int j = 0;
                while (child->child[j] != NULL) {
                    dbg_sprintf(dbgout, "Type is %d\n", child->child[j]->type);
                    if (child->child[j]->type == OPTIX_TEXT_TYPE) {
                        struct optix_text *child_text = (struct optix_text *) child->child[j];
                        optix_AlignTransformToTransform(child->child[j], child, child_text->x_centering, child_text->y_centering);
                    } else if (child->type == OPTIX_SPRITE_TYPE) {
                        //implement later
                    }
                    j++;
                }
            }
        }
        i++;
    }
}



void optix_UpdateMenu_default(struct optix_widget *widget) {
    struct optix_menu *menu = (struct optix_menu *) widget;
    bool found_selection = false;
    //check if it overlaps with the cursor
    if (gfx_CheckRectangleHotspot(widget->transform.x, widget->transform.y, widget->transform.width * menu->columns, widget->transform.height * menu->rows, 
                                  optix_cursor.x, optix_cursor.y, 10, 10)) widget->state.selected = true;
    if (widget->state.selected) {
        //see which option is selected
        int i = 0;
        while (widget->child[i] != NULL) {
            //we're hoping this is a button
            if (found_selection) widget->child[i]->state.selected = false;
            else {
                widget->child[i]->update(widget->child[i]);
                if (widget->child[i]->type == OPTIX_BUTTON_TYPE) {
                    //check if it's selected now
                    if (widget->child[i]->state.selected) {
                        menu->selection = i;
                        //we can only have one selected item
                        found_selection = true;
                    }
                }
            }
            i++;
        }
    }
}

void optix_RenderMenu_default(struct optix_widget *widget) {
    struct optix_menu *menu = (struct optix_menu *) widget;
    int i = 0;
    while (widget->child[i] != NULL) {
        //we're hoping this is a button
        if (widget->child[i]->type == OPTIX_BUTTON_TYPE) widget->child[i]->render(widget->child[i]);
        i++;
    }
}
