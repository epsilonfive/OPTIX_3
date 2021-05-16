#include "cursor.h"

struct optix_cursor_t optix_cursor = {
    .widget = {
        .transform = {
            .x = LCD_WIDTH / 2 -  OPTIX_CURSOR_SPRITE_WIDTH / 2,
            .y = LCD_HEIGHT / 2 - OPTIX_CURSOR_SPRITE_HEIGHT / 2,
            .width =  OPTIX_CURSOR_SPRITE_WIDTH,
            .height = OPTIX_CURSOR_SPRITE_HEIGHT,
        },
        .child = NULL,
        .state = {.visible = true},
        .update = optix_UpdateCursor_default,
        .render = optix_RenderCursor_default,
    },
};

//initialize
void optix_InitializeCursor(void) {
    struct optix_widget *widget = &optix_cursor.widget;
    widget->transform.x = LCD_WIDTH / 2;
    widget->transform.y = LCD_HEIGHT / 2;
    widget->transform.width =  OPTIX_CURSOR_WIDTH;
    widget->transform.height = OPTIX_CURSOR_HEIGHT;
    widget->update = optix_UpdateCursor_default;
    widget->render = optix_RenderCursor_default;
    widget->state.visible = true;
    optix_cursor.current_selection = NULL;
    optix_cursor.direction = OPTIX_CURSOR_NO_DIR;
    optix_settings.cursor_active = true;
    optix_cursor.back = gfx_MallocSprite(OPTIX_CURSOR_SPRITE_WIDTH, OPTIX_CURSOR_SPRITE_HEIGHT);
}

//this will also handle the box-based mode
void optix_UpdateCursor_default(void) {
    struct optix_transform *transform = &optix_cursor.widget.transform;
    //start by updating the last x and y position of the cursor
    optix_cursor.last_x = transform->x;
    optix_cursor.last_y = transform->y;
    if (optix_settings.cursor_active) {
        if (kb_Data[7] & kb_Up)    transform->y -= OPTIX_CURSOR_SPEED;
        if (kb_Data[7] & kb_Down)  transform->y += OPTIX_CURSOR_SPEED;
        if (kb_Data[7] & kb_Left)  transform->x -= OPTIX_CURSOR_SPEED;
        if (kb_Data[7] & kb_Right) transform->x += OPTIX_CURSOR_SPEED;
        if (transform->x > LCD_WIDTH - transform->width) transform->x = LCD_WIDTH - transform->width;
        if (transform->y > LCD_HEIGHT - transform->height) transform->y = LCD_HEIGHT - transform->height;
        if (transform->x < 0) transform->x = 0;
        if (transform->y < 0) transform->y = 0;
    } else if (optix_gui_data.can_press) {
        //kb_Scan will be called elsewhere
        if (kb_Data[7] & kb_Up)         optix_cursor.direction = OPTIX_CURSOR_UP;
        else if (kb_Data[7] & kb_Down)  optix_cursor.direction = OPTIX_CURSOR_DOWN;
        else if (kb_Data[7] & kb_Left)  optix_cursor.direction = OPTIX_CURSOR_LEFT;
        else if (kb_Data[7] & kb_Right) optix_cursor.direction = OPTIX_CURSOR_RIGHT;
        else optix_cursor.direction = OPTIX_CURSOR_NO_DIR;
        //the box-based GUI mode (implement later)
    } else if (!kb_AnyKey()) {
        optix_gui_data.can_press = true;
        optix_cursor.direction = OPTIX_CURSOR_NO_DIR;
    } else optix_cursor.direction = OPTIX_CURSOR_NO_DIR;
}  

void optix_RenderCursor_default(void) {
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
    if (true || optix_settings.cursor_active) {
        if (!optix_settings.constant_refresh) optix_RefreshCursorBackground();
        gfx_SetTransparentColor(255);
        gfx_TransparentSprite_NoClip(spr[optix_cursor.state], optix_cursor.widget.transform.x, optix_cursor.widget.transform.y);
    }
}

//to refresh the cursor background
void optix_RefreshCursorBackground(void) {
    //gfx_Sprite(optix_cursor.back, optix_cursor.last_x, optix_cursor.last_y);
    //get the new one
    gfx_GetSprite(optix_cursor.back, optix_cursor.widget.transform.x, optix_cursor.widget.transform.y);
}

void optix_RenderCursorBackground(void) {
    if (true || optix_settings.cursor_active) gfx_Sprite(optix_cursor.back, optix_cursor.last_x, optix_cursor.last_y);
}


//returns a pointer to the closest element found within the array
//use the ternary operator excessively
struct optix_widget *optix_FindNearestElement(uint8_t direction, struct optix_widget *reference, struct optix_widget *stack[]) {
    int i = 0;
    int closest_score = 0;
    struct optix_widget *closest = NULL;
    if (!reference || !stack || direction == OPTIX_CURSOR_NO_DIR) return NULL;
    //this is going to have to be recursive
    //return the element that is the closest
    //ideally, you'd use this by passing in the children of a window but it would also work in other cases as well
    while (stack[i]) {
        int score = 0;
        //this will make sure that every element is tested, which we may (?) not want
        struct optix_widget *current;
        if (stack[i]->type == OPTIX_WINDOW_TITLE_BAR_TYPE)
            current = optix_FindNearestElement(direction, reference, ((struct optix_window_title_bar *) stack[i])->window->widget.child);
        else if (stack[i]->state.visible) current = (stack[i]->child) ? optix_FindNearestElement(direction, reference, stack[i]->child) : stack[i];
        else {
            i++;
            continue;
        }
        //I only want to have buttons and window title bars be selectable at this moment
        if (current == optix_cursor.current_selection) {
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