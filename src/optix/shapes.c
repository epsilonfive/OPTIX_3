#include "shapes.h"

void optix_OutlinedRectangle(uint16_t x, uint8_t y, uint16_t width, uint8_t height, uint8_t fill_color, uint8_t outline_color) {
    gfx_SetColor(fill_color);
    gfx_FillRectangle(x, y, width, height);
    gfx_SetColor(outline_color);
    gfx_Rectangle(x, y, width, height);
}