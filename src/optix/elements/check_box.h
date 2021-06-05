#ifndef CHECK_BOX_H
#define CHECK_BOX_H

#include "button.h"
#include <debug.h>

//a check box is pretty much a button, right?
//but where it's clicked, it sets a value off or on
//so all we need to do is set the button clicked function to one that changes the value of the bool
struct optix_check_box {
    struct optix_button button;
    //it'll change this value when clicked
    bool *value;
};

//functions
void optix_CheckBoxValueXOR(void *widget);
void optix_RenderCheckBox_default(struct optix_widget *widget);

#endif