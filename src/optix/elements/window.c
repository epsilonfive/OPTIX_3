#include "window.h"

void optix_UpdateWindow_default(struct optix_widget *widget) {
    struct optix_window *window = (struct optix_window *) widget;
    if (widget->state.selected && widget->child != NULL) optix_UpdateStack(widget->child);
    if (kb_Data[6] & kb_Enter || kb_Data[1] & kb_2nd) {
        if (optix_CheckTransformOverlap(&optix_cursor.widget, widget)) {
            widget->state.selected = true;
        } else widget->state.selected = false;
    }
}

void optix_RenderWindow_default(struct optix_widget *widget) {
    struct optix_window *window = (struct optix_window *) widget;
    int title_bar_side_padding = 2;
    int element_size = 10;
    optix_OutlinedRectangle(widget->transform.x - 1, widget->transform.y - 1, widget->transform.width + 2, widget->transform.height + 2, optix_colors.window_bg, optix_colors.window_border);
    //render everything in it
    if (widget->child != NULL) optix_RenderStack(widget->child);
}

void optix_UpdateWindowTitleBar_default(struct optix_widget *widget) {
    struct optix_window_title_bar *window_title_bar = (struct optix_window_title_bar *) widget;
    bool window_selected = window_title_bar->window->widget.state.selected;
    //windows are children of the title bar
    //make it so you can move the windows around while holding the selection key and moving the mouse
    if (kb_Data[6] & kb_Enter || kb_Data[1] & kb_2nd) {
        if (optix_CheckTransformOverlap(&optix_cursor.widget, widget)) {
            if (window_selected && (optix_cursor.last_x != optix_cursor.widget.transform.x || optix_cursor.last_y != optix_cursor.widget.transform.y)) {
                int x_pos = widget->transform.x + (optix_cursor.widget.transform.x - optix_cursor.last_x);
                int y_pos = widget->transform.y + (optix_cursor.widget.transform.y - optix_cursor.last_y);
                if (x_pos < 1) x_pos = 1;
                if (y_pos < 1) y_pos = 1;
                if (x_pos + window_title_bar->window->widget.transform.width >  319) x_pos = 319 - window_title_bar->window->widget.transform.width;
                if (y_pos + window_title_bar->window->widget.transform.height > 239) y_pos = 239 - window_title_bar->window->widget.transform.height;
                optix_SetPosition(widget, x_pos, y_pos);
                optix_SetPosition(window_title_bar->window, x_pos, y_pos + widget->transform.height);
            } else window_title_bar->window->widget.state.selected = true;
            if (window_selected) optix_cursor.state = OPTIX_CURSOR_MOVE;
            else optix_cursor.state = OPTIX_CURSOR_OVER_ITEM;
        }
        //change the cursor icon
    }
    window_title_bar->window->widget.update((struct optix_widget *) window_title_bar->window);
    widget->state.selected = window_title_bar->window->widget.state.selected;
}

void optix_RenderWindowTitleBar_default(struct optix_widget *widget) {
    struct optix_window_title_bar *window_title_bar = (struct optix_window_title_bar *) widget;
    if (window_title_bar->window->widget.state.selected) {
        optix_OutlinedRectangle(widget->transform.x - 1, widget->transform.y, widget->transform.width + 2, widget->transform.height, //transform
        optix_colors.window_title_bar_selected, optix_colors.window_border);                                                         //color
        //optix_SetTextColor(optix_colors.window_title_text_fg_selected, optix_colors.window_title_text_bg_selected);
    } else {
        optix_OutlinedRectangle(widget->transform.x - 1, widget->transform.y, widget->transform.width + 2, widget->transform.height, //transform
        optix_colors.window_title_bar_unselected, optix_colors.window_border);                                                       //color
        //optix_SetTextColor(optix_colors.window_title_text_fg_unselected, optix_colors.window_title_text_bg_unselected);
    }
    if (widget->child != NULL) optix_RenderStack(widget->child);
    if (window_title_bar->window != NULL) window_title_bar->window->widget.render(window_title_bar->window);
    //optix_SetTextColor(optix_colors.text_fg, optix_colors.text_bg);
}

//utilities




