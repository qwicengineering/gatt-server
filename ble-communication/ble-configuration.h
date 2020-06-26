/**
 * @file ble-configuration.h
 * @brief Definitions and configuration structures for the ble driver.
 *
 */

#ifndef BLE_CONFIGURATION_H_
#define BLE_CONFIGURATION_H_

#include <esp_bt.h>
#include <esp_bt_defs.h>
#include <esp_gap_ble_api.h>

#define SERVICE_UUID_LEN 16

#define BLE_APP_ID 0x55   /*< app_id value in case there are many bluetooth applications running */
#define BLE_LOCAL_MTU 500 /*< Local maximum frame size */

extern esp_bt_controller_config_t bt_controller_cfg; /*< Config for the bluetoth cntroller */

extern esp_ble_adv_data_t adv_data; /*< Advertising data content, according to "Supplement to the
                                       "Bluetooth Core Specification" */
extern esp_ble_adv_data_t scan_rsp_data; /*< Scan response data content, according to "Supplement to
                                       the "Bluetooth Core Specification" */
extern esp_ble_adv_params_t adv_params;  /*< Advertising parameters */
extern esp_ble_conn_update_params_t
    connection_params; /* Connection parameter decided after negotiating with the client*/

#endif /** BLE_CONFIGURATION_H_ */
