/**
 * @file template-service-database.h
 * @brief This file contains the variables to hold the data related to this service.
 *
 */

#ifndef BLE_TEMPLATE_SERVICE_DATABASE_H_
#define BLE_TEMPLATE_SERVICE_DATABASE_H_
#include "../services-declarations.h"
#include "esp_gatt_defs.h"

/**
 * @brief  Service data structure.
 *
 * Add a variable here for each characteristic with the needed type. Also add an extra variable of
 * the type cccd_t in case that characteristic supports client configuration
 */
typedef struct {
    uint8_t char_1;
    cccd_t char_1_ccc;
} service_data_t;

/**
 * @brief Indexes for the service
 *
 * Index of each address of the service. Always starts by the service itself and ends with the _NB
 * to signal the total amount.
 *
 * Each characteristic has the _DEC for it's descriptor, _VAL for its value and optionally _CFG if
 * it implements client configuration.
 */
enum {
    IDX_TEMPLATE_SVC,

    IDX_TEMPLATE_CHAR_1_DEC,
    IDX_TEMPLATE_CHAR_1_VAL,
    IDX_TEMPLATE_CHAR_1_CFG,

    TEMPLATE_SERVICE_NB,
};

// The UUID of the service needs to be accesible by the gatrt_profile file in order to send events
// BLE to the correct service that originated them.
extern uint8_t TEMPLATE_SERVICE_UUID[16]; /*!< Service UUID. */

extern esp_gatts_attr_db_t template_service_gatt_db[TEMPLATE_SERVICE_NB]; /** Service database. */

extern service_data_t service_data; /*!< Copy of the chars values for easier access. */

#endif /* BLE_TEMPLATE_SERVICE_DATABASE_H_ */