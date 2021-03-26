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

struct optix_menu {
    struct optix_widget widget;
    struct optix_resize_info resize_info;
    int selection;
    int menu_min;
    int rows;
    int columns;
};

//functions
void optix_AlignMenu(struct optix_menu *menu, int menu_min);
void optix_InitializeMenu(struct optix_menu *menu, int num_options, const char *(*text)[], gfx_sprite_t *(*spr)[]);
void optix_UpdateMenu_default(struct optix_widget *widget);
void optix_RenderMenu_default(struct optix_widget *widget);

#endif