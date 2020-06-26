/**
 * @file dashboard-service.h
 * @brief Implementation of the Dashboard service
 *
 */

#ifndef BLE_DASHBOARD_SERVICE_H_
#define BLE_DASHBOARD_SERVICE_H_

#include "../services-declarations.h"
#include "esp_err.h"
#include "stdint.h"

/**
 * @brief Configuration structure for the service.
 *
 */
typedef struct {
    uint8_char_write_check_t
        support_level_write_cb; /*< callback called when the client tries to write the level */
    uint8_char_write_check_t
        front_light_write_cb; /*< callback called when the client tries to write the time */
    uint32_char_write_check_t
        time_write_cb; /*< callback called when the client tries to write the time */
    cccd_t speed_ccc;
    cccd_t pedal_cadence_ccc;
    cccd_t battery_charge_ccc;
    cccd_t pedal_power_ccc;
    cccd_t calories_ccc;
    cccd_t odometer_ccc;
    cccd_t trip_initial_time_ccc;
} dashboard_service_cfg_t;

/**
 * @defgroup DashboardServiceApi Public functions to access the characteristics
 * @{
 */
uint8_t get_ble_ds_support_level();
uint8_t get_ble_ds_front_light();

void set_ble_ds_speed(uint16_t *speed);
void set_ble_ds_pedal_cadence(uint16_t *pedal_cadence);
void set_ble_ds_battery_charge(uint8_t *battery_charge);
void set_ble_ds_front_light(uint8_t *front_light);
void set_ble_ds_walk_assist(uint8_t *walk_assist);
void set_ble_ds_pedal_power(uint16_t *pedal_power);
void set_ble_ds_calories(uint16_t *calories);
void set_ble_ds_odometer(uint32_t *odometer);
void set_ble_ds_support_level(uint8_t *support_level);
void set_ble_ds_console_status(uint16_t *console_status);
void set_ble_ds_time(uint32_t *time);
void set_ble_ds_trip_initial_odometer(uint32_t *trip_initial_odometer);
void set_ble_ds_trip_initial_time(uint32_t *trip_initial_time);
/**@}*/
#endif /* BLE_DASHBOARD_SERVICE_H_ */