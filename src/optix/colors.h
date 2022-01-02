#ifndef COLORS_H
#define COLORS_H

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <fontlibc.h>
#include <graphx.h>
#include <debug.h>
#include "gui_control.h"

#define BG_COLOR_DEFAULT                              gfx_RGBTo1555(0x00, 0x00, 0x00)
//cursor colors
#define CURSOR_FILL_DEFAULT                           gfx_RGBTo1555(0x20, 0x20, 0x20)
#define CURSOR_OUTLINE_DEFAULT                        gfx_RGBTo1555(0xFF, 0xFF, 0xFF)
//window colors      
#define WINDOW_BG_COLOR_DEFAULT                       gfx_RGBTo1555(0x08, 0x08, 0x08)
#define WINDOW_TITLE_BAR_COLOR_UNSELECTED_DEFAULT     gfx_RGBTo1555(0x08, 0x08, 0x08)
#define WINDOW_TITLE_BAR_COLOR_SELECTED_DEFAULT       gfx_RGBTo1555(0x0D, 0x1D, 0x77)
#define WINDOW_BORDER_COLOR_DEFAULT                   0 //unused
//maybe use these later
#define WINDOW_BORDER_BEVEL_LIGHT_DEFAULT             gfx_RGBTo1555(0x0D, 0x1D, 0x77)
#define WINDOW_BORDER_BEVEL_DARK_DEFAULT              gfx_RGBTo1555(0x78, 0x8A, 0xF1)
#define WINDOW_BORDER_BEVEL_MEDIUM_DEFAULT            0 //unused
//remove the rest of the borders
#define WINDOW_TITLE_TEXT_FG_COLOR_UNSELECTED_DEFAULT gfx_RGBTo1555(0xff, 0xff, 0xff)
#define WINDOW_TITLE_TEXT_BG_COLOR_UNSELECTED_DEFAULT gfx_RGBTo1555(0x00, 0x00, 0x00) 
#define WINDOW_TITLE_TEXT_FG_COLOR_SELECTED_DEFAULT   gfx_RGBTo1555(0xff, 0xff, 0xff)
#define WINDOW_TITLE_TEXT_BG_COLOR_SELECTED_DEFAULT   gfx_RGBTo1555(0x00, 0x00, 0x00)
//button colors
#define BUTTON_BG_COLOR_UNSELECTED_DEFAULT            gfx_RGBTo1555(0x08, 0x08, 0x08)
#define BUTTON_BG_COLOR_SELECTED_DEFAULT              gfx_RGBTo1555(0x08, 0x08, 0x08)
#define BUTTON_BG_COLOR_PRESSED_DEFAULT               gfx_RGBTo1555(0x78, 0x8A, 0xF1)
#define BUTTON_BORDER_COLOR_DEFAULT                   //unused
#define BUTTON_TEXT_FG_COLOR_UNSELECTED_DEFAULT       gfx_RGBTo1555(0xff, 0xff, 0xff)
#define BUTTON_TEXT_BG_COLOR_UNSELECTED_DEFAULT       gfx_RGBTo1555(0x3f, 0x3f, 0x3f)
#define BUTTON_TEXT_FG_COLOR_SELECTED_DEFAULT         gfx_RGBTo1555(0xff, 0xff, 0xff)
#define BUTTON_TEXT_BG_COLOR_SELECTED_DEFAULT         gfx_RGBTo1555(0x3f, 0x3f, 0x3f)
#define BUTTON_TEXT_FG_COLOR_PRESSED_DEFAULT          gfx_RGBTo1555(0xff, 0xff, 0xff)
#define BUTTON_TEXT_BG_COLOR_PRESSED_DEFAULT          gfx_RGBTo1555(0x3f, 0x3f, 0x3f)
//text colors
#define TEXT_FG_COLOR_DEFAULT                         gfx_RGBTo1555(0xff, 0xff, 0xff)
#define TEXT_BG_COLOR_DEFAULT                         gfx_RGBTo1555(0x3f, 0x3f, 0x3f)
//random other stuff
#define HIGHLIGHT_COLOR_DEFAULT                       gfx_RGBTo1555(0xFF, 0xFF, 0xFF)
#define DIVIDER_COLOR_DEFAULT                         gfx_RGBTo1555(0x0D, 0x1D, 0x77)

//indices
#define BG_COLOR_INDEX                                0
//cursor colors
#define CURSOR_FILL_INDEX                             1
#define CURSOR_OUTLINE_INDEX                          2
//window colors         
#define WINDOW_BG_COLOR_INDEX                         3
#define WINDOW_TITLE_BAR_COLOR_UNSELECTED_INDEX       4
#define WINDOW_TITLE_BAR_COLOR_SELECTED_INDEX         5
#define WINDOW_BORDER_COLOR_INDEX                     6
//maybe use these later   
#define WINDOW_BORDER_BEVEL_LIGHT_INDEX               7  
#define WINDOW_BORDER_BEVEL_DARK_INDEX                8
#define WINDOW_BORDER_BEVEL_MEDIUM_INDEX              9
//remove the rest of the borders
#define WINDOW_TITLE_TEXT_FG_COLOR_UNSELECTED_INDEX   10
#define WINDOW_TITLE_TEXT_BG_COLOR_UNSELECTED_INDEX   11
#define WINDOW_TITLE_TEXT_FG_COLOR_SELECTED_INDEX     12
#define WINDOW_TITLE_TEXT_BG_COLOR_SELECTED_INDEX     13
//button colors   
#define BUTTON_BG_COLOR_UNSELECTED_INDEX              14
#define BUTTON_BG_COLOR_SELECTED_INDEX                15
#define BUTTON_BG_COLOR_PRESSED_INDEX                 16
#define BUTTON_BORDER_COLOR_INDEX                     17
#define BUTTON_TEXT_FG_COLOR_UNSELECTED_INDEX         18
#define BUTTON_TEXT_BG_COLOR_UNSELECTED_INDEX         19
#define BUTTON_TEXT_FG_COLOR_SELECTED_INDEX           20
#define BUTTON_TEXT_BG_COLOR_SELECTED_INDEX           21
#define BUTTON_TEXT_FG_COLOR_PRESSED_INDEX            22
#define BUTTON_TEXT_BG_COLOR_PRESSED_INDEX            23
//text colors  
#define TEXT_FG_COLOR_INDEX                           24
#define TEXT_BG_COLOR_INDEX                           25
//random other stuff 
#define HIGHLIGHT_COLOR_INDEX                         26
#define DIVIDER_COLOR_INDEX                           27

struct optix_colors {
    //general
    uint16_t bg;
    //cursor
    uint16_t cursor_fill;
    uint16_t cursor_outline;
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