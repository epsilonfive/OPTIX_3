#ifndef MENU_H
#define MENU_H

struct optix_menu {
    struct optix_widget widget;
    int selection;
    int rows;
    int columns;
};

#endif