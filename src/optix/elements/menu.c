#include "menu.h"
//includes
#include <stdint.h>
#include <stdbool.h>
#include <graphx.h>
#include <keypadc.h>
#include <debug.h>
//OPTIX includes
#include "../gui_control.h"
#include "../cursor.h"
#include "../init.h"
#include "../util.h"
#include "button.h"

//Menu rewrite 1
//curr_selection must be curr_selection - menu_min, or the option's place in the grid
uint16_t optix_GetMenuOptionWidth(int option, struct optix_menu *menu) {
    struct optix_transform *t = &(menu->widget.transform);
    return (t->width / menu->columns) + (((option - menu->min) + 1) % menu->columns == 0) * (t->width % menu->columns);
}

uint8_t optix_GetMenuOptionHeight(int option, struct optix_menu *menu) {
    struct optix_transform *t = &(menu->widget.transform);
    return (t->height / menu->rows) + ((option - menu->min) / menu->columns == menu->rows - 1) * (t->height % menu->rows);
}

int optix_GetMenuOptionRelativeX(int option, struct optix_menu *menu) {
    return option % menu->columns * optix_GetMenuOptionWidth(0, menu);
}

int optix_GetMenuOptionRelativeY(int option, struct optix_menu *menu) {
    return (option - menu->min) / menu->columns * optix_GetMenuOptionHeight(0, menu);
}

void optix_SetMenuOptionTransform(int option, struct optix_button *button, struct optix_menu *menu) {
    struct optix_widget *widget = (struct optix_widget *) menu;
    struct optix_transform *transform = &(button->widget.transform);
    transform->x = widget->transform.x + optix_GetMenuOptionRelativeX(option, menu);
    transform->y = widget->transform.y + optix_GetMenuOptionRelativeY(option, menu);
    transform->width = optix_GetMenuOptionWidth(option, menu);
    transform->height = optix_GetMenuOptionHeight(option, menu);
}

void optix_UpdateMenu_default(struct optix_widget *widget) {
    struct optix_menu *menu = (struct optix_menu *) widget;
    //if we should check if we want to jump out of the menu (tries to access inapplicable option, like too far up or down or left or right)
    bool needs_jump = false;
    if (menu->selection != MENU_NO_SELECTION) menu->last_selection = menu->selection;
    //check if it overlaps with the cursor
    if (widget->state.visible) {
        //so if there is an element, only the one within the currently selected box will be updated
        //I think this makes sense?
        if (widget->child && widget->child[menu->last_selection] && widget->child[menu->last_selection]->update)
            widget->child[menu->last_selection]->update(widget->child[menu->last_selection]);
    }
    if (widget->state.selected) {
        //handle if it was pressed
        menu->needs_partial_redraw = false;
        //restore the previous selection if cursor was off, and also draw the selection box back again
        if (!menu->selected_save) {
            menu->selection = menu->last_selection;
            menu->needs_partial_redraw = true;
        }
        if (optix_DefaultKeyIsDown(KEY_ENTER) & KEY_PRESSED) {
            dbg_sprintf(dbgout, "Click args: %d\n", menu->click_action.click_args);
            if (menu->click_action.click_action) {
                menu->click_action.click_action(menu->pass_self ? widget : menu->click_action.click_args);
                //we don't want it to repeat
                optix_SetDefaultKeyState(KEY_ENTER, KEY_HELD);
            } else {
                //this should be the same as clicking the thing if applicable
                if (widget->child[menu->selection]->type == OPTIX_BUTTON_TYPE || widget->child[menu->selection]->type == OPTIX_CHECK_BOX_TYPE) {
                    struct optix_button *button = (struct optix_button *) widget->child[menu->selection];
                    if (button->click_action.click_action) button->click_action.click_action(button->click_action.click_args);
                }
            }
            widget->state.needs_redraw = true;
        } else if (optix_DefaultKeyIsDown(KEY_ENTER) & KEY_RELEASED)
            widget->state.needs_redraw = true;
        //only do this if we have to
        if (current_context->settings->cursor_active) {
            uint16_t option_width = optix_GetMenuOptionWidth(0, menu);
            uint8_t option_height = optix_GetMenuOptionHeight(0, menu);
            int potential_selection;
            //handle scrolling
            if (optix_DefaultKeyIsDown(KEY_ADD) & KEY_PRESSED) {
                menu->min = menu->min + menu->columns < menu->num_options - (menu->rows - 1) * menu->columns ? menu->min + menu->columns : menu->min;
                widget->state.needs_redraw = true;
            } else if (optix_DefaultKeyIsDown(KEY_SUB) & KEY_PRESSED) {
                menu->min = menu->min - menu->columns > 0 ? menu->min - menu->columns : 0;
                widget->state.needs_redraw = true;
            }
            potential_selection = menu->min + (((current_context->cursor->widget.transform.y - widget->transform.y) / option_height) * menu->columns + 
            ((current_context->cursor->widget.transform.x - widget->transform.x) / option_width));
            if (potential_selection < menu->num_options) {
                //if we got there from an option that was greater than the max
                if (menu->selection == MENU_NO_SELECTION && menu->selected_save)
                    menu->needs_partial_redraw = true;
                menu->selection = potential_selection;
            } else {
                if (menu->selected_save && menu->selection != MENU_NO_SELECTION)
                    menu->needs_partial_redraw = true;
                menu->selection = MENU_NO_SELECTION;
            }
        } else if (current_context->cursor->current_selection == widget) {
            bool key_pressed = false;
            if (optix_DefaultKeyIsDown(KEY_UP) & KEY_PRESSED) {
                menu->selection = (needs_jump = (menu->selection == 0)) ? 0 : menu->selection - menu->columns;
                key_pressed = true;
            }
            if (optix_DefaultKeyIsDown(KEY_DOWN) & KEY_PRESSED) {
                menu->selection = (needs_jump = (menu->selection == menu->num_options - 1)) ? menu->num_options - 1 : menu->selection + menu->columns;
                key_pressed = true;
            }
            //for both of these, don't do it if at the edge
            if (optix_DefaultKeyIsDown(KEY_LEFT) & KEY_PRESSED && !(needs_jump = menu->selection % menu->columns == 0)) {
                menu->selection--;
                key_pressed = true;
            }
            if (optix_DefaultKeyIsDown(KEY_RIGHT) & KEY_PRESSED && !(needs_jump = (((menu->selection + 1) % menu->columns) == 0))) {
                menu->selection++;
                key_pressed = true;
            }
            //if it's a transparent background we're going to need to redraw things
            if (key_pressed)
                if (menu->transparent_background && !menu->hide_selection_box) optix_IntelligentRecursiveSetNeedsRedraw((struct optix_widget **) current_context->stack, widget);
            //make sure we don't jump out of the menu where applicable
            if (!needs_jump) current_context->cursor->direction = OPTIX_CURSOR_NO_DIR;
        }
        //scroll if we need to
        //handle out of bounds
        if (menu->selection != MENU_NO_SELECTION) {
            if (menu->selection < 0) menu->selection = 0;
            if (menu->selection >= menu->num_options && menu->selection != MENU_NO_SELECTION)
                menu->selection = menu->num_options - 1;
            if (menu->selection < menu->min && menu->selection != MENU_NO_SELECTION) {
                menu->min = ((int) menu->selection / menu->columns) * menu->columns;
                widget->state.needs_redraw = true;
            } else if (menu->selection > menu->min + (menu->rows - 1) * menu->columns) {
                menu->min = ((int) menu->selection / menu->columns - (menu->rows - 1)) * menu->columns;
                widget->state.needs_redraw = true;
            }
        }
    } else {
        //if it was selected the last loop, signal for the last selection to be redrawn as unselected
        if (menu->selection != MENU_NO_SELECTION) menu->needs_partial_redraw = true;
        menu->selection = MENU_NO_SELECTION;
    }
    menu->selected_save = widget->state.selected;
}

//offload some of this here for simplicity
void optix_RenderMenuOption(int option, struct optix_menu *menu, char *option_text, gfx_sprite_t *option_spr) {
    struct optix_widget *widget = (struct optix_widget *) menu;
    //we're going to be handling this by not handling it (just make the menu be a glorified collection of buttons, and rendered the same way)
    //each has some text and some sprite
    struct optix_text text;
    struct optix_sprite sprite;
    struct optix_widget *button_children[] = {&text.widget, &sprite.widget, NULL, NULL};
    struct optix_button button = {.widget = {.child = button_children}};
    optix_InitializeWidget(&button.widget, OPTIX_BUTTON_TYPE);
    if (widget->state.needs_redraw || option == menu->selection || option == menu->last_selection) {
        button.widget.child = option < menu->num_options ? button_children : NULL;
        button.pressed = (option == menu->selection && optix_DefaultKeyIsDown(KEY_ENTER) & KEY_HELD);
        button.transparent_background = menu->transparent_background || (option == menu->selection && menu->hide_selection_box);
        if (button.widget.child && option_text) {
            memcpy(&text.widget, &(menu->text_args), sizeof(struct optix_text));
            text.text = option_text;
            text.widget.child = NULL;
            optix_InitializeTextTransform(&text);
            optix_InitializeWidget(&text.widget, OPTIX_TEXT_TYPE);
        } else text.text = NULL;
        //same for sprites
        if (button.widget.child && option_spr) {
            memcpy(&sprite.widget, &(menu->sprite_args), sizeof(struct optix_sprite));
            sprite.spr = option_spr;
            sprite.widget.child = NULL;
            optix_InitializeWidget(&sprite.widget, OPTIX_SPRITE_TYPE);
        } else sprite.spr = NULL;
        //place the null, add the special element if applicable
        if ((button.widget.child || widget->child) && option < menu->num_options) {
            button.widget.child[0] = &text.widget;
            button.widget.child[(text.text != NULL)] = &sprite.widget;
            //put the element in there to be rendered as well
            if (widget->child && widget->child[option])
                button.widget.child[(text.text != NULL) + (sprite.spr != NULL)] = widget->child[option];
            button.widget.child[(text.text != NULL) + (sprite.spr != NULL) + (widget->child != NULL && widget->child[option] != NULL)] = NULL;
        }
        //handle the transform
        optix_SetMenuOptionTransform(option, &button, menu);
        //this option will be shown as selected if it is selected and the selection box is not hidden
        if (button.widget.child) {
            optix_RecursiveAlign(&button.widget);
            button.widget.state.selected = (option == menu->selection && !menu->hide_selection_box);
            optix_RecursiveSetNeedsRedraw(button.widget.child);
        }
        button.widget.render(&button.widget);
    }
}


void optix_RenderMenu_default(struct optix_widget *widget) {
    struct optix_menu *menu = (struct optix_menu *) widget;
    //return if we don't have to do anything
    if (!(widget->state.needs_redraw || (menu->selection != menu->last_selection && widget->state.selected && menu->selection != MENU_NO_SELECTION) || menu->needs_partial_redraw)) return;
    //if ((!(widget->state.needs_redraw || menu->selection != menu->last_selection))) return;
    //just draw it
    for (int i = menu->min; i < menu->min + menu->rows * menu->columns; i++) {
        char *option_text = menu->text && menu->text[i] ? menu->text[i] : NULL;
        gfx_sprite_t *option_spr = menu->spr && menu->spr[i] ? menu->spr[i] : NULL;
        optix_RenderMenuOption(i, menu, option_text, option_spr);
    }
    //this will only be run if we didn't return earlier, which will happen if the selection is different or we need a redraw of some sort
    current_context->data->needs_blit = true;
    menu->needs_partial_redraw = false;
    widget->state.needs_redraw = false;
}