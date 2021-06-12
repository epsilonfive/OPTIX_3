#include "cursor.h"

//initialize
void optix_InitializeCursor(struct optix_widget *widget) {
    struct optix_cursor *cursor = (struct optix_cursor *) widget;
    cursor->widget.transform.x = LCD_WIDTH / 2 -  OPTIX_CURSOR_SPRITE_WIDTH / 2;
    cursor->widget.transform.y = LCD_HEIGHT / 2 - OPTIX_CURSOR_SPRITE_HEIGHT / 2;
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
    cursor->back = gfx_MallocSprite(OPTIX_CURSOR_SPRITE_WIDTH, OPTIX_CURSOR_SPRITE_HEIGHT);
}

//this will also handle the box-based mode
void optix_UpdateCursor_default(struct optix_widget *widget) {
    struct optix_transform *transform = &widget->transform;
    struct optix_cursor *cursor = (struct optix_cursor *) widget;
    //start by updating the last x and y position of the cursor
    cursor->last_x = transform->x;
    cursor->last_y = transform->y;
    if (current_context->settings->cursor_active) {
        if (kb_Data[7] & kb_Up)    transform->y -= OPTIX_CURSOR_SPEED;
        if (kb_Data[7] & kb_Down)  transform->y += OPTIX_CURSOR_SPEED;
        if (kb_Data[7] & kb_Left)  transform->x -= OPTIX_CURSOR_SPEED;
        if (kb_Data[7] & kb_Right) transform->x += OPTIX_CURSOR_SPEED;
        if (transform->x > LCD_WIDTH - transform->width) transform->x = LCD_WIDTH - transform->width;
        if (transform->y > LCD_HEIGHT - transform->height) transform->y = LCD_HEIGHT - transform->height;
        if (transform->x < 0) transform->x = 0;
        if (transform->y < 0) transform->y = 0;
    } else if (current_context->data->can_press) {
        //kb_Scan will be called elsewhere
        if (kb_Data[7] & kb_Up)         cursor->direction = OPTIX_CURSOR_UP;
        else if (kb_Data[7] & kb_Down)  cursor->direction = OPTIX_CURSOR_DOWN;
        else if (kb_Data[7] & kb_Left)  cursor->direction = OPTIX_CURSOR_LEFT;
        else if (kb_Data[7] & kb_Right) cursor->direction = OPTIX_CURSOR_RIGHT;
        else cursor->direction = OPTIX_CURSOR_NO_DIR;
        //the box-based GUI mode (implement later)
    } else {
        if (!kb_AnyKey()) current_context->data->can_press = true;
        cursor->direction = OPTIX_CURSOR_NO_DIR;
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
        if (!current_context->settings->constant_refresh) optix_RefreshCursorBackground(widget);
        gfx_SetTransparentColor(255);
        gfx_TransparentSprite_NoClip(spr[cursor->state], cursor->widget.transform.x, cursor->widget.transform.y);
    } else {
        struct optix_transform *transform = &cursor->current_selection->transform;
        draw_location = gfx_GetDraw();
        gfx_SetDrawScreen();
        gfx_SetColor(HIGHLIGHT_COLOR_INDEX);
        if (cursor->current_selection->type == OPTIX_WINDOW_TITLE_BAR_TYPE) {
            struct optix_window *window = ((struct optix_window_title_bar *) cursor->current_selection)->window;
            gfx_Rectangle(transform->x, transform->y, transform->width, transform->height + window->widget.transform.height);
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
        if (current == current_context->cursor->current_selection || !(current->state.selectable)) {
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

void optix_SetCurrentSelection(struct optix_widget *widget) {
    //we're assuming this is correct
    if (!current_context->settings->cursor_active) {
        current_context->cursor->current_selection = widget;
        current_context->cursor->widget.transform.x = widget->transform.x;
        current_context->cursor->widget.transform.y = widget->transform.y;
    }
}
