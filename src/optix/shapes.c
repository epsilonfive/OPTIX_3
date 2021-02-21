#include "shapes.h"

void optix_OutlinedRectangle(uint16_t x, uint8_t y, uint16_t width, uint8_t height, uint8_t fill_color, uint8_t outline_color) {
    gfx_SetColor(fill_color);
    gfx_FillRectangle(x, y, width, height);
    gfx_SetColor(outline_color);
    gfx_Rectangle(x, y, width, height);
}

void optix_X(uint16_t x, uint8_t y, uint8_t side_length, uint8_t color) {
    gfx_SetColor(color);
    gfx_Line(x, y, x + side_length, y + side_length);
    gfx_Line(x, y + side_length, x + side_length, y);
}