/**
 * @file demo-service.h
 * @brief Implementation of a simple service. The declarations on this file are accesible from
 * outside the library.
 *
 * This is the service implemented fo the first demo, the idea is to use it as a template for future
 * services.
 *
 * The implementation is divided in three files:
 *  - demo-service: This one. Is included by the gatt-profile-api. Implements and exposes
 * the public functions and the configuration structure.
 *  - demo-service-event-handler: Included by the gatt-profile. Implements the event handler for the
 * service and the initialization function.
 * - demo-service-database: Contains the declaration of all the variales related to the service data
 *
 */

#ifndef BLE_DEMO_SERVICE_H_
#define BLE_DEMO_SERVICE_H_

#include "../services-declarations.h"
#include "esp_err.h"
#include "stdint.h"

/**
 * @brief Configuration structure for the demo service.
 *
 */
typedef struct {
    uint8_char_write_check_t assist_level_write_check; /*< callback to validate the assist level
                                                          value when the client changes it */
    cccd_t assist_level_cccd; /*< Persistance for the assist level characteristic configuration */
    cccd_t
        battery_charge_cccd; /*< Persistance for the battery charge characteristic configuration */
    cccd_t speed_cccd;       /*< Persistance for the speed characteristic configuration */
} demo_service_cfg_t;

/* Public access to the service data. */

/**
 * @defgroup DemoServiceApi Functions to access the characteristics shared witht the client by the
 * demo service.
 * @{
 */

uint8_t get_ble_assist_level(); /*< Retrieves the assist level. Assist level needs a get since the
                                   client can write on it */

void set_ble_assisst_level(uint8_t assist_level);
void set_ble_battery_charge(uint8_t battery_charge);
void set_ble_speed(uint16_t speed);
/**@}*/
#endif /* BLE_DEMO_SERVICE_H_ */