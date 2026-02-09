#include <stdio.h>
#include <inttypes.h>
#include "esp_log.h"
#include "esp_chip_info.h"
#include "esp_flash.h"
#include "esp_system.h"
#include "esp_heap_caps.h"

#include "lvgl.h"
#include "display.h"
#include "esp_bsp.h"
#include "lv_port.h"

#include "ui/ui.h"



static const char *TAG = "DEMO_LVGL";

#define logSection(section) \
    ESP_LOGI(TAG, "\n\n************* %s **************\n", section);

/**
 * Rotación: 0, 90, 180 o 270 grados
 */
#define LVGL_PORT_ROTATION_DEGREE 270

void draw_argentina_flag(lv_obj_t *parent);

void app_main(void)
{
    logSection("LVGL porting example start");

    /* Información del chip */
    esp_chip_info_t chip_info;
    uint32_t flash_size;

    esp_chip_info(&chip_info);
    ESP_LOGI(TAG, "This is %s chip with %d CPU core(s), %s%s%s%s",
             CONFIG_IDF_TARGET,
             chip_info.cores,
             (chip_info.features & CHIP_FEATURE_WIFI_BGN) ? "WiFi/" : "",
             (chip_info.features & CHIP_FEATURE_BT) ? "BT/" : "",
             (chip_info.features & CHIP_FEATURE_BLE) ? "BLE/" : "",
             (chip_info.features & CHIP_FEATURE_IEEE802154) ? "802.15.4" : "");

    if (esp_flash_get_size(NULL, &flash_size) != ESP_OK) {
        ESP_LOGE(TAG, "Get flash size failed");
        return;
    }

    ESP_LOGI(TAG, "%" PRIu32 "MB %s flash",
             flash_size / (1024 * 1024),
             (chip_info.features & CHIP_FEATURE_EMB_FLASH) ? "embedded" : "external");

    ESP_LOGI(TAG, "Minimum free heap size: %" PRIu32 " bytes",
             esp_get_minimum_free_heap_size());

    size_t freePsram = heap_caps_get_free_size(MALLOC_CAP_SPIRAM);
    ESP_LOGI(TAG, "Free PSRAM: %d bytes", freePsram);

    /* Inicialización del display */
    logSection("Initialize panel device");

    bsp_display_cfg_t cfg = {
        .lvgl_port_cfg = ESP_LVGL_PORT_INIT_CONFIG(),
        .buffer_size = EXAMPLE_LCD_QSPI_H_RES * EXAMPLE_LCD_QSPI_V_RES,
#if LVGL_PORT_ROTATION_DEGREE == 90
        .rotate = LV_DISP_ROT_90,
#elif LVGL_PORT_ROTATION_DEGREE == 270
        .rotate = LV_DISP_ROT_270,
#elif LVGL_PORT_ROTATION_DEGREE == 180
        .rotate = LV_DISP_ROT_180,
#else
        .rotate = LV_DISP_ROT_NONE,
#endif
    };

    bsp_display_start_with_config(&cfg);
    bsp_display_backlight_on();


    /* Crear UI */
    logSection("Create UI");

    bsp_display_lock(0);

    //lv_obj_t *scr = lv_scr_act();   // <-- ORIGEN DE LVGL CREADO
    //draw_argentina_flag(scr);       // <-- LLAMADA A TU FUNCIÓN

    ui_init();


    bsp_display_unlock();

    logSection("LVGL porting example end");

    /* Bucle idle */
    while (1) {
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}


/**
 * @brief Dibuja la bandera de Argentina en la pantalla usando LVGL.
 */
void draw_argentina_flag(lv_obj_t *parent)
{
    const lv_coord_t width  = 480;   // tu resolución
    const lv_coord_t height = 320;

    const lv_coord_t stripe_height = height / 3;

    lv_color_t celeste = lv_color_make(116, 172, 223);   // #74ACDF
    lv_color_t blanco  = lv_color_white();

    /* Faja superior */
    lv_obj_t *top = lv_obj_create(parent);
    lv_obj_set_size(top, width, stripe_height);
    lv_obj_set_style_bg_color(top, celeste, LV_PART_MAIN);
    lv_obj_set_style_border_width(top, 0, LV_PART_MAIN);
    lv_obj_set_pos(top, 0, 0);

    /* Faja del medio */
    lv_obj_t *middle = lv_obj_create(parent);
    lv_obj_set_size(middle, width, stripe_height);
    lv_obj_set_style_bg_color(middle, blanco, LV_PART_MAIN);
    lv_obj_set_style_border_width(middle, 0, LV_PART_MAIN);
    lv_obj_set_pos(middle, 0, stripe_height);

    /* Faja inferior */
    lv_obj_t *bottom = lv_obj_create(parent);
    lv_obj_set_size(bottom, width, stripe_height);
    lv_obj_set_style_bg_color(bottom, celeste, LV_PART_MAIN);
    lv_obj_set_style_border_width(bottom, 0, LV_PART_MAIN);
    lv_obj_set_pos(bottom, 0, stripe_height * 2);
}
