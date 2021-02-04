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

//structs
struct optix_gui_stack test_stack[STACK_NUM_ENTRIES];

//functions
void test_func(void *args) {
   //we don't need to do anything with args
   gfx_PrintStringXY("It worked!", 5, 100);
   gfx_Blit(1);
   while (!os_GetCSC());
}



void main(void) {
   //test button
   struct optix_button test_button =     {.transform = {.x = 0, .y = 0, .height = 40, .width = 40},                   //transform
                                         .state = {.selected = false, .visible = true},                               //state
                                         .update = optix_UpdateButton_default, .render = optix_RenderButton_default,  //functions
                                         .click_action = test_func, .click_args = NULL};                              //on click
   struct optix_button test_button2 =    {.transform = {.x = 0, .y = 100, .height = 40, .width = 100},                //transform
                                         .state = {.selected = false, .visible = true},                               //state
                                         .update = optix_UpdateButton_default, .render = optix_RenderButton_default,  //functions
                                         .click_action = test_func, .click_args = NULL};                              //on click
   struct optix_text test_text =         {.transform = {.x = 10, .y = 200, .height = 0, .width = 0},
                                         .state = {.selected = false, .visible = true},
                                         .text = "Test text", .render = optix_RenderText_default};
   struct optix_gui_stack test_stack[] = {{.ptr = &test_button, .type = OPTIX_BUTTON_TYPE},
                                         {.ptr = &test_button2, .type = OPTIX_BUTTON_TYPE},
                                         {.ptr = &test_text,    .type = OPTIX_TEXT_TYPE}};                          //add more as needed
   gfx_Begin();
   gfx_SetDraw(1);
   gfx_SetTextFGColor(255);
   gfx_SetTextBGColor(0);
   gfx_SetTextTransparentColor(0);
   //optix_InitializeGUIState();
   optix_InitializeCursor();
   //optix_AddButton(optix_transform_temp, optix_state_temp, optix_UpdateButton_default, optix_RenderButton_default, test_func, NULL);
   dbg_sprintf(dbgout, "Buttons: %d", optix_gui_control.num_buttons);
   while (!(kb_Data[6] & kb_Clear)) {
       kb_Scan();
       optix_UpdateGUI(test_stack, 3);
       gfx_FillScreen(0);
       optix_RenderGUI(test_stack, 3);
       gfx_Blit(1);
   }
   gfx_End();
}
