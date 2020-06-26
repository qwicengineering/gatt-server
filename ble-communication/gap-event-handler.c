#include "gap-event-handler.h"
#include "ble-configuration.h"

#include <esp_gap_ble_api.h>
#include <esp_log.h>

#define BLE_GAP_TAG "BLE-gap"

#define ADV_CONFIG_FLAG (1 << 0)
#define SCAN_RSP_CONFIG_FLAG (1 << 1)

uint8_t config_status = 0b0;

void gap_event_handler(esp_gap_ble_cb_event_t event, esp_ble_gap_cb_param_t *param) {

    ESP_LOGI(BLE_GAP_TAG, "GAP_EVENT_HANDLER event: %d", event);

    switch (event) {
    case ESP_GAP_BLE_ADV_DATA_SET_COMPLETE_EVT:
        /* Set adv params flag*/
        config_status &= ADV_CONFIG_FLAG;
        /* start advertising when adv params and scan_response params are set*/
        if (config_status == 0) {
            esp_ble_gap_start_advertising(&adv_params);
        }
        break;
    case ESP_GAP_BLE_SCAN_RSP_DATA_SET_COMPLETE_EVT:
        /* Set scan response flag*/
        config_status &= SCAN_RSP_CONFIG_FLAG;

        if (config_status == (ADV_CONFIG_FLAG | SCAN_RSP_CONFIG_FLAG)) {
            esp_ble_gap_start_advertising(&adv_params);
        }
        break;
    case ESP_GAP_BLE_ADV_START_COMPLETE_EVT:
        if (param->adv_start_cmpl.status != ESP_BT_STATUS_SUCCESS) {
            ESP_LOGE(BLE_GAP_TAG, "advertising start failed");
        } else {
            ESP_LOGI(BLE_GAP_TAG, "advertising start successfully");
        }
        break;
    case ESP_GAP_BLE_ADV_STOP_COMPLETE_EVT:
        if (param->adv_stop_cmpl.status != ESP_BT_STATUS_SUCCESS) {
            ESP_LOGE(BLE_GAP_TAG, "Advertising stop failed");
        } else {
            ESP_LOGI(BLE_GAP_TAG, "Stop adv successfully\n");
        }
        break;
    case ESP_GAP_BLE_UPDATE_CONN_PARAMS_EVT:
        ESP_LOGI(BLE_GAP_TAG,
                 "update connection params status = %d, min_int = %d, max_int = "
                 "%d,conn_int = "
                 "%d,latency = %d, timeout = %d",
                 param->update_conn_params.status, param->update_conn_params.min_int,
                 param->update_conn_params.max_int, param->update_conn_params.conn_int,
                 param->update_conn_params.latency, param->update_conn_params.timeout);
        break;
    default:
        break;
    }
}