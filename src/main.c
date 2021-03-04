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
   struct optix_rectangle test_rectangle = {
      .widget = {
         .transform = {
            .x = 0,
            .y = 0,
            .width = 100,
            .height = 100,
         },
      },
      .filled = true,
      .border_color = 224,
      .fill_color = 255,
   };
   optix_InitializeWidget(&test_rectangle.widget, OPTIX_RECTANGLE_TYPE);
   struct optix_text test_text[] = {
      {.text = "1"},
      {.text = "2"},
      {.text = "3"},
      {.text = "4"},
   };
   struct optix_button test_button[4] = {
      {.widget.child = (struct optix_widget *[]) {&test_text[0].widget, NULL}},
      {.widget.child = (struct optix_widget *[]) {&test_text[1].widget, NULL}},     
      {.widget.child = (struct optix_widget *[]) {&test_text[2].widget, NULL}},
      {.widget.child = (struct optix_widget *[]) {&test_text[3].widget, NULL}},
   };
   for (int i = 0; i < 4; i++) {
      optix_InitializeWidget(&test_text[i].widget, OPTIX_TEXT_TYPE);
      optix_InitializeWidget(&test_button[i].widget, OPTIX_BUTTON_TYPE);
   }
   struct optix_menu test_menu = {
      .widget = {
         .transform = {
            .width = 50,
            .height = 100,
         },
         .child = (struct optix_widget *[]) {
            &test_button[0].widget,
            &test_button[1].widget,
            &test_button[2].widget,
            &test_button[3].widget,
            NULL,
         },
      },
      .resize_info = {
         .resizable = true,
         .x_lock = true,
         .y_lock = true,
         .min_width = 50,
         .min_height = 20,
      },
      .rows = 4,
      .columns = 1,
   };
   optix_InitializeWidget(&test_menu.widget, OPTIX_MENU_TYPE);
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
   struct optix_text button_text = {.text = "_"};
   struct optix_button minimize_button = {
      .widget = {
         .transform = {
            .width = 12,
            .height = 12,
         },
         .child = (struct optix_widget *[]) {&button_text.widget, NULL}
      },
      .click_action = minimize_window,
      .click_args = &test_window,
   };
   optix_InitializeWidget(&button_text.widget, OPTIX_TEXT_TYPE);
   optix_InitializeWidget(&minimize_button.widget, OPTIX_BUTTON_TYPE);
   minimize_button.widget.centering.x_centering = OPTIX_CENTERING_RIGHT;
   struct optix_window_title_bar test_title_bar = {
      .widget = {.child = (struct optix_widget *[]) {&minimize_button.widget, NULL}},
      .window = &test_window,
      .title = "Test",
   };
   optix_InitializeWidget(&test_title_bar.widget, OPTIX_WINDOW_TITLE_BAR_TYPE);
   //finally, align everything
   optix_RecursiveAlign(&test_title_bar.widget);
   struct optix_widget *test_stack[] = {&test_rectangle.widget, &test_title_bar.widget, NULL};   
   optix_InitializeColors();
   //graphics
   gfx_Begin();
   gfx_SetDraw(1);
   do {
      optix_UpdateGUI(&test_stack);
      gfx_ZeroScreen();
      optix_RenderGUI(test_stack);
      gfx_Blit(1);
   } while (!(kb_Data[6] & kb_Clear));
   gfx_End();
}