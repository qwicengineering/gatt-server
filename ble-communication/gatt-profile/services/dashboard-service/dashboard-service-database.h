/**
 * @file dashboard-service-database.h
 * @brief This file contains the variables to hold the data related to this service.
 *
 */

#ifndef BLE_DASHBOARD_SERVICE_DATABASE_H_
#define BLE_DASHBOARD_SERVICE_DATABASE_H_
#include "../services-declarations.h"
#include "esp_gatt_defs.h"

/**
 * @brief  Service data structure.
 *
 */
typedef struct {
    uint16_t speed;
    cccd_t speed_ccc;

    uint16_t pedal_cadence;
    cccd_t pedal_cadence_ccc;

    uint8_t battery_charge;
    cccd_t battery_charge_ccc;

    uint8_t front_light;

    uint8_t walk_assist;

    uint16_t pedal_power;
    cccd_t pedal_power_ccc;

    uint16_t calories;
    cccd_t calories_ccc;

    uint32_t odometer;
    cccd_t odometer_ccc;

    uint8_t support_level;

    uint16_t console_status;

    uint32_t time;

    uint32_t trip_initial_odometer;

    uint32_t trip_initial_time;
} dashboard_service_data_t;

/**
 * @brief Indexes for the service
 *
 */
enum {
    IDX_DASHBOARD_SVC,

    IDX_DS_SPEED_DEC,
    IDX_DS_SPEED_VAL,
    IDX_DS_SPEED_CFG,

    IDX_DS_PEDAL_CADENCE_DEC,
    IDX_DS_PEDAL_CADENCE_VAL,
    IDX_DS_PEDAL_CADENCE_CFG,

    IDX_DS_BATTERY_CHARGE_DEC,
    IDX_DS_BATTERY_CHARGE_VAL,
    IDX_DS_BATTERY_CHARGE_CFG,

    IDX_DS_FRONT_LIGHT_DEC,
    IDX_DS_FRONT_LIGHT_VAL,

    IDX_DS_WALK_ASSIST_DEC,
    IDX_DS_WALK_ASSIST_VAL,

    IDX_DS_PEDAL_POWER_DEC,
    IDX_DS_PEDAL_POWER_VAL,
    IDX_DS_PEDAL_POWER_CFG,

    IDX_DS_CALORIES_DEC,
    IDX_DS_CALORIES_VAL,
    IDX_DS_CALORIES_CFG,

    IDX_DS_ODOMETER_DEC,
    IDX_DS_ODOMETER_VAL,
    IDX_DS_ODOMETER_CFG,

    IDX_DS_SUPPORT_LEVEL_DEC,
    IDX_DS_SUPPORT_LEVEL_VAL,

    IDX_DS_CONSOLE_STATUS_DEC,
    IDX_DS_CONSOLE_STATUS_VAL,

    IDX_DS_TIME_DEC,
    IDX_DS_TIME_VAL,

    IDX_DS_TRIP_INITIAL_ODOMETER_DEC,
    IDX_DS_TRIP_INITIAL_ODOMETER_VAL,

    IDX_DS_TRIP_INITIAL_TIME_DEC,
    IDX_DS_TRIP_INITIAL_TIME_VAL,

    DASHBOARD_SERVICE_NB,
};

extern uint8_t DASHBOARD_SERVICE_UUID[16]; /*!< Service UUID. */

extern esp_gatts_attr_db_t dashboard_service_gatt_db[DASHBOARD_SERVICE_NB]; /** Service database. */

extern dashboard_service_data_t
    dashboard_service_data; /*!< Copy of the chars values for easier access. */

#endif /* BLE_DASHBOARD_SERVICE_DATABASE_H_ */