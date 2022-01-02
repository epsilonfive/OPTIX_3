#include "check_box.h"
//includes
#include "../colors.h"
#include "../shapes.h"

void optix_CheckBoxValueXOR(void *widget) {
    struct optix_check_box *check_box = (struct optix_check_box *) widget;
    *(check_box->value) = !(*(check_box->value));
}

//we do need a custom render function here, unfortunately
void optix_RenderCheckBox_default(struct optix_widget *widget) {
    struct optix_check_box *check_box = (struct optix_check_box *) widget;
    if (widget->state.visible && widget->state.needs_redraw) {
        //outer circle thing
        optix_OutlinedRectangle_WithBevel(widget->transform.x, widget->transform.y, widget->transform.width, widget->transform.height, //transform
        BUTTON_BG_COLOR_UNSELECTED_INDEX, WINDOW_BORDER_BEVEL_DARK_INDEX, WINDOW_BORDER_BEVEL_LIGHT_INDEX);                            //color
        if (*check_box->value || check_box->button.pressed) {
            //just put a little triangle in there;
            gfx_SetColor(check_box->button.pressed ? HIGHLIGHT_COLOR_INDEX : BUTTON_BG_COLOR_PRESSED_INDEX);
            gfx_FillRectangle(widget->transform.x + 2, widget->transform.y + 2, widget->transform.width - 4, widget->transform.height - 4);
        }
    }
}