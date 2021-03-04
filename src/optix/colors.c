#include "colors.h"

optix_colors_t optix_colors = {
    BG_COLOR_DEFAULT                              ,
    //window colors
    WINDOW_BG_COLOR_DEFAULT                       ,
    WINDOW_TITLE_BAR_COLOR_UNSELECTED_DEFAULT     ,
    WINDOW_TITLE_BAR_COLOR_SELECTED_DEFAULT       ,
    WINDOW_BORDER_COLOR_DEFAULT                   ,
    WINDOW_TITLE_TEXT_FG_COLOR_UNSELECTED_DEFAULT ,
    WINDOW_TITLE_TEXT_BG_COLOR_UNSELECTED_DEFAULT ,
    WINDOW_TITLE_TEXT_FG_COLOR_SELECTED_DEFAULT   ,
    WINDOW_TITLE_TEXT_BG_COLOR_SELECTED_DEFAULT   ,
    //button colors
    BUTTON_BG_COLOR_UNSELECTED_DEFAULT            ,
    BUTTON_BG_COLOR_SELECTED_DEFAULT              ,
    BUTTON_BG_COLOR_PRESSED_DEFAULT               ,
    BUTTON_BORDER_COLOR_DEFAULT                   ,
    BUTTON_TEXT_FG_COLOR_UNSELECTED_DEFAULT       ,
    BUTTON_TEXT_BG_COLOR_UNSELECTED_DEFAULT       ,
    BUTTON_TEXT_FG_COLOR_SELECTED_DEFAULT         ,
    BUTTON_TEXT_BG_COLOR_SELECTED_DEFAULT         ,
    BUTTON_TEXT_FG_COLOR_PRESSED_DEFAULT          ,
    BUTTON_TEXT_BG_COLOR_PRESSED_DEFAULT          ,
    //text colors
    TEXT_FG_COLOR_DEFAULT                         ,
    TEXT_BG_COLOR_DEFAULT                         ,
    //other
    HIGHLIGHT_COLOR_DEFAULT                       ,
    DIVIDER_COLOR_DEFAULT                         ,
};


void optix_InitializeColors(void) {
    /*optix_colors_t *c =            &optix_colors;
    c->bg =                        BG_COLOR_DEFAULT;
    //window     
    c->window_bg =                 WINDOW_BG_COLOR_DEFAULT;
    c->window_title_bar =          WINDOW_TITLE_BAR_COLOR_UNSELECTED_DEFAULT;
    c->window_border =             WINDOW_BORDER_COLOR_DEFAULT;
    c->window_title_text_fg =      WINDOW_TITLE_TEXT_FG_COLOR_UNSELECTED_DEFAULT;
    c->window_title_text_bg =      WINDOW_TITLE_TEXT_BG_COLOR_UNSELECTED_DEFAULT;
    //button     
    c->button_bg_unselected =      BUTTON_BG_COLOR_UNSELECTED_COLOR_DEFAULT;
    c->button_bg_selected =        BUTTON_BG_COLOR_SELECTED_COLOR_DEFAULT;
    c->button_bg_pressed =         BUTTON_BG_COLOR_PRESSED_DEFAULT;
    c->button_border =             BUTTON_BORDER_COLOR_DEFAULT;
    c->button_text_fg_unselected = BUTTON_TEXT_FG_COLOR_UNSELECTED_DEFAULT;
    c->button_text_bg_unselected = BUTTON_BG_COLOR_UNSELECTED_COLOR_DEFAULT;
    c->button_text_fg_selected =   BUTTON_TEXT_FG_COLOR_SELECTED_DEFAULT;
    c->button_text_bg_selected =   BUTTON_BG_COLOR_SELECTED_COLOR_DEFAULT;
    c->button_text_fg_pressed =    BUTTON_TEXT_FG_COLOR_PRESSED_DEFAULT;
    c->button_text_bg_pressed =    BUTTON_TEXT_BG_COLOR_PRESSED_DEFAULT;
    //text
    c->text_fg =                   TEXT_FG_COLOR_DEFAULT;
    c->text_bg =                   TEXT_BG_COLOR_DEFAULT;
    optix_SetTextColor(optix_colors.text_fg, optix_colors.text_bg);*/
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
    gfx_SetTextFGColor(fg);
    gfx_SetTextBGColor(bg);
    gfx_SetTextTransparentColor(bg);
}

