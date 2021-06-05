#include "colors.h"

void optix_InitializeColors(struct optix_colors *colors) {
    struct optix_colors default_colors = {
        .bg =                              BG_COLOR_DEFAULT,
        .border_bevel_dark =               WINDOW_BORDER_BEVEL_DARK_DEFAULT,
        .border_bevel_light =              WINDOW_BORDER_BEVEL_LIGHT_DEFAULT,
        .border_bevel_medium =             WINDOW_BORDER_BEVEL_MEDIUM_DEFAULT,
        //window colors
        .window_bg =                       WINDOW_BG_COLOR_DEFAULT,
        .window_title_bar_unselected =     WINDOW_TITLE_BAR_COLOR_UNSELECTED_DEFAULT,
        .window_title_bar_selected =       WINDOW_TITLE_BAR_COLOR_SELECTED_DEFAULT,
        //.window_border =                 WINDOW_BORDER_COLOR_DEFAULT,
        .window_title_text_fg_unselected = WINDOW_TITLE_TEXT_FG_COLOR_UNSELECTED_DEFAULT,
        .window_title_text_bg_unselected = WINDOW_TITLE_TEXT_BG_COLOR_UNSELECTED_DEFAULT,
        .window_title_text_fg_selected =   WINDOW_TITLE_TEXT_FG_COLOR_SELECTED_DEFAULT,
        .window_title_text_bg_selected =   WINDOW_TITLE_TEXT_BG_COLOR_SELECTED_DEFAULT,
        //button colors
        .button_bg_unselected =            BUTTON_BG_COLOR_UNSELECTED_DEFAULT,
        .button_bg_selected =              BUTTON_BG_COLOR_SELECTED_DEFAULT,
        .button_bg_pressed =               BUTTON_BG_COLOR_PRESSED_DEFAULT,
        //.button_border =                 BUTTON_BORDER_COLOR_DEFAULT,
        .button_text_fg_unselected =       BUTTON_TEXT_FG_COLOR_UNSELECTED_DEFAULT,
        .button_text_bg_unselected =       BUTTON_TEXT_BG_COLOR_UNSELECTED_DEFAULT,
        .button_text_fg_selected =         BUTTON_TEXT_FG_COLOR_SELECTED_DEFAULT,
        .button_text_bg_selected =         BUTTON_TEXT_BG_COLOR_SELECTED_DEFAULT,
        .button_text_fg_pressed =          BUTTON_TEXT_FG_COLOR_PRESSED_DEFAULT,
        .button_text_bg_pressed =          BUTTON_TEXT_BG_COLOR_PRESSED_DEFAULT,
        //text colors
        .text_fg =                         TEXT_FG_COLOR_DEFAULT,
        .text_bg =                         TEXT_BG_COLOR_DEFAULT,
        //other
        .highlight =                       HIGHLIGHT_COLOR_DEFAULT,
        .divider =                         DIVIDER_COLOR_DEFAULT,
    };
    dbg_sprintf(dbgout, "Size: %d\n", sizeof(struct optix_colors));
    memcpy(colors, &default_colors, sizeof(struct optix_colors));
    memcpy(gfx_palette, colors, sizeof(struct optix_colors));
}

void optix_WhiteText(void) {
    gfx_SetTextFGColor(255);
    gfx_SetTextBGColor(0);
    gfx_SetTextTransparentColor(0);
}

void optix_BlackText(void) {
    gfx_SetTextFGColor(0);
    gfx_SetTextBGColor(255);
    gfx_SetTextTransparentColor(0);
}

//fg will be the foreground color, bg will be both the background and transparent colors
void optix_SetTextColor(uint8_t fg, uint8_t bg) {
    if (current_context->data->font_valid) fontlib_SetColors(fg, bg);
    else {
        gfx_SetTextFGColor(fg);
        gfx_SetTextBGColor(bg);
        gfx_SetTextTransparentColor(bg);
    }
}

