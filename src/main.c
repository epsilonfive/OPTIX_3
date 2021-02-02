//////////////////////////////////////////////////
// { OPTIX CE } { 3.0.0 }                       //
// Author: epsilon5                             //
// License: DON'T TOUCH!!!!                     //
// Description: a cool shell for CE calculators //
//////////////////////////////////////////////////

/** OPTIX Rewrite Plan
I: Items
   - items will be a struct, having callbacks for updating and rendering them
   - all items will be rendered each loop, but some will be visible and others will be invisible
   - if [enter] or [2nd] is pressed and the item is selected, it will call its callback function for what it should do when clicked
II: Menus
   - menus will just be an array of items
   - it will also contain data about the width and height of the items in question, and update their x and y values as necessary for          
     scrolling
   - it will set which ones are visible/invisible as well, where necessary
   - finally, it will update which item within the menu is currently selected, and make sure that the other ones are not
III: Scroll bars
   - scroll bars will just be another GUI element, that contains a pointer to a menu which it will reflect as well as the generic transform
     structure that the item and menu structures also contain, which has the:
     * x position
     * y position
     * width
     * height
     * color (?)
IV: Text input
   - see the previous sections about callbacks and stuff, it'll need a function to update it when it is visible and active
IV: Windows
   - windows will be yet another struct, but will have arrays of items, menus, scroll bars, and dividers
   - it will update everything's x and y based on its own x and y, meaning it will be able to be moved around the screen
   - also, there will be different ways to left, right, center align things as needed with offset
   - windows will be able to adjust the positions of all of their children to line up with these alignments as specified earlier
*/

#include "main.h"

void test_func(void *args) {
    gfx_SetTextFGColor(255);
    gfx_SetTextBGColor(0);
    gfx_SetTextTransparentColor(0);
    gfx_PrintStringXY("It worked!", 5, 100);
    gfx_Blit(1);
    while (!os_GetCSC());
}



void main(void) {
    gfx_Begin();
    gfx_SetDraw(1);
    //a test transform
    optix_transform_temp.x = 0;
    optix_transform_temp.y = 0;
    optix_transform_temp.width = 40;
    optix_transform_temp.height = 40;
    optix_InitializeGUIState();
    optix_InitializeCursor();
    optix_AddButton(optix_transform_temp, optix_state_temp, optix_UpdateButton_default, optix_RenderButton_default, test_func, NULL);
    dbg_sprintf(dbgout, "Buttons: %d", optix_gui_control.num_buttons);
    while (!(kb_Data[6] & kb_Clear)) {
        kb_Scan();
        optix_UpdateGUI();
        gfx_FillScreen(0);
        optix_RenderGUI();
        gfx_Blit(1);
    }
    gfx_End();
}
