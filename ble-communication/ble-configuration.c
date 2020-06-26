#include "ble-configuration.h"
#include "esp_gatt_defs.h"

uint8_t manufacturer_uuid[2] = {0xFF, 0xFF};

/* The length of adv data must be less than 31 bytes */
esp_ble_adv_data_t adv_data = {
    .set_scan_rsp = false,
    .include_name = true,
    .include_txpower = true,
    .min_interval = 0x0006, // slave connection min interval, Time = min_interval * 1.25 msec
    .max_interval = 0x0010, // slave connection max interval, Time = max_interval * 1.25 msec
    .appearance = 0x00,
    .manufacturer_len = sizeof(manufacturer_uuid), // TEST_MANUFACTURER_DATA_LEN,
    .p_manufacturer_data = manufacturer_uuid,      // test_manufacturer,
    .service_data_len = 0,
    .p_service_data = NULL,
    .service_uuid_len = 0,
    .p_service_uuid = NULL,
    .flag = (ESP_BLE_ADV_FLAG_GEN_DISC | ESP_BLE_ADV_FLAG_BREDR_NOT_SPT),
};

// scan response data
esp_ble_adv_data_t scan_rsp_data = {
    .set_scan_rsp = true,
    .include_name = true,
    .include_txpower = true,
    .min_interval = 0x0006,
    .max_interval = 0x0010,
    .appearance = 0x00,
    .manufacturer_len = sizeof(manufacturer_uuid), // TEST_MANUFACTURER_DATA_LEN,
    .p_manufacturer_data = manufacturer_uuid,      //&test_manufacturer[0],
    .service_data_len = 0,
    .p_service_data = NULL,
    .service_uuid_len = 0,
    .p_service_uuid = NULL,
    .flag = (ESP_BLE_ADV_FLAG_GEN_DISC | ESP_BLE_ADV_FLAG_BREDR_NOT_SPT),
};

esp_ble_adv_params_t adv_params = {
    .adv_int_min = 0x20,
    .adv_int_max = 0x40,
    .adv_type = ADV_TYPE_IND,
    .own_addr_type = BLE_ADDR_TYPE_PUBLIC,
    .channel_map = ADV_CHNL_ALL,
    .adv_filter_policy = ADV_FILTER_ALLOW_SCAN_ANY_CON_ANY,
};

esp_bt_controller_config_t bt_controller_cfg = {
    .controller_task_stack_size = ESP_TASK_BT_CONTROLLER_STACK,
    .controller_task_prio = ESP_TASK_BT_CONTROLLER_PRIO,
    .hci_uart_no = BT_HCI_UART_NO_DEFAULT,
    .hci_uart_baudrate = BT_HCI_UART_BAUDRATE_DEFAULT,
    .scan_duplicate_mode = SCAN_DUPLICATE_MODE,
    .scan_duplicate_type = SCAN_DUPLICATE_TYPE_VALUE,
    .normal_adv_size = NORMAL_SCAN_DUPLICATE_CACHE_SIZE,
    .mesh_adv_size = MESH_DUPLICATE_SCAN_CACHE_SIZE,
    .send_adv_reserved_size = SCAN_SEND_ADV_RESERVED_SIZE,
    .controller_debug_flag = CONTROLLER_ADV_LOST_DEBUG_BIT,
    .mode = BTDM_CONTROLLER_MODE_EFF,
    .ble_max_conn = CONFIG_BTDM_CONTROLLER_BLE_MAX_CONN_EFF,
    .bt_max_acl_conn = CONFIG_BTDM_CONTROLLER_BR_EDR_MAX_ACL_CONN_EFF,
    .bt_max_sync_conn = CONFIG_BTDM_CONTROLLER_BR_EDR_MAX_SYNC_CONN_EFF,
    .magic = ESP_BT_CONTROLLER_CONFIG_MAGIC_VAL,
};

/**
 * @brief Parameters used to the connection negotiation once a device tries to start the pairing.
 *
 */
esp_ble_conn_update_params_t connection_params = {
    .latency = 15, /*!< How many events we can skip answering if we don't have new data in order to
                      save battery. To comply as an apple accesory it must be <=30 */
    .min_int =
        12, /*!< Min time interval between two consecutive BLE events. Unit is 1.25ms. To comply as
               an apple accesory it must be a multiple of 15ms, and greater or equal than 15ms. */
    .max_int = 24, /*!< Max time interval between two consecutive BLE events. Unit is 1.25ms. To
                      comply as an apple accesory it must follow that "Interval Min == Interval Max
                      == 15 ms" or that "Interval Min + 15 ms ≤ Interval Max" */
    .timeout =
        250, /*!< Time without activity before considering that the device is disconnected. Unit is
                10ms. To comply as an apple accesory it must be between 2 and 6 seconds*/
};
