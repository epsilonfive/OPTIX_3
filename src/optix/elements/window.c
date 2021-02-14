#include "window.h"

void optix_UpdateWindow_default(struct optix_widget *widget) {
    struct optix_window *window = (struct optix_window *) widget;
    if (widget->child != NULL) optix_UpdateGUI(widget->child);
    //nothing here yet, but I should add some code to let you move and minimize
}

void optix_RenderWindow_default(struct optix_widget *widget) {
    struct optix_window *window = (struct optix_window *) widget;
    int title_bar_height = 14;
    optix_OutlinedRectangle(widget->transform.x - 1, widget->transform.y - 1, widget->transform.width + 2, widget->transform.height + 2, 0, OUTLINE_COLOR);
    //title bar because why not
    gfx_SetColor(OUTLINE_COLOR);
    gfx_FillRectangle(widget->transform.x - 1, widget->transform.y - title_bar_height, widget->transform.width + 2, title_bar_height);
    //make a title here somewhere eventually
    //render everything in it
    if (widget->child != NULL) optix_RenderGUI(widget->child);
}