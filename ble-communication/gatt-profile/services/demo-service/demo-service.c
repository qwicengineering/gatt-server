#include "demo-service.h"
#include "demo-service-database.h"
#include "demo-service-event-handler.h"

uint8_t get_ble_assist_level() { return demo_service_data.assist_level; }

void set_ble_assisst_level(uint8_t assist_level) {
    demo_service_data.assist_level = assist_level;
    esp_ble_gatts_set_attr_value(demo_service_handles[IDX_ASSIST_LEVEL_VAL], sizeof(assist_level),
                                 (uint8_t *)&demo_service_data.assist_level);
}

void set_ble_battery_charge(uint8_t battery_charge) {
    demo_service_data.battery_charge = battery_charge;
    esp_ble_gatts_set_attr_value(demo_service_handles[IDX_BATTERY_CHARGE_VAL],
                                 sizeof(battery_charge),
                                 (uint8_t *)&demo_service_data.battery_charge);
}

void set_ble_speed(uint16_t speed) {
    demo_service_data.speed = speed;
    esp_ble_gatts_set_attr_value(demo_service_handles[IDX_SPEED_VAL], sizeof(speed),
                                 (uint8_t *)&demo_service_data.speed);
}