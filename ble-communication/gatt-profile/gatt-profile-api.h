/**
 * @file gatt-profile-api.h
 * @brief Used to expose the config structure and public functions only.
 *
 */

#ifndef BLE_GATT_PROFILE_API_H_
#define BLE_GATT_PROFILE_API_H_

#include "services/dashboard-service/dashboard-service.h"

/**
 * @brief GATT profile configuration object.
 *
 * Includes the configuration needed for each service in the profile.
 * If there is need for extra configuration (e.g. add advertised service as configurtion parameter)
 * it should be done here.
 *
 */
typedef struct {
    dashboard_service_cfg_t ds_cfg;
} gatt_profile_config_t;

/**
 * @brief Enum to identify if the ble client connected
 *
 */

typedef enum {
    BLE_CLIENT_DISCONNECTED,
    BLE_CLIENT_CONNECTED,
} conn_status_t;

/**
 * @brief Status of the BLE connection
 *
 */
typedef struct {
    conn_status_t status;
    esp_bd_addr_t client_add;
    uint16_t conn_id;
} ble_connection_status_t;

/**
 * @brief Get the conn status object
 *
 * @return conn_status_t
 */
ble_connection_status_t ble_get_conn_status();

#endif /* BLE_GATT_PROFILE_API_H_ */