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

//functions
void test_func(void *args) {
   //we don't need to do anything with args
   /*gfx_FillScreen(8);
   gfx_Blit(1);*/
}

void main(void) {
   //test button
   struct optix_button test_button =     {.click_action = test_func, .click_args = NULL};                                        //on click
   struct optix_button test_button2 =    {.click_action = test_func, .click_args = NULL};                                        //on click
   struct optix_text test_text =         {.text = "Test text"};
   struct optix_widget *test_stack[] =   {&test_button.widget, &test_button2.widget, NULL};                         //add more as needed
   struct optix_widget *test_button2_children[] = {&test_text.widget, NULL};
   //try this
   //configure the text
   optix_InitializeWidget(&test_text.widget, OPTIX_TEXT_TYPE);
   optix_InitializeTextTransform(&test_text);
   optix_SetObjectCallbacks(&test_text.widget, optix_RenderText_default, NULL);
   //configure the first button
   optix_InitializeWidget(&test_button.widget, OPTIX_BUTTON_TYPE);
   optix_SetObjectTransform(&test_button.widget, 0, 0, 40, 40);
   optix_SetObjectCallbacks(&test_button.widget, optix_RenderButton_default, optix_UpdateButton_default);
   //configure the second button
   optix_InitializeWidget(&test_button2.widget, OPTIX_BUTTON_TYPE);
   optix_SetObjectTransform(&test_button2.widget, 0, 100, 100, 20);
   optix_SetObjectCallbacks(&test_button2.widget, optix_RenderButton_default, optix_UpdateButton_default);
   //make that text a child of it
   optix_AlignTransformToTransform(&test_text.widget.transform, &test_button2.widget.transform, OPTIX_CENTERING_CENTERED, OPTIX_CENTERING_CENTERED);
   optix_SetObjectChildren(&test_button2.widget, test_button2_children);
   //do the actual program
   gfx_Begin();
   gfx_SetDraw(1);
   gfx_SetTextFGColor(224);
   gfx_SetTextBGColor(0);
   gfx_SetTextTransparentColor(0);
   optix_InitializeCursor();
   while (!(kb_Data[6] & kb_Clear)) {
       kb_Scan();
       optix_UpdateGUI(test_stack);
       gfx_FillScreen(0);
       optix_RenderGUI(test_stack);
       gfx_Blit(1);
   }
   gfx_End();
}
