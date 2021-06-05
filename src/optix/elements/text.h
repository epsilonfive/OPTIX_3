#ifndef TEXT_H
#define TEXT_H

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <graphx.h>
#include <fontlibc.h>
#include "../gui_control.h"
#include "../cursor.h"

#define DEFAULT_FONT_PACK_NAME     "DRSANS"
#define TEXT_SPACING               10
#define FORMAT_CENTERING           '\1'
#define ZERO_WIDTH_SPACE           '\6'


struct optix_text {
    struct optix_widget widget;
    //same rules as for regular alignment work here (OPTIX_CENTERING_CENTERED and so on)
    uint8_t alignment;
    int8_t x_offset;
    char *text;
    //for the first line
    int min;
    //offsets, for word wrapping (maybe preallocated)
    char **offsets;
    //could be useful
    int num_lines;
    //whether we should update the offsets
    bool needs_offset_update;
    //whether or not it has a rectangle behind it
    bool background_rectangle;
};

//functions
void optix_UpdateText_default(struct optix_widget *widget);
void optix_RenderText_default(struct optix_widget *widget);
void optix_InitializeTextTransform(struct optix_text *text);
bool optix_InitializeFont(void);
size_t optix_GetStringWidthL(char *str, size_t max_chars);
void optix_WrapText(struct optix_widget *widget);

#endif