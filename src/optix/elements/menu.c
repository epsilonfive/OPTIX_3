#include "menu.h"

//quick function to align 
void optix_AlignMenu(struct optix_menu *menu, int menu_min) {
    int i = 0;
    while (menu->widget.child[i]) {
        struct optix_widget *child = menu->widget.child[i];
        //just continue if this won't be visible, but set it to invisible as well
        if (i > menu->rows * menu->columns + menu->menu_min - 1 || i < menu->menu_min) {
            menu->widget.child[i]->state.visible = false;
            i++;
            continue;
        } else menu->widget.child[i]->state.visible = true;
        if (child->type == OPTIX_BUTTON_TYPE) {
            int default_width = menu->widget.transform.width / menu->columns;
            int default_height = menu->widget.transform.height / menu->rows;
            //resize the last entry in the row if it isn't the right size, because it irritates me if it's not
            child->transform.width = default_width + ((i - menu->menu_min) % (menu->columns - 1) == 0) * (menu->widget.transform.width % menu->columns);
            //the last row, i / menu->rows will be equal to 1
            child->transform.height = default_height + ((i - menu->menu_min) / (menu->rows - 1)) * (menu->widget.transform.height % menu->rows);
            child->transform.x = menu->widget.transform.x + (i - menu->menu_min) % menu->columns * default_width;
            child->transform.y = menu->widget.transform.y + (i - menu->menu_min) / menu->columns * default_height;
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

//check if a menu needs to be scrolled
void optix_ScrollMenu(struct optix_menu *menu) {
    if (optix_gui_data.can_press && kb_Data[7]) {
        int att_option = menu->selection;
        //first, get the number of options that are in the menu
        int num_options = 0;
        if (menu->widget.child) while (menu->widget.child[num_options]) num_options++;
        if (num_options <= menu->rows * menu->columns) return;
        if (kb_Data[7] & kb_Up)         att_option -= menu->columns;
        else if (kb_Data[7] & kb_Down)  att_option += menu->columns;
        else if (kb_Data[7] & kb_Right) att_option += ((menu->selection % menu->columns != 0 && menu->selection % menu->columns != menu->columns - 1) || menu->columns == 1);
        else if (kb_Data[7] & kb_Left)  att_option -= ((menu->selection % menu->columns != 0 && menu->selection % menu->columns != menu->columns - 1) || menu->columns == 1);
        //wrap if necessary
        //eh just do this later
        //actually scroll it, starting with the menu min
        if (att_option < menu->menu_min || att_option > menu->menu_min + menu->rows * menu->columns - 1) {
            if (att_option < menu->menu_min) menu->menu_min -= menu->columns;
            else menu->menu_min += menu->columns;
            dbg_sprintf(dbgout, "Success.\n");
            optix_AlignMenu(menu, menu->menu_min);
            menu->widget.state.needs_redraw = true;
            dbg_sprintf(dbgout, "Menu min: %d, menu selection: %d", menu->menu_min, menu->selection);
            optix_gui_data.can_press = false;
            optix_cursor.current_selection = menu->widget.child[att_option];
        }
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
    optix_ScrollMenu(menu);
    //see which option is selected
    while (widget->child[i]) {
        //we're hoping this is a button
        if (widget->state.visible && widget->state.selected) {
            if (found_selection) {
                if (widget->child[i]->type == OPTIX_BUTTON_TYPE && widget->child[i]->state.selected) {
                    widget->child[i]->state.needs_redraw = true;
                    optix_RecursiveSetNeedsRedraw(widget->child);
                }
                widget->child[i]->state.selected = false;
            } else {
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
        } else {
            if (widget->child[i]->type == OPTIX_BUTTON_TYPE && widget->child[i]->state.selected) {
                widget->child[i]->state.needs_redraw = true;
                optix_RecursiveSetNeedsRedraw(widget->child);
            }
            widget->child[i]->state.selected = false;
        }
        i++;
    }
}

void optix_RenderMenu_default(struct optix_widget *widget) {
    struct optix_menu *menu = (struct optix_menu *) widget;
    if (widget->child) optix_RenderStack(widget->child);
}
