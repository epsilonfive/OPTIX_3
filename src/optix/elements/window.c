#include "window.h"

//returns true if window size has changed
void optix_UpdateWindow_default(struct optix_widget *widget) {
    struct optix_window *window = (struct optix_window *) widget;
    if (widget->state.visible) {
        if (widget->state.selected && widget->child) optix_UpdateStack(widget->child);
        if (kb_Data[6] & kb_Enter || kb_Data[1] & kb_2nd) {
            //rescale it if necessary
            //if the cursor didn't move just continue
            if (widget->state.selected && optix_CheckTransformOverlap(&optix_cursor.widget, widget) && 
            (optix_cursor.last_x != optix_cursor.widget.transform.x || optix_cursor.last_y != optix_cursor.widget.transform.y)) {
                int center_x = widget->transform.x + widget->transform.width / 2;
                int center_y = widget->transform.y + widget->transform.height / 2;
                int x_size_change = 0, y_size_change = 0;
                int x_shift = 0, y_shift = 0;
                //if we're colliding with the left or right edges of the window
                //left edge
                if (abs(optix_cursor.widget.transform.x - widget->transform.x) < OPTIX_CURSOR_WIDTH + 2) {
                    if (widget->transform.x >= 1) {
                        x_shift = optix_cursor.widget.transform.x - optix_cursor.last_x;
                        x_size_change = -x_shift;
                        optix_cursor.state = OPTIX_CURSOR_RESIZE_HORIZ;
                    }
                } else if (abs(optix_cursor.widget.transform.x - (widget->transform.x + widget->transform.width)) < OPTIX_CURSOR_WIDTH + 2) {
                    x_size_change = optix_cursor.widget.transform.x - optix_cursor.last_x;
                    optix_cursor.state = OPTIX_CURSOR_RESIZE_HORIZ;
                } else if (abs(optix_cursor.widget.transform.y - (widget->transform.y + widget->transform.height)) < OPTIX_CURSOR_HEIGHT + 2) {
                    y_size_change = optix_cursor.widget.transform.y - optix_cursor.last_y;
                    optix_cursor.state = OPTIX_CURSOR_RESIZE_VERT;
                }
                //only if something actually happened
                if (x_size_change || y_size_change || x_shift || y_shift) {
                    optix_gui_data.gui_needs_full_redraw = true;
                    optix_SetPosition(widget, widget->transform.x += x_shift, widget->transform.y += y_shift);
                    optix_ResizeWindow(window, widget->transform.width += x_size_change, widget->transform.height += y_size_change);
                }
            } else if (optix_CheckTransformOverlap(&optix_cursor.widget, widget)) {
                if (widget->state.selected && widget->child) widget->state.needs_redraw = true;
                widget->state.selected = true;
                return;
            } else widget->state.selected = false;
        }
    } else widget->state.selected = false;
}

void optix_RenderWindow_default(struct optix_widget *widget) {
    struct optix_window *window = (struct optix_window *) widget;
    int title_bar_side_padding = 2;
    int element_size = 10;
    if (widget->state.visible) {
        if (widget->state.needs_redraw)
            optix_OutlinedRectangle(widget->transform.x - 1, widget->transform.y - 1, widget->transform.width + 2, widget->transform.height + 2, optix_colors.window_bg, optix_colors.window_border);
        //render everything in it
        if (widget->child) optix_RenderStack(widget->child);
    }
}

void optix_UpdateWindowTitleBar_default(struct optix_widget *widget) {
    struct optix_window_title_bar *window_title_bar = (struct optix_window_title_bar *) widget;
    bool window_selected = window_title_bar->window->widget.state.selected;
    bool needs_redraw;
    widget->state.visible = window_title_bar->window->widget.state.visible;
    if (widget->state.visible) {
        if (widget->state.selected && widget->child) optix_UpdateStack(widget->child);
        //windows are children of the title bar
        //make it so you can move the windows around while holding the selection key and moving the mouse
        //this is for the moving
        if (optix_CheckTransformOverlap(&optix_cursor.widget, widget)) {
            if (kb_Data[6] & kb_Enter || kb_Data[1] & kb_2nd) {
                if (window_selected && (optix_cursor.last_x != optix_cursor.widget.transform.x || optix_cursor.last_y != optix_cursor.widget.transform.y)) {
                    int x_pos = widget->transform.x + (optix_cursor.widget.transform.x - optix_cursor.last_x);
                    int y_pos = widget->transform.y + (optix_cursor.widget.transform.y - optix_cursor.last_y);
                    int new_width = widget->transform.width;
                    int new_height = widget->transform.height;
                    //snap left
                    if (x_pos < 1) {
                        x_pos = 1;
                        //resize the window to cover the left half of the screen if possible
                        if (window_title_bar->window->resize_info.resizable) {
                            y_pos = WINDOW_SNAP_MIN_Y;
                            new_width = LCD_WIDTH / 2 - 2;
                            new_height = WINDOW_SNAP_HEIGHT - widget->transform.height - 2;
                        }
                    }
                    //snap right
                    if (x_pos + window_title_bar->window->widget.transform.width >  319) {
                        x_pos = 320 - LCD_WIDTH / 2;
                        if (window_title_bar->window->resize_info.resizable) {
                            y_pos = WINDOW_SNAP_MIN_Y;
                            new_width = LCD_WIDTH / 2 - 2;
                            new_height = WINDOW_SNAP_HEIGHT - widget->transform.height - 2;
                        }
                    }
                    if (y_pos < 0) {
                        y_pos = WINDOW_SNAP_MIN_Y;
                        //make it be in the top left corner
                        if (x_pos == 1) {
                            new_width = LCD_WIDTH / 2 - 2;
                            new_height = (WINDOW_SNAP_HEIGHT - widget->transform.height - 2) / 2;
                        } else {
                            new_width = LCD_WIDTH - 2;
                            new_height = WINDOW_SNAP_HEIGHT - widget->transform.height - 2;
                        }
                        x_pos = 1;
                        //otherwise make it fullscreen
                    }
                    if (x_pos + window_title_bar->window->widget.transform.width >  319) x_pos = 319 - window_title_bar->window->widget.transform.width;
                    if (y_pos + window_title_bar->window->widget.transform.height > 239) y_pos = 239 - window_title_bar->window->widget.transform.height;
                    //if the size is new resize it
                    if (new_width != widget->transform.width || new_height != widget->transform.height) {
                        //this could cause some issues apparently
                        //make a preview rectangle thing, like in Windows
                        gfx_SetDraw(0);
                        gfx_SetColor(optix_colors.highlight);
                        gfx_Rectangle(x_pos - 1, y_pos, new_width + 2, new_height + widget->transform.height + 1);
                        gfx_Rectangle(x_pos, y_pos + 1, new_width, new_height + widget->transform.height - 1);
                        gfx_SetDraw(1);
                        while (kb_AnyKey()) kb_Scan();
                        optix_ResizeWindow(window_title_bar->window, new_width, new_height);
                    }
                    optix_SetPosition(widget, x_pos, y_pos);
                    optix_SetPosition(window_title_bar->window, x_pos, y_pos + widget->transform.height);
                    optix_gui_data.gui_needs_full_redraw = true;
                } else {
                    //we may need a redraws
                    if (!window_title_bar->window->widget.state.selected) window_title_bar->window->widget.state.needs_redraw = widget->state.needs_redraw = true;
                    window_title_bar->window->widget.state.selected = true;
                }
            }
            //change the cursor icon
            if (optix_cursor.state == OPTIX_CURSOR_NORMAL && window_selected) optix_cursor.state = OPTIX_CURSOR_MOVE;
        }
        //only update it if we need to
        window_title_bar->window->widget.update((struct optix_widget *) window_title_bar->window);
        widget->state.needs_redraw = window_title_bar->window->widget.state.needs_redraw;
        if (window_title_bar->window->widget.transform.x != widget->transform.x || window_title_bar->window->widget.transform.width != widget->transform.width)
            optix_RefreshWindowTitleBarTransform(window_title_bar);

        //kind of hacky but oh well
        if (window_title_bar->window->widget.state.selected != widget->state.selected) widget->state.needs_redraw = true;
        widget->state.selected = window_title_bar->window->widget.state.selected;
    } else widget->state.visible = false;
}

void optix_RenderWindowTitleBar_default(struct optix_widget *widget) {
    struct optix_window_title_bar *window_title_bar = (struct optix_window_title_bar *) widget;
    //those settextcolors are in there in case there's a title, which should change color on select
    if (widget->state.visible) {
        if (widget->state.needs_redraw) {
            if (window_title_bar->window->widget.state.selected) {
                optix_OutlinedRectangle(widget->transform.x - 1, widget->transform.y, widget->transform.width + 2, widget->transform.height, //transform
                optix_colors.window_title_bar_selected, optix_colors.window_border);                                                         //color
                optix_SetTextColor(optix_colors.window_title_text_fg_selected, optix_colors.window_title_text_bg_selected);
            } else {
                optix_OutlinedRectangle(widget->transform.x - 1, widget->transform.y, widget->transform.width + 2, widget->transform.height, //transform
                optix_colors.window_title_bar_unselected, optix_colors.window_border);                                                       //color
                optix_SetTextColor(optix_colors.window_title_text_fg_unselected, optix_colors.window_title_text_bg_unselected);
            }
        }
        if (widget->child) optix_RenderStack(widget->child);
        optix_SetTextColor(optix_colors.text_fg, optix_colors.text_bg);
        if (window_title_bar->window) {
            window_title_bar->window->widget.render(window_title_bar->window);
            window_title_bar->window->widget.state.needs_redraw = false;
        }
    }
}

//utilities
/*About:
    - resizes a window to a new size
    - this will probably need to be rewritten later, because the first iteration is probably going to be awful
    - at the moment, only menus will be able to have dynamic sizes, and besides that, it'll just adjust the sizes if they're larger than the window

*/
void optix_ResizeWindow(struct optix_widget *widget, uint16_t width, uint8_t height) {
    struct optix_window *window = (struct optix_window *) widget;
    int i = 0;
    if (window->resize_info.resizable) {
        if (width < window->resize_info.min_width) width = window->resize_info.min_width;
        if (height < window->resize_info.min_height) height = window->resize_info.min_height;
        widget->transform.width = width;
        widget->transform.height = height;
        if (widget->child) {
            while (widget->child[i]) {
                struct optix_widget *child = widget->child[i];
                //resize it to a new sizef
                if (child->type == OPTIX_MENU_TYPE) {
                    struct optix_menu *menu = (struct optix_menu *) child;
                    if (!menu->resize_info.x_lock) {
                        menu->columns = width / menu->resize_info.min_width;
                        menu->widget.transform.width = width;
                    }
                    if (!menu->resize_info.y_lock) {
                        menu->rows = height / menu->resize_info.min_height;
                        menu->widget.transform.height = height;
                    }
                } else {
                    child->transform.width = width;
                    child->transform.height = height;
                }
                i++;
            }
        }
        optix_RecursiveAlign(widget);
        widget->state.needs_redraw = true;
    }
}


//just refreshes the transform of a window title bar, to be consistent with its window's width
void optix_RefreshWindowTitleBarTransform(struct optix_window_title_bar *window_title_bar) {
    struct optix_transform *transform = &window_title_bar->widget.transform;
    transform->x =      window_title_bar->window->widget.transform.x;
    transform->y =      window_title_bar->window->widget.transform.y - TITLE_BAR_HEIGHT;
    transform->width =  window_title_bar->window->widget.transform.width;
    transform->height = TITLE_BAR_HEIGHT;
    optix_RecursiveAlign(&window_title_bar->widget);
}

