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
   struct optix_text test_text = {
      .text = "1",
      .x_centering = OPTIX_CENTERING_CENTERED,
      .y_centering = OPTIX_CENTERING_CENTERED,
   };
   struct optix_text test_text2 = {
      .text = "2",
      .x_centering = OPTIX_CENTERING_CENTERED,
      .y_centering = OPTIX_CENTERING_CENTERED,
   };
   //test button
   struct optix_widget *test_button_children[] = {&test_text.widget, NULL};
   struct optix_button test_button = {
      .widget = {
         .type = OPTIX_BUTTON_TYPE,
         .transform = {
            .x = 0,
            .y = 0,
            .width = 100,
            .height = 20,
         },
         .state = {
            .visible = true,
            .selected = false,
         },
         .child = test_button_children,
         .update = optix_UpdateButton_default,
         .render = optix_RenderButton_default,
      },
      .click_action = test_func,
      .click_args = NULL,
   };
   //test button 2
   struct optix_widget *test_button2_children[] = {&test_text2.widget, NULL};
   struct optix_button test_button2 = {
      .widget = {
         .type = OPTIX_BUTTON_TYPE,
         .transform = {
            .x = 0,
            .y = 100,
            .width = 120,
            .height = 40,
         },
         .state = {
            .visible = true,
            .selected = false,
         },
         .child = test_button2_children,
         .update = optix_UpdateButton_default,
         .render = optix_RenderButton_default,
      },
      .click_action = test_func,
      .click_args = NULL,
   };
   //menu
   struct optix_widget *test_menu_children[] = {&test_button.widget, &test_button2.widget, NULL};
   struct optix_menu test_menu = {
      .widget = {
         .type = OPTIX_MENU_TYPE,
         .transform = {
            .x = 0,
            .y = 0,
            .width = 100,
            .height = 20,
         },
         .render = optix_RenderMenu_default,
         .update = optix_UpdateMenu_default,
         .child = test_menu_children,
      },
      .rows = 1,
      .columns = 2,
   };
   //the window itself
   struct optix_widget *test_window_children[] = {&test_menu.widget, NULL};
   struct optix_window test_window = {
      .widget = {
         .type = OPTIX_WINDOW_TYPE,
         .transform = {
            .x = 10,
            .y = 10,
            .width = 200,
            .height = 100,
         },
         .render = optix_RenderWindow_default,
         .update = optix_UpdateWindow_default,
         .child = test_window_children,
      },
      .title = "Test window",
   };
   struct optix_widget *test_stack[] =   {&test_window.widget, NULL};                                      //add more as needed
   //try this
   //configure the text
   optix_InitializeWidget(&test_text.widget, OPTIX_TEXT_TYPE);
   optix_InitializeTextTransform(&test_text);
   optix_SetObjectCallbacks(&test_text.widget, optix_RenderText_default, NULL);
   optix_InitializeWidget(&test_text2.widget, OPTIX_TEXT_TYPE);
   optix_InitializeTextTransform(&test_text2);
   optix_SetObjectCallbacks(&test_text2.widget, optix_RenderText_default, NULL);
   //make that text a child of it
   //see if this works
   dbg_sprintf(dbgout, "x : %d, y : %d\n", test_text.x_centering, test_text.y_centering);
   dbg_sprintf(dbgout, "x : %d, y : %d\n", test_button.widget.transform.x, test_button.widget.transform.y);
   //do the actual program
   //do this too
   optix_AlignMenu(&test_menu);
   optix_AlignTransformToTransform(&test_menu.widget, &test_window.widget, OPTIX_CENTERING_CENTERED, OPTIX_CENTERING_CENTERED);
   srand(rtc_Time());
   gfx_Begin();
   gfx_SetDraw(1);
   gfx_SetTextFGColor(224);
   gfx_SetTextBGColor(0);
   gfx_SetTextTransparentColor(0);
   optix_InitializeCursor();
   timer_Control = TIMER1_ENABLE | TIMER1_32K | TIMER1_UP;
   optix_SetPosition(&test_window.widget, 10, 100);
   do {
      optix_UpdateGUI(test_stack);
      gfx_FillScreen(0);
      optix_RenderGUI(test_stack);
      gfx_Blit(1);
      timer_1_Counter = 0;
   } while (!(kb_Data[6] & kb_Clear));
   gfx_End();
}
