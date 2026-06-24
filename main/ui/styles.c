#include "styles.h"
#include "images.h"
#include "fonts.h"

#include "ui.h"
#include "screens.h"

//
// Style: MAIN_TITLE
//

void init_style_main_title_MAIN_SCROLLED(lv_style_t *style) {
    lv_style_set_text_font(style, &lv_font_montserrat_28);
    lv_style_set_text_color(style, lv_color_hex(0x1a235b));
};

lv_style_t *get_style_main_title_MAIN_SCROLLED() {
    static lv_style_t *style;
    if (!style) {
        style = (lv_style_t *)lv_mem_alloc(sizeof(lv_style_t));
        lv_style_init(style);
        init_style_main_title_MAIN_SCROLLED(style);
    }
    return style;
};

void add_style_main_title(lv_obj_t *obj) {
    (void)obj;
    lv_obj_add_style(obj, get_style_main_title_MAIN_SCROLLED(), LV_PART_MAIN | LV_STATE_SCROLLED);
};

void remove_style_main_title(lv_obj_t *obj) {
    (void)obj;
    lv_obj_remove_style(obj, get_style_main_title_MAIN_SCROLLED(), LV_PART_MAIN | LV_STATE_SCROLLED);
};

//
//
//

void add_style(lv_obj_t *obj, int32_t styleIndex) {
    typedef void (*AddStyleFunc)(lv_obj_t *obj);
    static const AddStyleFunc add_style_funcs[] = {
        add_style_main_title,
    };
    add_style_funcs[styleIndex](obj);
}

void remove_style(lv_obj_t *obj, int32_t styleIndex) {
    typedef void (*RemoveStyleFunc)(lv_obj_t *obj);
    static const RemoveStyleFunc remove_style_funcs[] = {
        remove_style_main_title,
    };
    remove_style_funcs[styleIndex](obj);
}