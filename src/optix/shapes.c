#include "shapes.h"

void optix_OutlinedRectangle(uint16_t x, uint8_t y, uint16_t width, uint8_t height, uint8_t fill_color, uint8_t outline_color) {
    gfx_SetColor(fill_color);
    gfx_FillRectangle(x, y, width, height);
    gfx_SetColor(outline_color);
    gfx_Rectangle(x, y, width, height);
}

void optix_OutlinedRectangle_WithBevel(uint16_t x, uint8_t y, uint16_t width, uint8_t height, uint8_t fill_color, uint8_t border_bevel_light, uint8_t border_bevel_dark) {
    gfx_SetColor(fill_color);
    gfx_FillRectangle(x, y, width, height);
    gfx_SetColor(border_bevel_dark);
    //bottom edge
    gfx_HorizLine(x + 1, y + height - 1, width - 1);
    //right
    gfx_VertLine(x + width - 1, y + 1, height - 1);
    gfx_SetColor(border_bevel_light);
    //top
    gfx_HorizLine(x, y, width);
    //left
    gfx_VertLine(x, y, height);
    /*gfx_SetColor(fill_color);
    gfx_FillRectangle(x + 1, y + 1, width - 2, height - 2);
    //highlight color
    gfx_SetColor(border_bevel_light);
    //top
    gfx_HorizLine(x + 1, y, width - 2);
    //left
    gfx_VertLine(x, y + 1, height - 2);
    //gfx_SetColor(border_bevel_dark);
    //bottom
    gfx_HorizLine(x + 1, y + height - 1, width - 2);
    //right
    gfx_VertLine(x + width - 1, y + 1, height - 2);*/
}


void optix_X(uint16_t x, uint8_t y, uint8_t side_length, uint8_t color) {
    gfx_SetColor(color);
    gfx_Line(x, y, x + side_length, y + side_length);
    gfx_Line(x, y + side_length, x + side_length, y);
}