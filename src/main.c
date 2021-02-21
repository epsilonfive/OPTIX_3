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
   gfx_FillRectangle(0, 0, 2, 2);
   gfx_Blit(1);
}

void main(void) {
   //first window, with a little bar on the bottom and some text
   struct optix_text test_message = {.text = "Hello, world!"};
   optix_InitializeWidget(&test_message.widget, OPTIX_TEXT_TYPE);
   optix_InitializeTextTransform(&test_message);
   struct optix_text test_text[] = {{.text = "1"}, {.text = "2"}, {.text = "3"}, {.text = "4"}};
   //test_button
   struct optix_button test_button[] = {
      {.widget.child = (struct optix_widget *[]) {&test_text[0].widget, NULL}},
      {.widget.child = (struct optix_widget *[]) {&test_text[1].widget, NULL}},
      {.widget.child = (struct optix_widget *[]) {&test_text[2].widget, NULL}},
      {.widget.child = (struct optix_widget *[]) {&test_text[3].widget, NULL}},
   };
   //initialize the buttons and their text
   for (int i = 0; i < 4; i++) {
      optix_InitializeWidget(&test_text[i].widget, OPTIX_TEXT_TYPE);
      optix_InitializeTextTransform(&test_text[i]);
      //initialize those buttons
      //transform doesn't matter because the menu will set it
      optix_InitializeWidget(&test_button[i].widget, OPTIX_BUTTON_TYPE);
   }
   struct optix_menu test_menu = {
      .widget = {
         .type = OPTIX_MENU_TYPE,
         .transform = {
            .width = 100,
            .height = 20,
         },
         .child = (struct optix_widget *[]) {
            &test_button[0].widget,
            &test_button[1].widget,
            &test_button[2].widget,
            &test_button[3].widget,
            NULL,
         },
         .update = optix_UpdateMenu_default,
         .render = optix_RenderMenu_default,
      },
      .rows = 1,
      .columns = 4,
   };
   optix_AlignMenu(&test_menu);
   struct optix_window test_window = {
      .widget = {
         .type = OPTIX_WINDOW_TYPE,
         .transform = {
            .x = 100,
            .y = 100,
            .width = 100,
            .height = 100,
         },
         .child = (struct optix_widget *[]) {&test_menu.widget, &test_message.widget, NULL},
         .update = optix_UpdateWindow_default,
         .render = optix_RenderWindow_default,
      },
   };
   struct optix_window_title_bar test_title_bar = {
      .widget = {
         .type = OPTIX_WINDOW_TITLE_BAR_TYPE,
         .transform = {
            .x = test_window.widget.transform.x,
            .y = test_window.widget.transform.y - TITLE_BAR_HEIGHT,
            .width = test_window.widget.transform.width,
            .height = TITLE_BAR_HEIGHT,
         },
         .child = NULL,
         .update = optix_UpdateWindowTitleBar_default,
         .render = optix_RenderWindowTitleBar_default,
      },
      .window = &test_window.widget,
   };
   optix_AlignTransformToTransform(&test_menu.widget, &test_window.widget, OPTIX_CENTERING_CENTERED, OPTIX_CENTERING_BOTTOM);
   optix_AlignTransformToTransform(&test_message.widget, &test_window.widget, OPTIX_CENTERING_CENTERED, OPTIX_CENTERING_CENTERED);
   //the start button
   struct optix_text test_text2 = {.text = "START"};
   optix_InitializeWidget(&test_text2.widget, OPTIX_TEXT_TYPE);
   optix_InitializeTextTransform(&test_text2);
   struct optix_button test_button2 = {
      .widget = {
         .transform = {
            .x = 0,
            .y = 228,
            .width = 70,
            .height = 12,
         },
         .child = (struct optix_widget *[]) {&test_text2.widget, NULL},
      },
   };
   optix_InitializeWidget(&test_button2.widget, OPTIX_BUTTON_TYPE);
   optix_AlignTransformToTransform(&test_text2.widget, &test_button2.widget, OPTIX_CENTERING_CENTERED, OPTIX_CENTERING_CENTERED);
   //a sprite for testing
   struct optix_sprite test_sprite = {
      .widget = {
         .transform = {
            .x = 0,
            .y = 0,
         },
      },
      .spr = cursor_normal,
      .x_scale = 2,
      .y_scale = 2,
      .transparent = false,
   };
   optix_InitializeWidget(&test_sprite.widget, OPTIX_SPRITE_TYPE);
   //another window, but only text in it
   struct optix_text test_message2 = {.text = "Test text"};
   optix_InitializeWidget(&test_message2.widget, OPTIX_TEXT_TYPE);
   optix_InitializeTextTransform(&test_message2);
   struct optix_window test_window2 = {
      .widget = {
         .type = OPTIX_WINDOW_TYPE,
         .transform = {
            .x = 10,
            .y = 150,
            .width = 200,
            .height = 50,
         },
         .child = (struct optix_widget *[]) {&test_message2.widget, NULL},
         .update = optix_UpdateWindow_default,
         .render = optix_RenderWindow_default,
      },
   };
   optix_AlignTransformToTransform(&test_message2.widget, &test_window2.widget, OPTIX_CENTERING_CENTERED, OPTIX_CENTERING_CENTERED);
   struct optix_window_title_bar test_title_bar2 = {
      .widget = {
         .type = OPTIX_WINDOW_TITLE_BAR_TYPE,
         .transform = {
            .x = test_window2.widget.transform.x,
            .y = test_window2.widget.transform.y - TITLE_BAR_HEIGHT,
            .width = test_window2.widget.transform.width,
            .height = TITLE_BAR_HEIGHT,
         },
         .child = NULL,
         .update = optix_UpdateWindowTitleBar_default,
         .render = optix_RenderWindowTitleBar_default,
      },
      .window = &test_window2.widget,
   };
   //another window, but only text in it
   struct optix_text test_message3 = {.text = "Test 2"};
   optix_InitializeWidget(&test_message3.widget, OPTIX_TEXT_TYPE);
   optix_InitializeTextTransform(&test_message3);
   struct optix_window test_window3 = {
      .widget = {
         .type = OPTIX_WINDOW_TYPE,
         .transform = {
            .x = 10,
            .y = 190,
            .width = 50,
            .height = 50,
         },
         .child = (struct optix_widget *[]) {&test_message3.widget, NULL},
         .update = optix_UpdateWindow_default,
         .render = optix_RenderWindow_default,
      },
   };
   optix_AlignTransformToTransform(&test_message3.widget, &test_window3.widget, OPTIX_CENTERING_CENTERED, OPTIX_CENTERING_CENTERED);
   struct optix_window_title_bar test_title_bar3 = {
      .widget = {
         .type = OPTIX_WINDOW_TITLE_BAR_TYPE,
         .transform = {
            .x = test_window3.widget.transform.x,
            .y = test_window3.widget.transform.y - TITLE_BAR_HEIGHT,
            .width = test_window3.widget.transform.width,
            .height = TITLE_BAR_HEIGHT,
         },
         .child = NULL,
         .update = optix_UpdateWindowTitleBar_default,
         .render = optix_RenderWindowTitleBar_default,
      },
      .window = &test_window3.widget,
   };
   //we need a null at the end
   struct optix_widget *test_stack[] = {&test_sprite.widget, &test_button2.widget, &test_title_bar.widget, &test_title_bar2.widget, &test_title_bar3.widget, NULL};
   optix_InitializeColors();
   //graphics
   gfx_Begin();
   gfx_SetDraw(1);
   do {
      optix_UpdateGUI(&test_stack);
      gfx_FillScreen(0);
      gfx_SetColor(255);
      gfx_HorizLine(0, 227, 320);
      gfx_VertLine(70, 228, 20);
      optix_RenderGUI(test_stack);
      gfx_Blit(1);
   } while (!(kb_Data[6] & kb_Clear));
   gfx_End();
}