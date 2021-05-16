#ifndef cursor_text_include_file
#define cursor_text_include_file

#ifdef __cplusplus
extern "C" {
#endif

#define cursor_text_width 16
#define cursor_text_height 16
#define cursor_text_size 258
#define cursor_text ((gfx_sprite_t*)cursor_text_data)
extern unsigned char cursor_text_data[258];

#ifdef __cplusplus
}
#endif

#endif
