#ifndef TEXT_H
#define TEXT_H

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <tice.h>
#include "../gui_control.h"

#define DEFAULT_FONT_PACK_NAME     "DRSANS"
#define TEXT_SPACING               10
//the offset that the text is from the edge (and including)
#define TEXT_OFFSET                2
#define FORMAT_CENTERING           '\1'
#define ZERO_WIDTH_SPACE           '\6'
#define MAX_VISIBLE_LINES          LCD_HEIGHT / TEXT_SPACING


struct optix_text {
    struct optix_widget widget;
    //same rules as for regular alignment work here (OPTIX_CENTERING_CENTERED and so on)
    uint8_t alignment;
    int8_t x_offset;
    char *text;
    //for the first line
    int min;
    //could be useful
    int num_lines;
    //whether we should update the offsets
    bool needs_offset_update;
    //whether or not it has a rectangle behind it
    bool background_rectangle;
    //whether we should use a custom text color for this
    bool use_custom_color;
    uint8_t custom_fg_color;
    uint8_t custom_bg_color;
};

//functions
void optix_UpdateText_default(struct optix_widget *widget);
void optix_RenderText_default(struct optix_widget *widget);
void optix_InitializeTextTransform(struct optix_text *text);
bool optix_InitializeFont(void);
size_t optix_GetStringWidthL(char *str, size_t max_chars);
size_t optix_GetStringWidth(char *str);
void optix_GetTextNumLines(struct optix_widget *widget);

#endif