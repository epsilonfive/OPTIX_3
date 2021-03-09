#include "menu.h"

//quick function to align 
void optix_AlignMenu(struct optix_menu *menu) {
    int i = 0;
    while (menu->widget.child[i]) {
        struct optix_widget *child = menu->widget.child[i];
        //just continue if this won't be visible, but set it to invisible as well
        if (i > menu->rows * menu->columns - 1) {
            child->state.visible = false;
            i++;
            continue;
        } else child->state.visible = true;
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

//function to quickly initialize menu, because I was noticing I had to write this out a lot
//it does allocate things itself so the freeing function will have to be called at some point to get rid of those
//just pass it an array of text and an array of sprites and it will make a basic menu for you
//I'm making an executive decision that text comes before sprites when applicable
void optix_InitializeMenu(struct optix_menu *menu, int num_options, const char *(*text)[], gfx_sprite_t *(*spr)[]) {
    int i = 0;
    //start by allocating out the menu's children
    menu->widget.child = malloc((num_options + 1) * sizeof(struct optix_widget *));
    menu->widget.child[num_options] = NULL;
    //next go through and initialize each of those
    for (i = 0; i < num_options; i++) {
        //make a button and initialize it
        menu->widget.child[i] = malloc(sizeof(struct optix_button));
        optix_InitializeWidget(menu->widget.child[i], OPTIX_BUTTON_TYPE);
        //next put the text and sprites into that button
        //allocate it out
        //it always has to be at least 3 bytes, because it always has to contain a NULL at the end, or if there is nothing in the array
        menu->widget.child[i]->child = malloc((1 + (text && (*text)[i]) + (spr && (*spr)[i])) * sizeof(struct optix_widget *));
        //so put that null at the end
        menu->widget.child[i]->child[(text && (*text)[i]) + (spr && (*spr)[i])] = NULL;
        //if the text is not null add a new child to the button for it
        if (text && (*text)[i]) {
            struct optix_widget *new_text = menu->widget.child[i]->child[0] = malloc(sizeof(struct optix_text));
            ((struct optix_text *) new_text)->text = (*text)[i];
            optix_InitializeWidget(new_text, OPTIX_TEXT_TYPE);
        }
        //same for sprite 
        if (spr && (*spr)[i]) {
            struct optix_widget *new_spr = menu->widget.child[i]->child[(*text)[i] != NULL] = malloc(sizeof(struct optix_sprite));
            ((struct optix_sprite *) new_spr)->spr = (*spr)[i];
            optix_InitializeWidget(new_spr, OPTIX_SPRITE_TYPE);
        }
    }
    //general stuff
    optix_InitializeWidget(&menu->widget, OPTIX_MENU_TYPE);
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
