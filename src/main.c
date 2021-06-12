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

void toggle_cursor(void *args) {
   //current_context->settings.cursor_active = !current_context->cursor_active;
}

void main(void) {
   struct optix_cursor cursor;
   struct optix_colors colors;
   struct optix_settings settings;
   struct optix_gui_data data;
   //initialize it
   //graphics
   gfx_Begin();
   gfx_SetDraw(1);
   optix_InitializeColors(&colors);
   optix_InitializeCursor(&cursor);
   optix_InitializeSettings(&settings);
   optix_InitializeData(&data);
   //the final context (we'll add the stack last)
   struct optix_context context = {
      .settings = &settings,
      .cursor = &cursor,
      .data = &data,
      .colors = &colors,
   };
   optix_SetContext(&context);
   optix_InitializeFont();
   //elements
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
   char *test_text[] = {"Programs", "Appvars", "Files", "Search", "Settings", "Refresh", "About", "Exit", "More", "Options", "Please", "Work"};
   struct optix_menu test_menu = {
      .widget = {
         .transform = {
            .width = 50,
            .height = 48,
         },
         .centering = {.x_centering = OPTIX_CENTERING_LEFT, .y_centering = OPTIX_CENTERING_TOP}
      },
      .resize_info = {
         .resizable = true,
         .x_lock = true,
         .y_lock = true,
         .min_width = 25,
         .min_height = 25,
      },
      .text_centering = {.y_centering = OPTIX_CENTERING_CENTERED, .x_centering = OPTIX_CENTERING_CENTERED},
      .rows = 4,
      .columns = 1,
      .text = test_text,
      .spr = NULL,
   };
   optix_InitializeWidget(&test_menu.widget, OPTIX_MENU_TYPE);
   char *test_text_b[] = {"01", "02", "03", "04", "05", "06", "07", "08"};
   struct optix_menu test_menu_b = {
      .widget = {
         .transform = {
            .width = 50,
            .height = 24,
         },
      },
      .resize_info = {
         .resizable = true,
         .x_lock = true,
         .y_lock = true,
         .min_width = 25,
         .min_height = 25,
      },
      .text_centering = {.y_centering = OPTIX_CENTERING_CENTERED, .x_centering = OPTIX_CENTERING_CENTERED},
      .rows = 2,
      .columns = 2,
      .text = test_text_b,
      .spr = NULL,
   };
   optix_InitializeWidget(&test_menu_b.widget, OPTIX_MENU_TYPE);
   test_menu_b.widget.centering.x_centering = OPTIX_CENTERING_RIGHT;
   test_menu_b.widget.centering.y_centering = OPTIX_CENTERING_TOP;
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
   struct optix_scroll_bar test_scroll_bar = {
      .widget = {.transform = {
         .width = 8, .height = 48},
         .centering = {
            .x_centering = OPTIX_CENTERING_LEFT, .y_centering = OPTIX_CENTERING_TOP,
            .x_offset = test_menu.widget.transform.width, .y_offset = 0,
         }
      },
      .direction = SCROLL_BAR_VERT,
      .mirror = &test_menu.widget,
   };
   optix_InitializeWidget(&test_scroll_bar.widget, OPTIX_SCROLL_BAR_TYPE);
   struct optix_window test_window = {
      .widget = {
         .transform = {
            .x = 10,
            .y = 100,
            .width = 200,
            .height = 100,
         },
         .child = (struct optix_widget *[]) {&test_menu.widget, &test_menu_b.widget, &test_scroll_bar.widget, NULL},
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
      .text = button_action_text,
      .spr = NULL,
   };
   //optix_InitializeMenu(&menu_actions, 2, &button_action_text, NULL);
   //optix_AlignMenu(&menu_actions);
   struct optix_window_title_bar test_title_bar = {
      .widget = {.child = NULL},
      .window = &test_window,
   };
   optix_InitializeWidget(&test_title_bar.widget, OPTIX_WINDOW_TITLE_BAR_TYPE);
   //another window
   char temp[100] = "NO";
   char *temp_b = "Here is some test text to see if the word wrapping routine works as it should. Hopefully it does. Does this work? Does it now? I'm just going to keep adding more text and hope it doesn't implode.";
   struct optix_text test_text2 = {
      .widget = {.transform = {.width = 80, .height = 50}},
      .text = temp_b,
      .alignment = OPTIX_CENTERING_CENTERED,
      .x_offset = 0,
      .min = 1,
      .needs_offset_update = true,
   };
   optix_InitializeWidget(&test_text2.widget, OPTIX_TEXT_TYPE);
   //test_text2.widget.update(&test_text2.widget);
   test_text2.widget.centering.x_centering = test_text2.widget.centering.y_centering = OPTIX_CENTERING_CENTERED;
   struct optix_input_box test_input_box = {
      .widget = {
         .transform = {.width = 100, .height = 12},
         .child = NULL,
         .centering = {.x_centering = OPTIX_CENTERING_CENTERED, .y_centering = OPTIX_CENTERING_TOP},
      },
      .cursor_offset = 0,
      .has_max_length = false,
      .mode = 0,
      .text = &test_text2,
   };
   optix_InitializeWidget(&test_input_box.widget, OPTIX_INPUT_BOX_TYPE);
   struct optix_window test_window2 = {
      .widget = {
         .transform = {
            .x = 1,
            .y = 40,
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
   //optix_InitializeWidget(&test_title_bar2_text.widget, OPTIX_TEXT_TYPE);
   test_title_bar2_text.widget.centering.x_centering = OPTIX_CENTERING_LEFT;
   test_title_bar2_text.widget.centering.x_offset = 2;
   struct optix_window_title_bar test_title_bar2 = {
      //.widget = {.child = (struct optix_widget *[]) {&test_title_bar2_text.widget, NULL}},
      .widget = {.child = NULL},
      .window = &test_window2,
   };
   optix_InitializeWidget(&test_title_bar2.widget, OPTIX_WINDOW_TITLE_BAR_TYPE);
   //button
   bool test_bool = false;
   struct optix_check_box test_check_box = {
      .button = {
         .widget = {
            .transform = {
               .x = 0,
               .y = 0,
               .width = 10,
               .height = 10,
            },
            .child = NULL,
         },
      },
      .value = &test_bool,
   };
   optix_InitializeWidget(&test_check_box.button.widget, OPTIX_CHECK_BOX_TYPE);
   int test_int = 0;
   struct optix_slider test_slider = {
      .widget = {
         .transform = {
            .x = 0,
            .y = 30,
            .width = 50,
            .height = 8,
         },
      },
      .min = 0,
      .max = 10,
      .value = &test_int,
      .disp_current_value = false,
   };
   optix_InitializeWidget(&test_slider.widget, OPTIX_SLIDER_TYPE);
   //finally, align everything
   optix_RecursiveAlign(&test_title_bar.widget);
   optix_RecursiveAlign(&test_title_bar2.widget);

   struct optix_widget *widget;
   optix_CopyElement(&widget, &test_title_bar2.widget);
   optix_SetPosition(&test_window.widget, 50, 30);
   optix_SetPosition(&test_window2.widget, 120, 60);
   current_context->data->gui_needs_full_redraw = true;
   dbg_sprintf(dbgout, "X: %d, Y: %d\n", widget->transform.x, widget->transform.y);
   dbg_sprintf(dbgout, "We finished?!\n");
   struct optix_widget *test_stack[] = {&test_rectangle.widget, &test_check_box.button.widget, &test_slider.widget, widget, &test_title_bar2.widget, &test_title_bar.widget, NULL};   
   optix_SetCurrentSelection(&test_check_box.button.widget);
   //add it to the context
   context.stack = &test_stack;
   do {
      dbg_sprintf(dbgout, "Updating...\n");
      optix_UpdateGUI();
      dbg_sprintf(dbgout, "Rendering...\n");
      optix_RenderGUI();
      gfx_Blit(1);
   } while (!(kb_Data[6] & kb_Clear));
   gfx_End();
}