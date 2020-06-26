#include "dashboard-service.h"
#include "dashboard-service-database.h"
#include "dashboard-service-event-handler.h"
#include "esp_log.h"

uint8_t get_ble_ds_support_level() { return dashboard_service_data.support_level; }
uint8_t get_ble_ds_front_light() { return dashboard_service_data.front_light; }

void set_ble_ds_speed(uint16_t *speed) {
    dashboard_service_data.speed = *speed;
    esp_ble_gatts_set_attr_value(dashboard_service_handles[IDX_DS_SPEED_VAL],
                                 sizeof(dashboard_service_data.speed),
                                 (uint8_t *)&dashboard_service_data.speed);
}
void set_ble_ds_pedal_cadence(uint16_t *pedal_cadence) {
    dashboard_service_data.pedal_cadence = *pedal_cadence;
    esp_ble_gatts_set_attr_value(dashboard_service_handles[IDX_DS_PEDAL_CADENCE_VAL],
                                 sizeof(dashboard_service_data.pedal_cadence),
                                 (uint8_t *)&dashboard_service_data.pedal_cadence);
}
void set_ble_ds_battery_charge(uint8_t *battery_charge) {
    dashboard_service_data.battery_charge = *battery_charge;
    esp_ble_gatts_set_attr_value(dashboard_service_handles[IDX_DS_BATTERY_CHARGE_VAL],
                                 sizeof(dashboard_service_data.battery_charge),
                                 (uint8_t *)&dashboard_service_data.battery_charge);
}
void set_ble_ds_front_light(uint8_t *front_light) {
    dashboard_service_data.front_light = *front_light;
    esp_ble_gatts_set_attr_value(dashboard_service_handles[IDX_DS_FRONT_LIGHT_VAL],
                                 sizeof(dashboard_service_data.front_light),
                                 (uint8_t *)&dashboard_service_data.front_light);
}
void set_ble_ds_walk_assist(uint8_t *walk_assist) {
    dashboard_service_data.walk_assist = *walk_assist;
    esp_ble_gatts_set_attr_value(dashboard_service_handles[IDX_DS_WALK_ASSIST_VAL],
                                 sizeof(dashboard_service_data.walk_assist),
                                 (uint8_t *)&dashboard_service_data.walk_assist);
}
void set_ble_ds_pedal_power(uint16_t *pedal_power) {
    dashboard_service_data.pedal_power = *pedal_power;
    esp_ble_gatts_set_attr_value(dashboard_service_handles[IDX_DS_PEDAL_POWER_VAL],
                                 sizeof(dashboard_service_data.pedal_power),
                                 (uint8_t *)&dashboard_service_data.pedal_power);
}
void set_ble_ds_calories(uint16_t *calories) {
    dashboard_service_data.calories = *calories;
    esp_ble_gatts_set_attr_value(dashboard_service_handles[IDX_DS_CALORIES_VAL],
                                 sizeof(dashboard_service_data.calories),
                                 (uint8_t *)&dashboard_service_data.calories);
}
void set_ble_ds_odometer(uint32_t *odometer) {
    dashboard_service_data.odometer = *odometer;
    esp_ble_gatts_set_attr_value(dashboard_service_handles[IDX_DS_ODOMETER_VAL],
                                 sizeof(dashboard_service_data.odometer),
                                 (uint8_t *)&dashboard_service_data.odometer);
}
void set_ble_ds_support_level(uint8_t *support_level) {
    dashboard_service_data.support_level = *support_level;
    esp_ble_gatts_set_attr_value(dashboard_service_handles[IDX_DS_SUPPORT_LEVEL_VAL],
                                 sizeof(dashboard_service_data.support_level),
                                 (uint8_t *)&dashboard_service_data.support_level);
}
void set_ble_ds_console_status(uint16_t *console_status) {
    dashboard_service_data.console_status = *console_status;
    esp_ble_gatts_set_attr_value(dashboard_service_handles[IDX_DS_CONSOLE_STATUS_VAL],
                                 sizeof(dashboard_service_data.console_status),
                                 (uint8_t *)&dashboard_service_data.console_status);
}
void set_ble_ds_time(uint32_t *time) {
    dashboard_service_data.time = *time;
    esp_ble_gatts_set_attr_value(dashboard_service_handles[IDX_DS_TIME_VAL],
                                 sizeof(dashboard_service_data.time),
                                 (uint8_t *)&dashboard_service_data.time);
}
void set_ble_ds_trip_initial_odometer(uint32_t *trip_initial_odometer) {
    dashboard_service_data.trip_initial_odometer = *trip_initial_odometer;
    esp_ble_gatts_set_attr_value(dashboard_service_handles[IDX_DS_TRIP_INITIAL_ODOMETER_VAL],
                                 sizeof(dashboard_service_data.trip_initial_odometer),
                                 (uint8_t *)&dashboard_service_data.trip_initial_odometer);
}
void set_ble_ds_trip_initial_time(uint32_t *trip_initial_time) {
    dashboard_service_data.trip_initial_time = *trip_initial_time;
    esp_ble_gatts_set_attr_value(dashboard_service_handles[IDX_DS_TRIP_INITIAL_TIME_VAL],
                                 sizeof(dashboard_service_data.trip_initial_time),
                                 (uint8_t *)&dashboard_service_data.trip_initial_time);
}
