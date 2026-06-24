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
#include "ui/vars.h"
#include "ui/actions.h"
#include "ui/screens.h"
#include <string.h>


void crear_pantalla_contador(void) ;

bool fan=true;

bool get_var_fan() {
    return fan;
}

void set_var_fan(bool value) {
    fan = value;
}


bool heatting = true;

bool get_var_heatting() {
    return heatting;
}

void set_var_heatting(bool value) {
    heatting = value;
}


int32_t medicion = 0, referencia = 0;
 int32_t get_var_medicion(){
    return medicion;
}
 void set_var_medicion(int32_t value){
    medicion = value;
}
 int32_t get_var_referencia(){
    return referencia;
}
 void set_var_referencia(int32_t value){
    referencia = value;
}



QueueHandle_t xQueueRef = NULL;


void vTaskMedicion(void * pvParameters);

char string_medicion[100] = { "---" };
char string_referencia[100] = { "---" };


const char *get_var_string_medicion() {
    return string_medicion;
}

void set_var_string_medicion(const char *value) {
    strncpy(string_medicion, value, sizeof(string_medicion) / sizeof(char));
    string_medicion[sizeof(string_medicion) / sizeof(char) - 1] = 0;
}



const char *get_var_string_referencia() {
    return string_referencia;
}

void set_var_string_referencia(const char *value) {
    strncpy(string_referencia, value, sizeof(string_referencia) / sizeof(char));
    string_referencia[sizeof(string_referencia) / sizeof(char) - 1] = 0;
}


 void action_update_data(lv_event_t * e){
    int32_t value = lv_arc_get_value( objects.refencia_slider);
    set_var_referencia(value);
    char buffer[10]={0};
    printf("El valor de arc es:%ld\n",value);
    sprintf(buffer,"%ld",value);
    set_var_string_referencia(buffer);
    ui_tick();

}




 void action_set_ready(lv_event_t * e){

  
// 1. Sacamos el valor del arco
    int32_t ref = lv_arc_get_value(objects.refencia_slider);
    printf("action:%ld -> Enviando a la cola...\n", ref);

    if (xQueueRef != NULL) {
        // 2. Enviamos el valor a la cola sin bloquear (timeout = 0)
        // Si la cola está llena, xQueueOverwrite o liberar espacio dependerá de tu lógica, 
        // aquí usamos xQueueSend.
        if (xQueueSend(xQueueRef, &ref, 0) != pdPASS) {
            printf("Cola llena. Ignorando nuevo valor.\n");
        }
    }

 
    

    
}

static const char *TAG = "DEMO_LVGL";

#define logSection(section) \
    ESP_LOGI(TAG, "\n\n************* %s **************\n", section);

/**
 * Rotación: 0, 90, 180 o 270 grados
 */
#define LVGL_PORT_ROTATION_DEGREE 270

void draw_argentina_flag(lv_obj_t *parent);
void info_logs();




void app_main(void)
{
  

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

    logSection("Create UI");

    bsp_display_lock(0);

    lv_obj_t *scr = lv_scr_act();   // <-- ORIGEN DE LVGL CREADO
 //   draw_argentina_flag(scr);       // <-- LLAMADA A TU FUNCIÓN
   ui_init();
    bsp_display_unlock();

    logSection("LVGL porting example end");
    int32_t value = lv_arc_get_value( objects.refencia_slider);

    set_var_referencia(value);
    char buff[20]={0};

    sprintf(buff,"%ld",value);
    set_var_string_referencia(buff);
   


// Creamos una cola para guardar 1 elemento de tipo int32_t
    xQueueRef = xQueueCreate(1, sizeof(int32_t));
    
    if (xQueueRef != NULL) {
        // Creamos la tarea que procesará el bucle en segundo plano
        xTaskCreate(vTaskMedicion, "TaskMedicion", 4000, NULL, 1, NULL);
    } else {
        printf("Error al crear la cola\n");
    }

    crear_pantalla_contador();

    while (1) {
        vTaskDelay(pdMS_TO_TICKS(500));
        lv_timer_handler();
    }
}

void draw_argentina_flag(lv_obj_t *parent)
{
    const lv_coord_t width  = 480;   // tu resolución
    const lv_coord_t height = 320;

    const lv_coord_t stripe_height = height / 3;

    lv_color_t celeste = lv_color_make(116, 172, 223);   // #74ACDF
    lv_color_t blanco  = lv_color_white();
    lv_color_t amarillo_sol = lv_color_make(252, 191, 73); // #FCBF49 (Amarillo Oro)

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

    /* El Sol de Mayo */
    const lv_coord_t sun_size = 66; // Tamaño proporcional para la faja blanca (~106px)
    lv_obj_t *sun = lv_obj_create(parent);
    lv_obj_set_size(sun, sun_size, sun_size);
    
    // Convertimos el objeto en un círculo perfecto
    lv_obj_set_style_radius(sun, LV_RADIUS_CIRCLE, LV_PART_MAIN);
    lv_obj_set_style_bg_color(sun, amarillo_sol, LV_PART_MAIN);
    lv_obj_set_style_border_width(sun, 0, LV_PART_MAIN);
    
    // Desactivar barras de desplazamiento por si acaso
    lv_obj_clear_flag(sun, LV_OBJ_FLAG_SCROLLABLE); 
    
    // Centrado absoluto en la pantalla (X y Y)
    lv_obj_set_pos(sun, (width - sun_size) / 2, (height - sun_size) / 2);
}



void info_logs(){
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
}


void vTaskMedicion(void * pvParameters)
{
    int32_t target_ref = 0;
    char buffer[20];

    for(;;) 
    {
        // La tarea se bloquea aquí hasta que action_set_ready mande un dato
        if (xQueueReceive(xQueueRef, &target_ref, portMAX_DELAY) == pdPASS) 
        {
            printf("Tarea iniciada. Destino: %ld\n", target_ref);

            for (int32_t value = get_var_medicion(); value != target_ref; ) 
            {
                if(value < target_ref) {
                    set_var_heatting(false);
                    set_var_fan(true);

                    value++;
                }
                else{
                    set_var_fan(false);
                    set_var_heatting(true);
                    value --;
                }
               
                set_var_medicion(value);
                
                snprintf(buffer, sizeof(buffer), "%ld ", value);
                set_var_string_medicion(buffer);
                
                
                 ui_tick(); // Si esto maneja lógica interna tuya, déjalo. 

                vTaskDelay(pdMS_TO_TICKS(200)); 
            }
            set_var_heatting(true);
            set_var_fan(true);
            ui_tick(); // Si esto maneja lógica interna tuya, déjalo. 

            printf("Bucle de medición finalizado.\n");
        }
    }
}



// --- VARIABLES Y CALLBACKS (Por fuera de la función de la UI) ---

static int32_t contador = 0;
static lv_obj_t * lbl_contador;

// Acción para el botón de incrementar (+1)
static void btn_incrementar_cb(lv_event_t * e) {
    lv_event_code_t code = lv_event_get_code(e);
    if(code == LV_EVENT_CLICKED) {
        contador++;
        lv_label_set_text_fmt(lbl_contador, "%ld", contador);
    }
}

// Acción para el botón de decrementar (-1)
static void btn_decrementar_cb(lv_event_t * e) {
    lv_event_code_t code = lv_event_get_code(e);
    if(code == LV_EVENT_CLICKED) {
        contador--;
        lv_label_set_text_fmt(lbl_contador, "%ld", contador);
    }
}


// --- FUNCIÓN PRINCIPAL DE LA INTERFAZ ---

void crear_pantalla_contador(void) {
    // Obtener la pantalla activa actual
    lv_obj_t * pantalla = lv_scr_act();

    // 1. ETIQUETA DEL CONTADOR (Número grande en el centro)
    lbl_contador = lv_label_create(pantalla);
    lv_label_set_text(lbl_contador, "0");
    
    // Usamos una fuente grande nativa de LVGL para que resalte
    lv_obj_set_style_text_font(lbl_contador, &lv_font_montserrat_32, 0); 
    // Alineado al centro, pero subido 40 píxeles en el eje Y
    lv_obj_align(lbl_contador, LV_ALIGN_CENTER, 0, -40);


    // 2. BOTÓN DE DECREMENTAR (-1)
    lv_obj_t * btn_menos = lv_btn_create(pantalla);
    lv_obj_set_size(btn_menos, 90, 50); // Tamaño ideal para pantallas de 480x320
    // Desplazado 80px a la izquierda y 40px hacia abajo del centro
    lv_obj_align(btn_menos, LV_ALIGN_CENTER, -80, 40); 
    lv_obj_add_event_cb(btn_menos, btn_decrementar_cb, LV_EVENT_CLICKED, NULL);

    // Texto del botón -1
    lv_obj_t * lbl_menos = lv_label_create(btn_menos);
    lv_label_set_text(lbl_menos, "-1");
    lv_obj_center(lbl_menos);


    // 3. BOTÓN DE INCREMENTAR (+1)
    lv_obj_t * btn_mas = lv_btn_create(pantalla);
    lv_obj_set_size(btn_mas, 90, 50);
    // Desplazado 80px a la derecha y 40px hacia abajo del centro
    lv_obj_align(btn_mas, LV_ALIGN_CENTER, 80, 40); 
    lv_obj_add_event_cb(btn_mas, btn_incrementar_cb, LV_EVENT_CLICKED, NULL);

    // Texto del botón +1
    lv_obj_t * lbl_mas = lv_label_create(btn_mas);
    lv_label_set_text(lbl_mas, "+1");
    lv_obj_center(lbl_mas);
}