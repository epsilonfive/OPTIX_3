#include "text.h"

#include <debug.h>
#include <graphx.h>
#include <fontlibc.h>

#include "../shapes.h"
#include "../cursor.h"
#include "../colors.h"
#include "../util.h"

#include "input_box.h"


//sets a text object's width and height according to the text's width and height
void optix_InitializeTextTransform(struct optix_text *text) {
    text->widget.transform.x = 0;
    text->widget.transform.y = 0;
    if (current_context->data->font_valid) {
        text->widget.transform.width = fontlib_GetStringWidth(text->text);
        text->widget.transform.height = fontlib_GetCurrentFontHeight();
    } else {
        text->widget.transform.width = gfx_GetStringWidth(text->text);
        text->widget.transform.height = 8;
    }
    //text->widget.centering.x_centering = OPTIX_CENTERING_CENTERED;
    //text->widget.centering.y_centering = OPTIX_CENTERING_CENTERED;
    //text->widget.centering.x_offset = text->widget.centering.y_offset = 0;
}

//generic string length getting function, with a max number of characters
//why isn't this a default in graphx
//obviously the length doesn't include the null
size_t optix_GetStringWidthL(char *str, size_t max_chars) {
    if (current_context->data->font_valid) return fontlib_GetStringWidthL(str, max_chars);
    else if (strlen(str) < max_chars) return gfx_GetStringWidth(str);
    else {
        char temp = str[max_chars + 1];
        size_t size;
        str[max_chars + 1] = '\0';
        size = gfx_GetStringWidth(str);
        str[max_chars + 1] = temp;
        return size;
    }
}

size_t optix_GetStringWidth(char *str) {
    if (current_context->data->font_valid) return fontlib_GetStringWidth(str);
    else return gfx_GetStringWidth(str);
}

void optix_DrawStringL(char *str, size_t max_chars) {
    if (current_context->data->font_valid) fontlib_DrawStringL(str, max_chars);
    else {
        char temp = str[max_chars + 1];
        str[max_chars + 1] = '\0';
        //since we know the fontlib cursor position will always be set regardless of whether
        //or not the font is valid
        gfx_PrintStringXY(str, fontlib_GetCursorX(), fontlib_GetCursorY());
        str[max_chars + 1] = temp;
    }
}



//stolen and modified from DrDnar
//link: https://github.com/drdnar/open-adventure-ce/blob/db326b80753669cddf4a86697e14cdade8f49899/style.c#L186
char *optix_PrintStringWrapped_fontlibc(const char *string, bool fake_print) {
    char old_stop = fontlib_GetAlternateStopCode();
    unsigned int left = fontlib_GetWindowXMin();
    unsigned int width = fontlib_GetWindowWidth();
    unsigned int right = left + width;
    unsigned int str_width;
    unsigned int x = fontlib_GetCursorX();
    unsigned char first_printable = (unsigned char)fontlib_GetFirstPrintableCodePoint();
    unsigned char c;
    unsigned int space_width = fontlib_GetGlyphWidth(' ');
    if (first_printable == '\0') first_printable = '\1';
    fontlib_SetAlternateStopCode(' ');
    do {
        /* Check if the next word can fit on the current line */
        str_width = fontlib_GetStringWidth(string);
        if (x + str_width <= right)
            if (!fake_print) x = fontlib_DrawString(string);
            else x += str_width;
        else {
            /* If the word is super-long such that it won't fit in the window,
             * then forcibly print it starting on a new line. */
            if (str_width != 0) {
                //altered to make this greater than or equal to
                if (str_width >= width && x == left)
                    if (!fake_print) x = fontlib_DrawString(string);
                    else {
                        do x += (str_width = fontlib_GetGlyphWidth(*string++));
                        while (x < right);
                        string--;
                        break;
                    }
                else break;
            }
            /* If the width returned was zero, that means either another space
             * is waiting to be printed, which will be handled below; or a
             * control code is next, which also will be handled below.  This can
             * occur, for example, if a control code immediately follows a
             * space. */
        }
        /* FontLibC will kindly tell us exactly where it left off. */
        string = fontlib_GetLastCharacterRead();
        /* Now we need to deal with why the last word was terminated. */
        c = (unsigned char) (*string);
        /* If it's a control code, we either process a newline or give up. */
        if (c < first_printable) {
            if (c == ZERO_WIDTH_SPACE) string++;
            else if (c == '\t') {
                string++;
                x += 16;
                x &= 0xFFFFF0;
                if (!fake_print) {
                    fontlib_ClearEOL();
                    fontlib_SetCursorPosition(x, fontlib_GetCursorY());
                }
            }
            else break;
        }
        /* If it's a space, we need to process that manually since DrawString
         * won't handle it because we set space as a stop code. */
        if (c == ' ') {
            string++;
            /* We do actually need to check if there's space to print the
             * space. */
            if (x + space_width <= right) {
                if (!fake_print) fontlib_DrawGlyph(' ');
                x += space_width;
            }
            else {
                /* If there isn't room, we need to eat the space; it would look
                 * weird to print a space at the start of the next line. 
                 * However, we do not eat ALL the spaces if there's more than
                 * one, just the first one or two. */
                if (*string == ' ') /* Take care of possible second space. */
                    string++;
                break;
            }
        }
    } while (true);
    if (!fake_print) fontlib_ClearEOL();
    fontlib_SetAlternateStopCode(old_stop);
    return (char *) string;
}

void optix_UpdateText_default(struct optix_widget *widget) {
    struct optix_text *text = (struct optix_text *) widget;
    if (current_context->cursor->current_selection == widget || (current_context->settings->cursor_active && optix_CheckTransformOverlap(&current_context->cursor->widget, widget))) {
        int max_lines = widget->transform.height / TEXT_SPACING;
        int lines_to_render = max_lines < text->num_lines ? max_lines : text->num_lines;
        //handle the scrolling of the text
        bool up_pressed = current_context->settings->cursor_active ? optix_DefaultKeyIsDown(KEY_SUB) & KEY_PRESSED : optix_DefaultKeyIsDown(KEY_UP) & KEY_PRESSED;
        bool down_pressed = current_context->settings->cursor_active ? optix_DefaultKeyIsDown(KEY_ADD) & KEY_PRESSED : optix_DefaultKeyIsDown(KEY_DOWN) & KEY_PRESSED;
        if (down_pressed) {
            text->min = text->min < text->num_lines - lines_to_render ? text->min + 1 : text->min;
            widget->state.needs_redraw = true;
        }
        if (up_pressed) {
            text->min = text->min > 0 ? text->min - 1 : 0;
            widget->state.needs_redraw = true;
        }
    }
}

void optix_RenderText_default(struct optix_widget *widget) {
    struct optix_text *text = (struct optix_text *) widget;
    if (widget->state.visible && widget->state.needs_redraw) {
        if (text->use_custom_color) optix_SetTextColor(text->custom_fg_color, text->custom_bg_color);
        if (current_context->data->font_valid) {
            int max_lines = widget->transform.height / TEXT_SPACING;
            int lines_to_render = max_lines < text->num_lines ? max_lines : text->num_lines;
            int i = 0;
            char *str = text->text;
            fontlib_SetWindow(widget->transform.x, widget->transform.y, widget->transform.width, widget->transform.height);
            if (text->background_rectangle)
                optix_OutlinedRectangle_WithBevel(widget->transform.x, widget->transform.y, widget->transform.width, widget->transform.height,           //x, y, width, height
                BUTTON_BG_COLOR_UNSELECTED_INDEX, WINDOW_BORDER_BEVEL_DARK_INDEX, WINDOW_BORDER_BEVEL_LIGHT_INDEX);                                      //bevel (make it look depressed)
            //if the text is only one line, I think it's fair to make the assumption that it can just be printed (this will fix some issues with the
            //optix_PrintStringWrapped_fontlibc function not working correctly for single-line text with a window the same width as the string as well)
            while (i < text->min + lines_to_render) {
                char *old_str = str;
                bool special_character_processed = false;
                //only print if we're past  the text min
                fontlib_SetCursorPosition(widget->transform.x, widget->transform.y + (i < text->min ? 0 : i - text->min) * TEXT_SPACING);
                str = optix_PrintStringWrapped_fontlibc(str, true);
                switch (*str) {
                    case '\n':
                    //add more here later if necessary
                        special_character_processed = true;
                        break;
                    default:
                        break;
                }
                //we should print with the appropriate alignment
                if (i >= text->min) {
                    size_t str_length = optix_GetStringWidthL(old_str, (size_t) (str - old_str + (!special_character_processed)));
                    unsigned int new_x_pos = widget->transform.x + (text->alignment * ((widget->transform.width - str_length) / 2));
                    fontlib_SetCursorPosition(new_x_pos, fontlib_GetCursorY());
                    optix_DrawStringL(old_str, (size_t) (str - old_str + (!special_character_processed)));
                }
                if (special_character_processed) str++;
                i++;
                if (*str == '\0' || str == text->text) {
                    if (str == text->text) dbg_sprintf(dbgout, "WE'RE BREAKING, TAKE NOTICE!!!!!!\n");
                    break;
                }
            }
        } else gfx_PrintStringXY(text->text, widget->transform.x, widget->transform.y);
    }
}


void optix_GetTextNumLines(struct optix_widget *widget) {
    struct optix_text *text = (struct optix_text *) widget;
    //struct optix_input_box *input_box = (struct optix_input_box *) widget;
    //all we need to do here is to update the wrap if necessary
    char *str = text->text;
    char *last_str = text->text;
    text->num_lines = 0;
    fontlib_SetWindow(widget->transform.x, widget->transform.y, widget->transform.width, widget->transform.height);
    fontlib_SetCursorPosition(widget->transform.x, widget->transform.y);
    while (true) {
        last_str = str;
        str = optix_PrintStringWrapped_fontlibc(str, true);
        if (*str == '\n') str++;
        text->num_lines++;
        if (*str == '\0' || str == text->text || str == last_str) break;
    }
}


//sets the default font, which is in an appvar
bool optix_InitializeFont(void) {
    fontlib_font_t *font_pack;
    if ((font_pack = fontlib_GetFontByIndex(DEFAULT_FONT_PACK_NAME, 0))) {
        fontlib_SetFont(font_pack, 0);
        fontlib_SetColors(TEXT_FG_COLOR_INDEX, TEXT_BG_COLOR_INDEX);
        fontlib_SetTransparency(true);
        //fontlib_SetNewlineOptions(FONTLIB_AUTO_CLEAR_TO_EOL);
    }
    current_context->data->font_valid = (bool) font_pack;
    return (bool) font_pack;
}



