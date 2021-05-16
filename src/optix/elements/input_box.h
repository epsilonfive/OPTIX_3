#ifndef INPUT_BOX_H
#define INPUT_BOX_H

//includes
#include "../globals.h"
#include "../util.h"
#include "../shapes.h"

//defines
#define INPUT_BOX_MODE_UPPER       0
#define INPUT_BOX_MODE_LOWER       1
#define INPUT_BOX_MODE_MATH        2
//the amount of pixels the cursor can be off from a word width and still work
#define INPUT_BOX_CURSOR_TOLERANCE 2

//structs
struct optix_input_box {
    struct optix_widget widget;
    //the string we're editing
    struct optix_text *text;
    //offset (from start)
    int cursor_offset;
    //current mode
    uint8_t mode;
    //whether it has a limit in size
    bool has_max_length;
    int max_length;
};

//functions
void optix_RenderInputBox_default(struct optix_widget *widget);
void optix_UpdateInputBox_default(struct optix_widget *widget);
size_t optix_GetStringIndexByOffset(char *str, int offset);

#endif