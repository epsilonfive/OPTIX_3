#ifndef SHAPES_H
#define SHAPES_H

#include <stdint.h>
#include <graphx.h>

void optix_OutlinedRectangle(uint16_t x, uint8_t y, uint16_t width, uint8_t height, uint8_t fill_color, uint8_t outline_color);\
void optix_OutlinedRectangle_WithBevel(uint16_t x, uint8_t y, uint16_t width, uint8_t height, uint8_t fill_color, uint8_t border_bevel_light, uint8_t border_bevel_dark);
void optix_X(uint16_t x, uint8_t y, uint8_t side_length, uint8_t color);


#endif