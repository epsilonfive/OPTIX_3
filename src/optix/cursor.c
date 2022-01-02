#include "cursor.h"

#include <stdbool.h>
#include <tice.h>
#include <debug.h>
#include <string.h>
#include "input.h"
#include "gfx/optix_gfx.h"
#include "gui_control.h"
#include "colors.h"
#include "util.h"
#include "elements/window.h"
#include "elements/button.h"
#include "elements/menu.h"


//initialize
void optix_InitializeCursor(struct optix_cursor *cursor) {
    struct optix_widget *widget = (struct optix_widget *) cursor;
    cursor->widget.transform.x = LCD_WIDTH / 2 -  OPTIX_CURSOR_SPRITE_WIDTH / 2;
    cursor->widget.transform.y = LCD_HEIGHT / 2 - OPTIX_CURSOR_SPRITE_HEIGHT / 2;
    //true x and y
    cursor->true_x = (float) widget->transform.x;
    cursor->true_y = (float) widget->transform.y;
    cursor->widget.transform.width =  OPTIX_CURSOR_SPRITE_WIDTH;
    cursor->widget.transform.height = OPTIX_CURSOR_SPRITE_HEIGHT;
    cursor->widget.child = NULL;
    cursor->widget.state.visible = true;
    cursor->widget.update = optix_UpdateCursor_default;
    cursor->widget.render = optix_RenderCursor_default;
    widget->transform.x = LCD_WIDTH / 2;
    widget->transform.y = LCD_HEIGHT / 2;
    widget->transform.width =  OPTIX_CURSOR_WIDTH;
    widget->transform.height = OPTIX_CURSOR_HEIGHT;
    widget->update = optix_UpdateCursor_default;
    widget->render = optix_RenderCursor_default;
    widget->state.visible = true;
    cursor->current_selection = NULL;
    cursor->direction = OPTIX_CURSOR_NO_DIR;
    cursor->back = gfx_AllocSprite(OPTIX_CURSOR_SPRITE_WIDTH, OPTIX_CURSOR_SPRITE_HEIGHT, malloc);
    cursor->state = OPTIX_CURSOR_NORMAL;
}

//this will also handle the box-based mode
void optix_UpdateCursor_default(struct optix_widget *widget) {
    struct optix_transform *transform = &widget->transform;
    struct optix_cursor *cursor = (struct optix_cursor *) widget;
    //start by updating the last x and y position of the cursor
    cursor->last_x = transform->x;
    cursor->last_y = transform->y;
    if (current_context->settings->cursor_active) {
        bool key_pressed = false;
        float seconds_elapsed = ((float) current_context->data->ticks) / ((float) TIMER_FREQUENCY);
        float last_true_x = cursor->true_x;
        float last_true_y = cursor->true_y;
        current_context->cursor->state = OPTIX_CURSOR_NORMAL;
        if (optix_DefaultKeyIsDown(KEY_UP)) {
            cursor->true_y -= cursor->current_speed;
            key_pressed = true;
        }
        if (optix_DefaultKeyIsDown(KEY_DOWN)) {
            cursor->true_y += cursor->current_speed;
            key_pressed = true;
        }
        if (optix_DefaultKeyIsDown(KEY_LEFT)) {
            cursor->true_x -= cursor->current_speed;
            key_pressed = true;
        }
        if (optix_DefaultKeyIsDown(KEY_RIGHT)) {
            cursor->true_x += cursor->current_speed;
            key_pressed = true;
        }
        if (key_pressed) {
            transform->x = (int) cursor->true_x;
            transform->y = (int) cursor->true_y;
            if (transform->x > LCD_WIDTH - OPTIX_CURSOR_RESIZE_WIDTH) transform->x = LCD_WIDTH - OPTIX_CURSOR_RESIZE_WIDTH;
            if (transform->y > LCD_HEIGHT - OPTIX_CURSOR_RESIZE_HEIGHT) transform->y = LCD_HEIGHT - OPTIX_CURSOR_RESIZE_WIDTH;
            if (transform->x < 0) transform->x = 0;
            if (transform->y < 0) transform->y = 0;
            //handle this as well-we don't want the cursor to be accelerating if it's not moving (e.g. holding opposite arrow keys)
            if (cursor->true_x == last_true_x && cursor->true_y == last_true_y && cursor->current_speed > OPTIX_CURSOR_INITIAL_SPEED) 
                current_context->cursor->current_speed = OPTIX_CURSOR_INITIAL_SPEED;
            //update cursor speed
            cursor->current_speed += OPTIX_CURSOR_ACCELERATION * seconds_elapsed;
            cursor->current_speed = cursor->current_speed > OPTIX_CURSOR_MAX_SPEED ? OPTIX_CURSOR_MAX_SPEED : cursor->current_speed;
        } else current_context->cursor->current_speed = OPTIX_CURSOR_INITIAL_SPEED;
        //handle this as well-the GUI should be frozen if the selected element is a window title bar that is currently being dragged
        if (cursor->current_selection) {
            if (cursor->current_selection->type == OPTIX_WINDOW_TYPE) {
                struct optix_window *window = (struct optix_window *) current_context->cursor->current_selection;
                if (window->resize_active && (optix_DefaultKeyIsDown(KEY_ENTER) != KEY_RELEASED)) cursor->gui_frozen = true;
            } else if (cursor->current_selection->type == OPTIX_WINDOW_TITLE_BAR_TYPE) {
                struct optix_window_title_bar *window_title_bar = (struct optix_window_title_bar *) current_context->cursor->current_selection;
                if (window_title_bar->drag_active && (optix_DefaultKeyIsDown(KEY_ENTER) != KEY_RELEASED)) cursor->gui_frozen = true;
            }
        }
    } else {
        cursor->direction = OPTIX_CURSOR_NO_DIR;
        if (optix_DefaultKeyIsDown(KEY_UP) & KEY_PRESSED)    cursor->direction = OPTIX_CURSOR_UP;
        if (optix_DefaultKeyIsDown(KEY_DOWN) & KEY_PRESSED)  cursor->direction = OPTIX_CURSOR_DOWN;
        if (optix_DefaultKeyIsDown(KEY_LEFT) & KEY_PRESSED)  cursor->direction = OPTIX_CURSOR_LEFT;
        if (optix_DefaultKeyIsDown(KEY_RIGHT) & KEY_PRESSED) cursor->direction = OPTIX_CURSOR_RIGHT;
    }
}  

void optix_RenderCursor_default(struct optix_widget *widget) {
    struct optix_cursor *cursor = (struct optix_cursor *) widget;
    uint8_t draw_location;
    gfx_sprite_t *spr[] = {
        cursor_normal,
        cursor_pointer,
        cursor_move,
        cursor_resize_horizontal,
        cursor_resize_vertical,
        cursor_text,
        cursor_text_upper,
        cursor_text_lower,
        cursor_text_math,
    };
    if (current_context->settings->cursor_active) {
        struct optix_widget *last_element = current_context->stack[optix_GetNumElementsInStack(current_context->stack) - 1];
        //start with this, show a window being moved if applicable
        if (last_element->type == OPTIX_WINDOW_TITLE_BAR_TYPE) {
            struct optix_window_title_bar *window_title_bar = (struct optix_window_title_bar *) last_element;
            if (window_title_bar->window->active) {
                int x_pos = window_title_bar->widget.transform.x;
                int y_pos = window_title_bar->widget.transform.y;
                int x_change = current_context->cursor->widget.transform.x - window_title_bar->drag_start_x;
                int y_change = current_context->cursor->widget.transform.y - window_title_bar->drag_start_y;
                uint8_t draw_location = gfx_GetDraw();
                gfx_SetDrawScreen();
                gfx_SetColor(HIGHLIGHT_COLOR_INDEX);
                if (window_title_bar->drag_active || window_title_bar->window->resize_active) {
                    gfx_BlitBuffer();
                    if (window_title_bar->window->resize_active) {
                        struct optix_window *window = window_title_bar->window;
                        dbg_sprintf(dbgout, "Resizing window...%d\n", y_change);
                        switch (window->resize_border) {
                            case WINDOW_RESIZE_LEFT:
                                window->resize_width = window->widget.transform.width - x_change;
                                window->resize_height = window->widget.transform.height;
                                cursor->state = OPTIX_CURSOR_RESIZE_HORIZ;
                                x_pos += x_change;
                                break;
                            case WINDOW_RESIZE_RIGHT:
                                window->resize_width = window->widget.transform.width + x_change;
                                window->resize_height = window->widget.transform.height;
                                cursor->state = OPTIX_CURSOR_RESIZE_HORIZ;
                                break;
                            case WINDOW_RESIZE_BOTTOM:
                                window->resize_width = window->widget.transform.width;
                                window->resize_height = window->widget.transform.height + y_change;
                                cursor->state = OPTIX_CURSOR_RESIZE_VERT;
                                break;
                            default:
                                break;
                        }
                        gfx_Rectangle(x_pos - 1, y_pos, window->resize_width + 2, window->resize_height + TITLE_BAR_HEIGHT + 1);
                    } else {
                        cursor->state = OPTIX_CURSOR_MOVE;
                        x_pos = window_title_bar->window->widget.transform.x + x_change;
                        y_pos = window_title_bar->window->widget.transform.y + y_change;
                        gfx_Rectangle(x_pos - 1, y_pos - TITLE_BAR_HEIGHT, window_title_bar->widget.transform.width + 2, TITLE_BAR_HEIGHT + window_title_bar->window->widget.transform.height + 1);
                    }
                }
                gfx_SetDraw(draw_location);
            }
        }
        if (!current_context->settings->constant_refresh) optix_RefreshCursorBackground(widget);
        gfx_SetTransparentColor(0);
        gfx_TransparentSprite_NoClip(spr[cursor->state], cursor->widget.transform.x, cursor->widget.transform.y);
        //blit it
        gfx_BlitRectangle(gfx_buffer, widget->transform.x, widget->transform.y, OPTIX_CURSOR_SPRITE_WIDTH, OPTIX_CURSOR_SPRITE_HEIGHT);
        gfx_BlitRectangle(gfx_buffer, cursor->last_x, cursor->last_y, OPTIX_CURSOR_SPRITE_WIDTH, OPTIX_CURSOR_SPRITE_HEIGHT);
    } else if (cursor->current_selection) {
        struct optix_transform *transform = &(cursor->current_selection->transform);
        draw_location = gfx_GetDraw();
        gfx_SetDrawScreen();
        gfx_SetColor(HIGHLIGHT_COLOR_INDEX);
        if (cursor->current_selection->type == OPTIX_WINDOW_TITLE_BAR_TYPE) {
            struct optix_window *window = ((struct optix_window_title_bar *) cursor->current_selection)->window;
            gfx_Rectangle(transform->x - 1, transform->y, transform->width + 2, transform->height + window->widget.transform.height + 1);
        } else if (cursor->current_selection->type == OPTIX_MENU_TYPE) {
            struct optix_menu *menu = (struct optix_menu *) cursor->current_selection;
            struct optix_button temp;
            if (menu->hide_selection_box) {
                int selection = menu->selection != -1 ? menu->selection : menu->last_selection;
                struct optix_transform *temp_transform = &temp.widget.transform;
                optix_SetMenuOptionTransform(selection, &temp, menu);
                gfx_Rectangle(temp_transform->x, temp_transform->y, temp_transform->width, temp_transform->height);
            }
        } else gfx_Rectangle(transform->x, transform->y, transform->width, transform->height);
        gfx_SetDraw(draw_location);
    }
}

//to refresh the cursor background
void optix_RefreshCursorBackground(struct optix_widget *widget) {
    struct optix_cursor *cursor = (struct optix_cursor *) widget;
    //gfx_Sprite(current_context->cursor->back, current_context->cursor->last_x, current_context->cursor->last_y);
    //get the new one
    gfx_GetSprite(cursor->back, cursor->widget.transform.x, cursor->widget.transform.y);
}

void optix_RenderCursorBackground(struct optix_widget *widget) {
    struct optix_cursor *cursor = (struct optix_cursor *) widget;
    if (current_context->settings->cursor_active) gfx_Sprite(cursor->back, cursor->last_x, cursor->last_y);
}

void optix_HandleNearestElement(void) {
    struct optix_cursor *cursor = current_context->cursor;
    if (!current_context->settings->cursor_active) {
        if (cursor->direction != OPTIX_CURSOR_NO_DIR) {
            //this is the element at the top of the OPTIX stack, which should be a window or window title bar
            struct optix_widget *top_element = current_context->stack[optix_GetNumElementsInStack(current_context->stack) - 1];
            struct optix_widget **stack;
            struct optix_widget *nearest_element;
            if (top_element->type == OPTIX_WINDOW_TYPE || top_element->type == OPTIX_WINDOW_TITLE_BAR_TYPE) {
                //if it's a window title bar, we want the window
                struct optix_window *window = top_element->type == OPTIX_WINDOW_TYPE ? (struct optix_window *) top_element : ((struct optix_window_title_bar *) top_element)->window;
                dbg_sprintf(dbgout, "This was true.\n");
                if (window->active && window->widget.child) stack = window->widget.child;
                else stack = current_context->stack;
            } else stack = current_context->stack;
            //find the nearest element based on this information and set the current selection to it
            nearest_element = optix_FindNearestElement(cursor->direction, cursor->current_selection, stack);
            if (nearest_element) optix_SetCurrentSelection(nearest_element);
        }
        if (cursor->direction != OPTIX_CURSOR_NO_DIR) current_context->data->needs_blit = true;
    }
}


//returns a pointer to the closest element found within the array
//use the ternary operator excessively
struct optix_widget *optix_FindNearestElement(uint8_t direction, struct optix_widget *reference, struct optix_widget *stack[]) {
    int i = 0;
    int closest_score = 0;
    struct optix_widget *closest = NULL;
    if (!reference || !stack || direction == OPTIX_CURSOR_NO_DIR) return reference ? reference : NULL;
    //this is going to have to be recursive
    //return the element that is the closest
    //ideally, you'd use this by passing in the children of a window but it would also work in other cases as well
    while (stack[i]) {
        int score = 0;
        //this will make sure that every element is tested, which we may (?) not want
        struct optix_widget *current = stack[i];
        /*if (stack[i]->type == OPTIX_WINDOW_TITLE_BAR_TYPE)
            current = optix_FindNearestElement(direction, reference, ((struct optix_window_title_bar *) stack[i])->window->widget.child);
        else if (stack[i]->state.visible) current = (stack[i]->child) ? optix_FindNearestElement(direction, reference, stack[i]->child) : stack[i];
        else {
            i++;
            continue;
        }*/
        //I only want to have buttons and window title bars be selectable at this moment
        if (current == current_context->cursor->current_selection || !current->state.selectable || !current->state.visible) {
            i++;
            continue;
        }
        switch (direction) {
            case OPTIX_CURSOR_LEFT:
                score = (current->transform.x < reference->transform.x) * (abs(current->transform.x - reference->transform.x) + (OPTIX_BOX_SENSITIVITY * abs(current->transform.y - reference->transform.y)));
                break;
            case OPTIX_CURSOR_RIGHT:
                score = (current->transform.x > reference->transform.x) * (abs(current->transform.x - reference->transform.x) + (OPTIX_BOX_SENSITIVITY * abs(current->transform.y - reference->transform.y)));
                break;
            case OPTIX_CURSOR_UP:
                score = (current->transform.y < reference->transform.y) * (abs(current->transform.y - reference->transform.y) + (OPTIX_BOX_SENSITIVITY * abs(current->transform.x - reference->transform.x)));
                break;
            case OPTIX_CURSOR_DOWN:
                score = (current->transform.y > reference->transform.y) * (abs(current->transform.y - reference->transform.y) + (OPTIX_BOX_SENSITIVITY * abs(current->transform.x - reference->transform.x)));
                break;
        }
        //I finally get to use this
        if ((score && closest_score && score <= closest_score) || (!closest_score && score)) {
            closest = current;
        }
        closest_score = (score && score <= closest_score) || (score && !closest_score) ? score : closest_score;
        i++;
    }
    return closest;
}

//this will update the current selection, or what is currently underneath the cursor
//does this recursively, so it will return the topmost thing in that stack, taking into account children and window contents
struct optix_widget *optix_GetCurrentSelection(struct optix_widget *stack[]) {
    int i = 0;
    struct optix_widget *potential_selection = NULL;
    while (stack && stack[i]) {
        if (!stack[i]->state.selectable || !stack[i]->state.visible) {
            i++;
            continue;
        }
        if (optix_CheckTransformOverlap(&current_context->cursor->widget, stack[i])) {
            //check if it has any children that overlap, and if so check those as well
            if (stack[i]->child) potential_selection = optix_GetCurrentSelection(stack[i]->child);
            if (!potential_selection) potential_selection = stack[i];
        }
        if (stack[i]->type == OPTIX_WINDOW_TITLE_BAR_TYPE) {
            struct optix_window *window = ((struct optix_window_title_bar *) stack[i])->window;
            if (optix_CheckTransformOverlap(&current_context->cursor->widget, (struct optix_widget *) window)) {
                potential_selection = (struct optix_widget *) window;
                if (window->active) potential_selection = optix_GetCurrentSelection(window->widget.child);
            }
        }
        i++;
    }
    //dbg_sprintf(dbgout, "Potential selection: %d Type: %d\n", potential_selection, potential_selection->type);
    return potential_selection;
}

void optix_SetCurrentSelection(struct optix_widget *widget) {
    //we're assuming this is correct
    //firstly, the old current selection should now be set to not selected
    if (current_context->cursor->current_selection) current_context->cursor->current_selection->state.selected = false;
    current_context->cursor->current_selection = widget;
    if (widget) {
        widget->state.selected = true;
        if (!current_context->settings->cursor_active) {
            current_context->cursor->widget.transform.x = widget->transform.x;
            current_context->cursor->widget.transform.y = widget->transform.y;
        }
    }
}
