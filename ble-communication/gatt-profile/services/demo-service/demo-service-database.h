/**
 * @file demo-service-database.h
 * @brief This file contains the variables to hold the data related to this service.
 *
 */

#ifndef BLE_DEMO_SERVICE_DATABASE_H_
#define BLE_DEMO_SERVICE_DATABASE_H_
#include "../services-declarations.h"
#include "esp_gatt_defs.h"

/**
 * @brief  Demo service data structure.
 *
 */
typedef struct {
    uint8_t assist_level;
    cccd_t assist_level_ccc;
    uint8_t battery_charge;
    cccd_t battery_charge_ccc;
    uint16_t speed;
    cccd_t speed_ccc;
} demo_service_data_t;

/**
 * @brief Table indexes for the demo service
 *
 */
enum {
    IDX_DEMO_SVC,

    IDX_ASSIST_LEVEL_DEC,
    IDX_ASSIST_LEVEL_VAL,
    IDX_ASSIST_LEVEL_CFG,

    IDX_BATTERY_CHARGE_DEC,
    IDX_BATTERY_CHARGE_VAL,
    IDX_BATTERY_CHARGE_CFG,

    IDX_SPEED_DEC,
    IDX_SPEED_VAL,
    IDX_SPEED_CFG,

    DEMO_SERVICE_NB,
};

extern uint8_t DEMO_SERVICE_UUID[16]; /*!< Service UUID. */

extern esp_gatts_attr_db_t demo_service_gatt_db[DEMO_SERVICE_NB]; /** Service database. */

extern demo_service_data_t demo_service_data; /*!< Copy of the chars values for easier access. */

#endif /* BLE_DEMO_SERVICE_DATABASE_H_ */