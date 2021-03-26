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
   gfx_SetColor(7);
   gfx_FillRectangle(0, 0, 320, 2);
   gfx_Blit(1);
}

void focus_window(void *args) {
   struct optix_window *window = (struct optix_widget *) args;
   window->needs_focus = true;
}

void minimize_window(void *args) {
   struct optix_window *window = (struct optix_widget *) args;
   window->widget.state.visible = false;
}

void main(void) {
   optix_InitializeFont();
   struct optix_rectangle test_rectangle = {
      .widget = {
         .transform = {
            .x = 0,
            .y = 0,
            .width = LCD_WIDTH,
            .height = LCD_HEIGHT,
         },
      },
      .filled = true,
      .border_color = 0,
      .fill_color = 0,
   };
   optix_InitializeWidget(&test_rectangle.widget, OPTIX_RECTANGLE_TYPE);
   char *test_text[] = {"1", "2", "3", "4", "5", "6", "7", "8"};
   struct optix_menu test_menu = {
      .widget = {
         .transform = {
            .width = 50,
            .height = 50,
         },
      },
      .resize_info = {
         .resizable = true,
         .x_lock = true,
         .y_lock = false,
         .min_width = 25,
         .min_height = 25,
      },
      .rows = 2,
      .columns = 2,
   };
   optix_InitializeMenu(&test_menu, 8, &test_text, NULL);
   test_menu.widget.centering.x_centering = OPTIX_CENTERING_LEFT;
   struct optix_divider test_divider = {
      .alignment = DIVIDER_ALIGNMENT_RIGHT,
      .reference = &test_menu.widget,
      //let's not talk about this alright
      .height_increase = 1,
   };
   optix_InitializeWidget(&test_divider.widget, OPTIX_DIVIDER_TYPE);
   struct optix_divider test_divider2 = {
      .alignment = DIVIDER_ALIGNMENT_BOTTOM,
      .reference = &test_menu.widget,
   };
   optix_InitializeWidget(&test_divider2.widget, OPTIX_DIVIDER_TYPE);
   struct optix_window test_window = {
      .widget = {
         .transform = {
            .x = 100,
            .y = 100,
            .width = 100,
            .height = 100,
         },
         .child = (struct optix_widget *[]) {&test_menu.widget, &test_divider.widget, &test_divider2.widget, NULL},
      },
      .resize_info = {
         .resizable = true,
         .min_width = 50,
         .min_height = 50,
      },
   };
   optix_InitializeWidget(&test_window.widget, OPTIX_WINDOW_TYPE);
   //a button that will minimize the window when pressed
   char *button_action_text[] = {"-", "X"};
   struct optix_menu menu_actions = {
      .widget = {
         .transform = {
            .width = 24,
            .height = 12,
         },
         .centering = {
            .x_centering = OPTIX_CENTERING_RIGHT,
            .y_centering = OPTIX_CENTERING_CENTERED,
         },
      },
      .rows = 1,
      .columns = 2,
   };
   optix_InitializeMenu(&menu_actions, 2, &button_action_text, NULL);
   //optix_AlignMenu(&menu_actions);
   struct optix_window_title_bar test_title_bar = {
      .widget = {.child = (struct optix_widget *[]) {&menu_actions.widget, NULL}},
      .window = &test_window,
   };
   optix_InitializeWidget(&test_title_bar.widget, OPTIX_WINDOW_TITLE_BAR_TYPE);
   //another window
   struct optix_text test_text2 = {.text = "FontlibC is cool"};
   optix_InitializeWidget(&test_text2.widget, OPTIX_TEXT_TYPE);
   test_text2.widget.centering.x_centering = test_text2.widget.centering.y_centering = 0;
   test_text2.widget.centering.x_offset = test_text2.widget.centering.y_offset = 2;
   struct optix_window test_window2 = {
      .widget = {
         .transform = {
            .x = 150,
            .y = 100,
            .width = 100,
            .height = 100,
         },
         .child = (struct optix_widget *[]) {&test_text2.widget, NULL},
      },
      .resize_info = {
         .resizable = true,
         .min_width = 50,
         .min_height = 50,
      },
   };
   optix_InitializeWidget(&test_window2.widget, OPTIX_WINDOW_TYPE);
   //title
   struct optix_text test_title_bar2_text = {.text = "FontlibC"};
   optix_InitializeWidget(&test_title_bar2_text.widget, OPTIX_TEXT_TYPE);
   test_title_bar2_text.widget.centering.x_centering = OPTIX_CENTERING_LEFT;
   test_title_bar2_text.widget.centering.x_offset = 2;
   struct optix_window_title_bar test_title_bar2 = {
      .widget = {.child = (struct optix_widget *[]) {&test_title_bar2_text.widget, NULL}},
      .window = &test_window2,
   };
   optix_InitializeWidget(&test_title_bar2.widget, OPTIX_WINDOW_TITLE_BAR_TYPE);
   //finally, align everything
   optix_RecursiveAlign(&test_title_bar.widget);
   optix_RecursiveAlign(&test_title_bar2.widget);
   struct optix_widget *test_stack[] = {&test_rectangle.widget, &test_title_bar2.widget,  &test_title_bar.widget, NULL};   
   optix_InitializeColors();
   optix_InitializeCursor();
   //graphics
   gfx_Begin();
   gfx_SetDraw(1);
   do {
      optix_UpdateGUI(&test_stack);
      optix_RenderGUI(test_stack);
      gfx_Blit(1);
   } while (!(kb_Data[6] & kb_Clear));
   gfx_End();
}