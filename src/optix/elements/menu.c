#include "menu.h"

//quick function to align 
void optix_AlignMenu(struct optix_menu *menu) {
    int i = 0;
    dbg_sprintf(dbgout, "(menu) width: %d, height: %d", menu->widget.transform.width, menu->widget.transform.height);
    while (menu->widget.child[i]) {
        struct optix_widget *child = menu->widget.child[i];
        //just continue if this won't be visible, but set it to invisible as well
        if (i > menu->rows * menu->columns - 1) {
            child->state.visible = false;
            i++;
            continue;
        } else child->state.visible = true;
        dbg_sprintf(dbgout, "Entry %d\n", i);
        if (child->type == OPTIX_BUTTON_TYPE) {
            int default_width = menu->widget.transform.width / menu->columns;
            int default_height = menu->widget.transform.height / menu->rows;
            //resize the last entry in the row if it isn't the right size, because it irritates me if it's not
            child->transform.width = default_width + (i % (menu->columns - 1) == 0) * (menu->widget.transform.width % menu->columns);
            //the last row, i / menu->rows will be equal to 1
            child->transform.height = default_height + (i / (menu->rows - 1)) * (menu->widget.transform.height % menu->rows);
            child->transform.x = menu->widget.transform.x + i % menu->columns * default_width;
            child->transform.y = menu->widget.transform.y + i / menu->columns * default_height;
            //realign it if necessary
            if (child->child) {
                int j = 0;
                while (child->child[j]) {
                    dbg_sprintf(dbgout, "%d\n", child->child[j]->type);
                    if (child->child[j]->type == OPTIX_TEXT_TYPE) {
                        struct optix_text *child_text = (struct optix_text *) child->child[j];
                        optix_AlignTransformToTransform(child->child[j], child, child_text->widget.centering.x_centering, child_text->widget.centering.y_centering);
                    } else if (child->type == OPTIX_SPRITE_TYPE) {
                        //implement later
                        struct optix_sprite *child_sprite = (struct optix_sprite *) child->child[j];
                        //optix_AlignTransformToTransform(child->child[j], )
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
    int i = 0;
    //check if it overlaps with the cursor
    if (optix_CheckTransformOverlap(&optix_cursor.widget, widget)) widget->state.selected = true;
    else widget->state.selected = false;
    //see which option is selected
    while (widget->child[i] != NULL) {
        //we're hoping this is a button
        if (widget->state.selected) {
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
        } else widget->child[i]->state.selected = false;
        i++;
    }
}

void optix_RenderMenu_default(struct optix_widget *widget) {
    struct optix_menu *menu = (struct optix_menu *) widget;
    if (widget->child != NULL) optix_RenderStack(widget->child);
}
