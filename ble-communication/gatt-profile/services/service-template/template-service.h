/**
 * @file template-service.h
 * @brief Template to implement a BLE service
 *
 * The implementation is divided in three files:
 *  - template-service: This one. Is included by the gatt-profile-api. Implements and exposes
 * the public functions and the configuration structure.
 *  - template-service-event-handler: Included by the gatt-profile. Implements the event handler for
 * the service and the initialization function.
 * - template-service-database: Contains the declaration of all the variales related to the service
 * data
 *
 */

#ifndef BLE_TEMPLATE_SERVICE_H_
#define BLE_TEMPLATE_SERVICE_H_

#include "../services-declarations.h"
#include "esp_err.h"
#include "stdint.h"

/**
 * @brief Configuration structure for the service.
 *
 * Add callbacks to be executed after a client writes a value and the ccc fields so the system can
 * persist them
 *
 */
typedef struct {
    uint8_char_write_check_t
        char_1_write_check; /*< callback to called after the char_1 is written */
    cccd_t char_1_ccc;      /*< Persistance for the assist level characteristic configuration */
} template_service_cfg_t;

/**
 * @defgroup TemplateServiceApi Public functions to access the characteristics
 * @{
 */

uint8_t
get_ble_char_1(); /*< Retrieves the value of char_1. This is needed for writable characteristics */

void set_ble_char_1(uint8_t char_1); /* Modifies the value of char_1. A notification will be sent if
                                        they are configured for the char */

/**@}*/
#endif /* BLE_TEMPLATE_SERVICE_H_ */