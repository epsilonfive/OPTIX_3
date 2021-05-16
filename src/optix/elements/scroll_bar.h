#ifndef SCROLL_BAR_H
#define SCROLL_BAR_H

//includes
#include "../globals.h"



//structs
struct optix_scroll_bar {
    struct optix_widget widget;
    //the number of rows, the logic is that you can have this point to the rows field of a menu or something
    int *num_rows;
    int *rows_displayed;
    //if there's columns we'll want to use this
    //int 


};

#endif