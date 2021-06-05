#ifndef COLORS_H
#define COLORS_H

#include "globals.h"

//defines
/*NOTES:
    -text background color is the same as text transparent color
*/

#define BG_COLOR_DEFAULT                              gfx_RGBTo1555(0x00, 0x00, 0x00)
//window colors      
#define WINDOW_BG_COLOR_DEFAULT                       gfx_RGBTo1555(0x12, 0x12, 0x12)
#define WINDOW_TITLE_BAR_COLOR_UNSELECTED_DEFAULT     gfx_RGBTo1555(0x12, 0x12, 0x12)
#define WINDOW_TITLE_BAR_COLOR_SELECTED_DEFAULT       gfx_RGBTo1555(0x12, 0x12, 0x12)
#define WINDOW_BORDER_COLOR_DEFAULT                   0 //unused
//maybe use these later
#define WINDOW_BORDER_BEVEL_LIGHT_DEFAULT             gfx_RGBTo1555(0x0D, 0x1D, 0x77)
#define WINDOW_BORDER_BEVEL_DARK_DEFAULT              gfx_RGBTo1555(0x78, 0x8A, 0xF1)
#define WINDOW_BORDER_BEVEL_MEDIUM_DEFAULT            0 //unused
//remove the rest of the borders
#define WINDOW_TITLE_TEXT_FG_COLOR_UNSELECTED_DEFAULT gfx_RGBTo1555(0xC0, 0xC0, 0xC0)
#define WINDOW_TITLE_TEXT_BG_COLOR_UNSELECTED_DEFAULT gfx_RGBTo1555(0x12, 0x12, 0x12) 
#define WINDOW_TITLE_TEXT_FG_COLOR_SELECTED_DEFAULT   gfx_RGBTo1555(0xFF, 0xFF, 0xFF)
#define WINDOW_TITLE_TEXT_BG_COLOR_SELECTED_DEFAULT   gfx_RGBTo1555(0x12, 0x12, 0x12)
//button colors
#define BUTTON_BG_COLOR_UNSELECTED_DEFAULT            gfx_RGBTo1555(0x12, 0x12, 0x12)
#define BUTTON_BG_COLOR_SELECTED_DEFAULT              gfx_RGBTo1555(0x34, 0x34, 0x34)
#define BUTTON_BG_COLOR_PRESSED_DEFAULT               gfx_RGBTo1555(0x12, 0x12, 0x12)
#define BUTTON_BORDER_COLOR_DEFAULT                   //unused
#define BUTTON_TEXT_FG_COLOR_UNSELECTED_DEFAULT       gfx_RGBTo1555(0xFF, 0xFF, 0xFF)
#define BUTTON_TEXT_BG_COLOR_UNSELECTED_DEFAULT       gfx_RGBTo1555(0x12, 0x12, 0x12)
#define BUTTON_TEXT_FG_COLOR_SELECTED_DEFAULT         gfx_RGBTo1555(0xFF, 0xFF, 0xFF)
#define BUTTON_TEXT_BG_COLOR_SELECTED_DEFAULT         gfx_RGBTo1555(0x12, 0x12, 0x12)
#define BUTTON_TEXT_FG_COLOR_PRESSED_DEFAULT          gfx_RGBTo1555(0xFF, 0xFF, 0xFF)
#define BUTTON_TEXT_BG_COLOR_PRESSED_DEFAULT          gfx_RGBTo1555(0x12, 0x12, 0x12)
//text colors
#define TEXT_FG_COLOR_DEFAULT                         gfx_RGBTo1555(0xFF, 0xFF, 0xFF)
#define TEXT_BG_COLOR_DEFAULT                         gfx_RGBTo1555(0x12, 0x12, 0x12)
//random other stuff
#define HIGHLIGHT_COLOR_DEFAULT                       gfx_RGBTo1555(0xA7, 0xB3, 0xF3)
#define DIVIDER_COLOR_DEFAULT                         gfx_RGBTo1555(0x34, 0x34, 0x34)

//indices
#define BG_COLOR_INDEX                                0
//window colors         
#define WINDOW_BG_COLOR_INDEX                         1
#define WINDOW_TITLE_BAR_COLOR_UNSELECTED_INDEX       2
#define WINDOW_TITLE_BAR_COLOR_SELECTED_INDEX         3
#define WINDOW_BORDER_COLOR_INDEX                     4
//maybe use these later   
#define WINDOW_BORDER_BEVEL_LIGHT_INDEX               5  
#define WINDOW_BORDER_BEVEL_DARK_INDEX                6
#define WINDOW_BORDER_BEVEL_MEDIUM_INDEX              7
//remove the rest of the borders
#define WINDOW_TITLE_TEXT_FG_COLOR_UNSELECTED_INDEX   8
#define WINDOW_TITLE_TEXT_BG_COLOR_UNSELECTED_INDEX   9
#define WINDOW_TITLE_TEXT_FG_COLOR_SELECTED_INDEX     10
#define WINDOW_TITLE_TEXT_BG_COLOR_SELECTED_INDEX     11
//button colors   
#define BUTTON_BG_COLOR_UNSELECTED_INDEX              12
#define BUTTON_BG_COLOR_SELECTED_INDEX                13
#define BUTTON_BG_COLOR_PRESSED_INDEX                 14
#define BUTTON_BORDER_COLOR_INDEX                     15
#define BUTTON_TEXT_FG_COLOR_UNSELECTED_INDEX         16
#define BUTTON_TEXT_BG_COLOR_UNSELECTED_INDEX         17
#define BUTTON_TEXT_FG_COLOR_SELECTED_INDEX           18
#define BUTTON_TEXT_BG_COLOR_SELECTED_INDEX           19
#define BUTTON_TEXT_FG_COLOR_PRESSED_INDEX            20
#define BUTTON_TEXT_BG_COLOR_PRESSED_INDEX            21
//text colors  
#define TEXT_FG_COLOR_INDEX                           22
#define TEXT_BG_COLOR_INDEX                           23
//random other stuff  
#define HIGHLIGHT_COLOR_INDEX                         24
#define DIVIDER_COLOR_INDEX                           25

struct optix_colors {
    //general
    uint16_t bg;
    //window
    uint16_t window_bg;
    uint16_t window_title_bar_unselected;
    uint16_t window_title_bar_selected;
    uint16_t window_border;
    //borders
    uint16_t border_bevel_dark;
    uint16_t border_bevel_light;
    uint16_t border_bevel_medium;
    //uint8_t window_border;
    uint16_t window_title_text_fg_unselected;
    uint16_t window_title_text_bg_unselected;
    uint16_t window_title_text_fg_selected;
    uint16_t window_title_text_bg_selected;
    //button
    uint16_t button_bg_unselected;
    uint16_t button_bg_selected;
    uint16_t button_bg_pressed;
    uint16_t button_border;
    uint16_t button_text_fg_unselected;
    uint16_t button_text_bg_unselected;
    uint16_t button_text_fg_selected;
    uint16_t button_text_bg_selected;
    uint16_t button_text_fg_pressed;
    uint16_t button_text_bg_pressed;
    //text
    uint16_t text_fg;
    uint16_t text_bg;
    //other
    uint16_t highlight;
    uint16_t divider;
};

//functions
void optix_InitializeColors(struct optix_colors *colors);
void optix_WhiteText(void);
void optix_BlackText(void);
void optix_SetTextColor(uint8_t fg, uint8_t bg);


#endif