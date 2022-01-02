#ifndef MENU_H
#define MENU_H

#include "button.h"
#include "sprite.h"
#include "text.h"

/*NOTES:
This will work best if the width of the menu is evenly divisible by its number of columns,
and the same for the height and number of rows. Otherwise, there could be some white space,
which we don't want.
EDIT: That's no longer an issue, it will always resize perfectly now (using modulus).
*/

#define MENU_NO_SELECTION -1

struct optix_menu {
    //if widget.child is set, then these widgets will be updated with the menu and rendered
    //within the menu boxes (so child[0] would be rendered within the box for the 0th menu option)
    struct optix_widget widget;
    //centerings for the sprites and text
    struct optix_text text_args;
    struct optix_sprite sprite_args;
    //minimum option sizes
    uint16_t min_option_width;
    uint8_t min_option_height;
    //if this is set, the text and sprite arrays will be freed on optix_FreeElement
    bool dynamic;
    char **text;
    //sprite things
    gfx_sprite_t **spr;
    //things to use internally
    int selection;
    bool needs_partial_redraw;
    bool selected_save;
    //option we start from (used for scrolling, mainly)
    int min;
    int rows;
    int columns;
    int num_options;
    //for partial redraw (overridden if the needs_redraw thing is set)
    int last_selection;
    //array of strings and sprites
    //you could do some hacky thing with passing the struct to itself or something here
    //or even pointer to the currently selected element
    //up to the programmer, yay
    struct optix_click_action click_action;
    //if enabled, it will pass a pointer to itself instead of click_args
    bool pass_self;
    //whether background of menu should be transparent or not (literally just toggles transparent_background
    //in the produced button)
    bool transparent_background;
    //whether we should draw the selection box
    bool hide_selection_box;
};

//functions
//void optix_AlignMenu(struct optix_menu *menu, int menu_min);
//void optix_InitializeMenu(struct optix_menu *menu, int num_options, const char *(*text)[], gfx_sprite_t *(*spr)[]);

uint16_t optix_GetMenuOptionWidth(int option, struct optix_menu *menu);
uint8_t optix_GetMenuOptionHeight(int option, struct optix_menu *menu);
void optix_SetMenuOptionTransform(int option, struct optix_button *button, struct optix_menu *menu);
void optix_RenderMenuOption(int option, struct optix_menu *menu, char *option_text, gfx_sprite_t *option_spr);
void optix_UpdateMenu_default(struct optix_widget *widget);
void optix_RenderMenu_default(struct optix_widget *widget);

#endif