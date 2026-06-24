#ifndef EEZ_LVGL_UI_STYLES_H
#define EEZ_LVGL_UI_STYLES_H

#include <lvgl.h>

#ifdef __cplusplus
extern "C" {
#endif

// Style: MAIN_TITLE
lv_style_t *get_style_main_title_MAIN_SCROLLED();
void add_style_main_title(lv_obj_t *obj);
void remove_style_main_title(lv_obj_t *obj);

#ifdef __cplusplus
}
#endif

#endif /*EEZ_LVGL_UI_STYLES_H*/