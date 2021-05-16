#ifndef MENU_H
#define MENU_H

#include "../globals.h"
#include "text.h"
#include "button.h"
#include "../util.h"

/*NOTES:
This will work best if the width of the menu is evenly divisible by its number of columns,
and the same for the height and number of rows. Otherwise, there could be some white space,
which we don't want.
EDIT: That's no longer an issue, it will always resize perfectly now (using modulus).
*/

#define MENU_NO_SELECTION -1

struct optix_menu {
    struct optix_widget widget;
    struct optix_resize_info resize_info;
    //centerings for the sprites and text
    struct optix_centering text_centering;
    struct optix_centering sprite_centering;
    char **text;
    gfx_sprite_t **spr;
    //things to use internally
    int selection;
    //option we start from (used for scrolling, mainly)
    int min;
    int rows;
    int columns;
    int num_options;
    //for partial redraw (overridden if the needs_redraw thing is set)
    int last_selection;
    //array of strings and sprites

};

//functions
//void optix_AlignMenu(struct optix_menu *menu, int menu_min);
//void optix_InitializeMenu(struct optix_menu *menu, int num_options, const char *(*text)[], gfx_sprite_t *(*spr)[]);

uint16_t optix_GetMenuOptionWidth(int curr_selection, int rows, int columns, uint16_t width, uint8_t height);
uint8_t optix_GetMenuOptionHeight(int curr_selection, int rows, int columns, uint16_t width, uint8_t height);
void optix_UpdateMenu_default(struct optix_widget *widget);
void optix_RenderMenu_default(struct optix_widget *widget);

#endif