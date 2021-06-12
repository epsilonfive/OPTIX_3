#include "slider.h"

//functions
void optix_UpdateSlider_default(struct optix_widget *widget) {
    struct optix_slider *slider = (struct optix_slider *) widget;
    //dbg_sprintf(dbgout, "Updating slider...\n");
    if (widget->state.visible && optix_CheckTransformOverlap(&current_context->cursor->widget, widget)) {
        //too easy
        bool left_pressed = current_context->settings->cursor_active ? (kb_Data[6] & kb_Sub) : (kb_Data[7] & kb_Left);
        bool right_pressed = current_context->settings->cursor_active ? (kb_Data[6] & kb_Add) : (kb_Data[7] & kb_Right);
        if (left_pressed || right_pressed) {
            widget->state.needs_redraw = true;
            current_context->data->can_press = false;
        }
        if (left_pressed) *(slider->value) -= 1;
        if (right_pressed) *(slider->value) += 1;
        //handle out of bounds
        if (*slider->value > slider->max) *slider->value = slider->max;
        if (*slider->value < slider->min) *slider->value = slider->min;
        dbg_sprintf(dbgout, "Current value: %d\n", *slider->value);
    }
    //dbg_sprintf(dbgout, "Finished.\n");
}

void optix_RenderSlider_default(struct optix_widget *widget) {
    struct optix_slider *slider = (struct optix_slider *) widget;
    if (widget->state.visible && widget->state.needs_redraw) {
        uint16_t width = slider->disp_current_value ? widget->transform.width - slider->current_value_box_size - 1 : widget->transform.width;
        float filled = (float) (*slider->value - slider->min) / (slider->max - slider->min);
        //display the outer box first
        optix_OutlinedRectangle_WithBevel(widget->transform.x, widget->transform.y, width, widget->transform.height,                   //transform
        BUTTON_BG_COLOR_UNSELECTED_INDEX, WINDOW_BORDER_BEVEL_DARK_INDEX, WINDOW_BORDER_BEVEL_LIGHT_INDEX);                            //color
        //the inner part
        gfx_SetColor(BUTTON_BG_COLOR_PRESSED_INDEX);
        gfx_FillRectangle(widget->transform.x + 2, widget->transform.y + 2, (width - 4) * filled, widget->transform.height - 4);
        //then the text part, do that later
    }
}