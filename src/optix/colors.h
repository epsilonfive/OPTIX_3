#ifndef COLORS_H
#define COLORS_H

#include "globals.h"

//defines
/*NOTES:
    -text background color is the same as text transparent color
*/

#define BG_COLOR_DEFAULT                              0
//window colors      
#define WINDOW_BG_COLOR_DEFAULT                       0
#define WINDOW_TITLE_BAR_COLOR_UNSELECTED_DEFAULT     255
#define WINDOW_TITLE_BAR_COLOR_SELECTED_DEFAULT       18
#define WINDOW_BORDER_COLOR_DEFAULT                   18
#define WINDOW_TITLE_TEXT_FG_COLOR_UNSELECTED_DEFAULT 255
#define WINDOW_TITLE_TEXT_BG_COLOR_UNSELECTED_DEFAULT 0   
#define WINDOW_TITLE_TEXT_FG_COLOR_SELECTED_DEFAULT   0
#define WINDOW_TITLE_TEXT_BG_COLOR_SELECTED_DEFAULT   255
//button colors
#define BUTTON_BG_COLOR_UNSELECTED_DEFAULT            0
#define BUTTON_BG_COLOR_SELECTED_DEFAULT              255
#define BUTTON_BG_COLOR_PRESSED_DEFAULT               18
#define BUTTON_BORDER_COLOR_DEFAULT                   8
#define BUTTON_TEXT_FG_COLOR_UNSELECTED_DEFAULT       255
#define BUTTON_TEXT_BG_COLOR_UNSELECTED_DEFAULT       0
#define BUTTON_TEXT_FG_COLOR_SELECTED_DEFAULT         0
#define BUTTON_TEXT_BG_COLOR_SELECTED_DEFAULT         255
#define BUTTON_TEXT_FG_COLOR_PRESSED_DEFAULT          255
#define BUTTON_TEXT_BG_COLOR_PRESSED_DEFAULT          0
//text colors
#define TEXT_FG_COLOR_DEFAULT                         255
#define TEXT_BG_COLOR_DEFAULT                         0
/*
4.24
order 4
use to determine rate constant
5 2 iodide 1 for sulfate right but I didn't get that

*/
typedef struct {
    //general
    uint8_t bg;
    //window
    uint8_t window_bg;
    uint8_t window_title_bar_unselected;
    uint8_t window_title_bar_selected;
    uint8_t window_border;
    uint8_t window_title_text_fg_unselected;
    uint8_t window_title_text_bg_unselected;
    uint8_t window_title_text_fg_selected;
    uint8_t window_title_text_bg_selected;
    //button
    uint8_t button_bg_unselected;
    uint8_t button_bg_selected;
    uint8_t button_bg_pressed;
    uint8_t button_border;
    uint8_t button_text_fg_unselected;
    uint8_t button_text_bg_unselected;
    uint8_t button_text_fg_selected;
    uint8_t button_text_bg_selected;
    uint8_t button_text_fg_pressed;
    uint8_t button_text_bg_pressed;
    //text
    uint8_t text_fg;
    uint8_t text_bg;
} optix_colors_t;
extern optix_colors_t optix_colors; 

//functions
void optix_InitializeColors(void);
void optix_WhiteText(void);
void optix_BlackText(void);
void optix_SetTextColor(uint8_t fg, uint8_t bg);


#endif