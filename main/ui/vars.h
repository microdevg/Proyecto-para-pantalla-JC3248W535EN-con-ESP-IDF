#ifndef EEZ_LVGL_UI_VARS_H
#define EEZ_LVGL_UI_VARS_H

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

// enum declarations

// Flow global variables

enum FlowGlobalVariables {
    FLOW_GLOBAL_VARIABLE_MEDICION = 0,
    FLOW_GLOBAL_VARIABLE_REFERENCIA = 1,
    FLOW_GLOBAL_VARIABLE_STRING_MEDICION = 2,
    FLOW_GLOBAL_VARIABLE_STRING_REFERENCIA = 3,
    FLOW_GLOBAL_VARIABLE_HEATTING = 4,
    FLOW_GLOBAL_VARIABLE_FAN = 5
};

// Native global variables

extern int32_t get_var_medicion();
extern void set_var_medicion(int32_t value);
extern int32_t get_var_referencia();
extern void set_var_referencia(int32_t value);
extern const char *get_var_string_medicion();
extern void set_var_string_medicion(const char *value);
extern const char *get_var_string_referencia();
extern void set_var_string_referencia(const char *value);
extern bool get_var_heatting();
extern void set_var_heatting(bool value);
extern bool get_var_fan();
extern void set_var_fan(bool value);

#ifdef __cplusplus
}
#endif

#endif /*EEZ_LVGL_UI_VARS_H*/