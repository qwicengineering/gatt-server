/**
 * @file ble-communication.h
 * @brief This library manages advertising and communication over BLE.
 *
 * The profile can manage many services defined independently in /gatt-profile/services.
 *
 * If the service is implemented properly it'll expose a get/set API for each characteristic value
 * and a configuration structure for the library init function.
 *
 * The service configuration typically includes the persisted value for the cccd characteristics and
 * a callback for value validation.
 *
 */
#ifndef BLE_COMMUNICATION_H_
#define BLE_COMMUNICATION_H_

#include "esp_err.h"
#include "gatt-profile/gatt-profile-api.h"

/**
 * @brief Configuration for the BLE device.
 *
 */
typedef struct {
    char *device_name;
    gatt_profile_config_t gatt_config;
} ble_communication_config_t;

/**
 * @brief Initializes the ble module
 *
 * @return esp_err_t
 */
esp_err_t ble_communication_init(ble_communication_config_t ble_config);

#endif /* BLE_COMMUNICATION_H_ */
