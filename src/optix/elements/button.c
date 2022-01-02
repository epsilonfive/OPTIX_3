#include "button.h"
//includes
#include <stdbool.h>
#include <stdint.h>
#include <graphx.h>
#include "../gui_control.h"
#include "../loop.h"
#include "../input.h"
#include "../shapes.h"
#include "../cursor.h"
#include "../util.h"
#include "../colors.h"

//functions
//update button
//just change the color to red if pressed and run the click action for now
void optix_UpdateButton_default(struct optix_widget *widget) {
    struct optix_button *button = (struct optix_button *) widget;
    //check if the cursor overlaps with it
    if (!widget->state.visible) return;
    //kb_Scan will be called elsewhere
    if ((current_context->data->key && current_context->data->key == button->alternate_key) || ((optix_DefaultKeyIsDown(KEY_ENTER) & KEY_PRESSED) && widget->state.selected)) {
        dbg_sprintf(dbgout, "Clicking...\n");
        if (button->click_action.click_action) {
            button->click_action.click_action(button->click_action.click_args);
            //so that it doesn't repeat
            optix_SetDefaultKeyState(KEY_ENTER, KEY_HELD);
        }
        widget->state.needs_redraw = true;
    } else if (optix_DefaultKeyIsDown(KEY_ENTER) & KEY_PRESSED) widget->state.needs_redraw = true;
    if (button->selected_save != widget->state.selected) widget->state.needs_redraw = true;
    button->selected_save = widget->state.selected;
}

//render button
void optix_RenderButton_default(struct optix_widget *widget) {
    struct optix_button *button = (struct optix_button *) widget;
    if (widget->state.visible && widget->state.needs_redraw) {
        if (widget->state.selected) {
            if (!button->hide_selection_box) {
                if (button->pressed) {
                    optix_OutlinedRectangle_WithBevel(widget->transform.x, widget->transform.y, widget->transform.width, widget->transform.height, //transform
                    BUTTON_BG_COLOR_PRESSED_INDEX, WINDOW_BORDER_BEVEL_DARK_INDEX, WINDOW_BORDER_BEVEL_LIGHT_INDEX);                               //color
                    optix_SetTextColor(BUTTON_TEXT_FG_COLOR_PRESSED_INDEX, BUTTON_TEXT_BG_COLOR_PRESSED_INDEX);
                } else {
                    optix_OutlinedRectangle_WithBevel(widget->transform.x, widget->transform.y, widget->transform.width, widget->transform.height, //transform
                    BUTTON_BG_COLOR_SELECTED_INDEX, WINDOW_BORDER_BEVEL_LIGHT_INDEX, WINDOW_BORDER_BEVEL_DARK_INDEX);                              //color
                    optix_SetTextColor(BUTTON_TEXT_FG_COLOR_SELECTED_INDEX, BUTTON_TEXT_BG_COLOR_SELECTED_INDEX);
                }
            }
        } else {
            //for partial redraw later on
            if (!button->transparent_background) {
                gfx_SetColor(BUTTON_BG_COLOR_UNSELECTED_INDEX);
                gfx_FillRectangle(widget->transform.x, widget->transform.y, widget->transform.width, widget->transform.height);
            }
            optix_SetTextColor(BUTTON_TEXT_FG_COLOR_UNSELECTED_INDEX, BUTTON_TEXT_BG_COLOR_SELECTED_INDEX);
        }
        //cool, recursion
        if (widget->child) optix_RenderStack(widget->child);
        //set this back
        optix_SetTextColor(TEXT_FG_COLOR_INDEX, TEXT_BG_COLOR_INDEX);
    }
}