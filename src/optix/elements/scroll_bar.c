#include "scroll_bar.h"

#include <stdint.h>

#include "../colors.h"
#include "../shapes.h"

#include "menu.h"


//functions
//we probably don't need this actually, unless more functionality needs to be added later on
void optix_UpdateScrollBar_default(struct optix_widget *widget) {
    struct optix_scroll_bar *scroll_bar = (struct optix_scroll_bar *) widget;
    //not great but I think this is good
    //if (scroll_bar->mirror->state.needs_redraw) 
    widget->state.needs_redraw = scroll_bar->mirror->state.needs_redraw;
}

void optix_RenderScrollBar_default(struct optix_widget *widget) {
    struct optix_scroll_bar *scroll_bar = (struct optix_scroll_bar *) widget;
    struct optix_menu *menu = (struct optix_menu *) scroll_bar->mirror;
    //struct optix_text *text = (struct optix_text *) scroll_bar->mirror;
    int num_positions = 0, current_position = 0, displayed_positions = 0;
    uint16_t offset;
    uint16_t size;
    //initialize these things, depending on the type
    //if it's not visible or does not need a redraw, then just return
    if (!(widget->state.visible && widget->state.needs_redraw)) return;
    switch (scroll_bar->mirror->type) {
        case OPTIX_MENU_TYPE:
            num_positions = menu->rows == 1 ? menu->num_options : menu->num_options / menu->columns;
            current_position = menu->min / (menu->rows == 1 ? 1 : menu->columns);
            displayed_positions = menu->rows == 1 ? menu->columns : menu->rows;
            break;
        case OPTIX_TEXT_TYPE:
            //do this later, I'm lazy
            break;
        //if it's not one of those, I think we just want to return
        default:
            return;
    }
    size = displayed_positions * (scroll_bar->direction ? widget->transform.height : widget->transform.width) / num_positions;
    offset = ((scroll_bar->direction ? widget->transform.height : widget->transform.width) - size) * current_position / (num_positions - displayed_positions);
    //this part should be pretty easy, I think
    optix_OutlinedRectangle_WithBevel(widget->transform.x, widget->transform.y, widget->transform.width, widget->transform.height,                                    //transform stuff
    BUTTON_BG_COLOR_UNSELECTED_INDEX, WINDOW_BORDER_BEVEL_DARK_INDEX, WINDOW_BORDER_BEVEL_LIGHT_INDEX);
    //the inner thing, we want to scale this as well
    optix_OutlinedRectangle_WithBevel(widget->transform.x + (scroll_bar->direction ? 0 : offset) + 1, widget->transform.y + (scroll_bar->direction ? offset : 0) + 1, //x, y
    (scroll_bar->direction ? widget->transform.width : size) - 2, (scroll_bar->direction ? size : widget->transform.height) - 2,                                      //width, height
    WINDOW_BG_COLOR_INDEX, WINDOW_BORDER_BEVEL_LIGHT_INDEX, WINDOW_BORDER_BEVEL_DARK_INDEX);
}



