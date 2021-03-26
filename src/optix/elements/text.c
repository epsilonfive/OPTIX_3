#include "text.h"

//sets a text object's width and height according to the text's width and height
void optix_InitializeTextTransform(struct optix_text *text) {
    text->widget.transform.x = 0;
    text->widget.transform.y = 0;
    if (optix_gui_data.font_valid) {
        text->widget.transform.width = fontlib_GetStringWidth(text->text);
        text->widget.transform.height = fontlib_GetCurrentFontHeight();
    } else {
        text->widget.transform.width = gfx_GetStringWidth(text->text);
        text->widget.transform.height = 8;
    }
    text->widget.centering.x_centering = OPTIX_CENTERING_CENTERED;
    text->widget.centering.y_centering = OPTIX_CENTERING_CENTERED;
    text->widget.centering.x_offset = text->widget.centering.y_offset = 0;
}


void optix_RenderText_default(struct optix_widget *widget) {
    struct optix_text *text = (struct optix_text *) widget;
    if (widget->state.visible && widget->state.needs_redraw) {
        if (optix_gui_data.font_valid) {
            fontlib_SetWindow(widget->transform.x, widget->transform.y, widget->transform.width, widget->transform.height);
            fontlib_SetCursorPosition(widget->transform.x, widget->transform.y);
            fontlib_DrawString(text->text);
        } else gfx_PrintStringXY(text->text, widget->transform.x, widget->transform.y);
    }
}

//sets the default font, which is in an appvar
bool optix_InitializeFont(void) {
    fontlib_font_t *font_pack;
    if (font_pack = fontlib_GetFontByIndex(DEFAULT_FONT_PACK_NAME, 0)) {
        fontlib_SetFont(font_pack, 0);
        fontlib_SetColors(TEXT_FG_COLOR_DEFAULT, TEXT_BG_COLOR_DEFAULT);
        fontlib_SetTransparency(true);
    }
    optix_gui_data.font_valid = (bool) font_pack;
    dbg_sprintf(dbgout, "Font pack %d\n", (bool) font_pack);
    return (bool) font_pack;
}



