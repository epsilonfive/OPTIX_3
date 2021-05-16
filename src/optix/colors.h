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
#define WINDOW_TITLE_BAR_COLOR_UNSELECTED_DEFAULT     0
#define WINDOW_TITLE_BAR_COLOR_SELECTED_DEFAULT       0
#define WINDOW_BORDER_COLOR_DEFAULT                   255
//maybe use these later
#define WINDOW_BORDER_BEVEL_LIGHT_DEFAULT             18
#define WINDOW_BORDER_BEVEL_DARK_DEFAULT              8
#define WINDOW_BORDER_BEVEL_MEDIUM_DEFAULT            0
//remove the rest of the borders
#define WINDOW_TITLE_TEXT_FG_COLOR_UNSELECTED_DEFAULT 255
#define WINDOW_TITLE_TEXT_BG_COLOR_UNSELECTED_DEFAULT 0   
#define WINDOW_TITLE_TEXT_FG_COLOR_SELECTED_DEFAULT   18
#define WINDOW_TITLE_TEXT_BG_COLOR_SELECTED_DEFAULT   0
//button colors
#define BUTTON_BG_COLOR_UNSELECTED_DEFAULT            0
#define BUTTON_BG_COLOR_SELECTED_DEFAULT              255
#define BUTTON_BG_COLOR_PRESSED_DEFAULT               181
#define BUTTON_BORDER_COLOR_DEFAULT                   8
#define BUTTON_TEXT_FG_COLOR_UNSELECTED_DEFAULT       255
#define BUTTON_TEXT_BG_COLOR_UNSELECTED_DEFAULT       0
#define BUTTON_TEXT_FG_COLOR_SELECTED_DEFAULT         0
#define BUTTON_TEXT_BG_COLOR_SELECTED_DEFAULT         255
#define BUTTON_TEXT_FG_COLOR_PRESSED_DEFAULT          18
#define BUTTON_TEXT_BG_COLOR_PRESSED_DEFAULT          0
//text colors
#define TEXT_FG_COLOR_DEFAULT                         255
#define TEXT_BG_COLOR_DEFAULT                         0
//random other stuff
#define HIGHLIGHT_COLOR_DEFAULT                       158
#define DIVIDER_COLOR_DEFAULT                         181

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

typedef struct {
    //general
    uint8_t bg;
    uint8_t border_bevel_dark;
    uint8_t border_bevel_light;
    uint8_t border_bevel_medium;
    //window
    uint8_t window_bg;
    uint8_t window_title_bar_unselected;
    uint8_t window_title_bar_selected;
    //uint8_t window_border;
    uint8_t window_title_text_fg_unselected;
    uint8_t window_title_text_bg_unselected;
    uint8_t window_title_text_fg_selected;
    uint8_t window_title_text_bg_selected;
    //button
    uint8_t button_bg_unselected;
    uint8_t button_bg_selected;
    uint8_t button_bg_pressed;
    //uint8_t button_border;
    uint8_t button_text_fg_unselected;
    uint8_t button_text_bg_unselected;
    uint8_t button_text_fg_selected;
    uint8_t button_text_bg_selected;
    uint8_t button_text_fg_pressed;
    uint8_t button_text_bg_pressed;
    //text
    uint8_t text_fg;
    uint8_t text_bg;
    //other
    uint8_t highlight;
    uint8_t divider;
} optix_colors_t;
extern optix_colors_t optix_colors; 

//functions
void optix_InitializeColors(void);
void optix_WhiteText(void);
void optix_BlackText(void);
void optix_SetTextColor(uint8_t fg, uint8_t bg);


#endif