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
*/

struct optix_menu {
    struct optix_widget widget;
    int selection;
    int rows;
    int columns;
};

//functions
void optix_AlignMenu(struct optix_menu *menu);
void optix_UpdateMenu_default(struct optix_widget *widget);
void optix_RenderMenu_default(struct optix_widget *widget);

#endif