#include "template-service-event-handler.h"
#include "esp_log.h"
#include "string.h"
#include "template-service.h"

#define BLE_TEMPLATE_SERVICE_EH_TAG "BLE-DS-EH"

uint16_t service_handles[TEMPLATE_SERVICE_NB];
template_service_cfg_t service_cfg;

void template_service_init(template_service_cfg_t cfg, uint16_t **profile_handles_p,
                           esp_gatts_attr_db_t **service_gatt_db, uint8_t **uuid) {

    service_cfg = cfg;
    *(profile_handles_p) = service_handles;
    *(service_gatt_db) = template_service_gatt_db;
    *(uuid) = TEMPLATE_SERVICE_UUID;
}

void template_service_event_handler(esp_gatts_cb_event_t event, esp_gatt_if_t gatts_if,
                                    esp_ble_gatts_cb_param_t *param) {

    switch (event) {
    case ESP_GATTS_READ_EVT: /** Do you need to send manual responses or do anything else on read?*/
    case ESP_GATTS_WRITE_EVT: /** Do you need to check values, send manual responses or receive data
                                 longer than 20 bytes? */
    case ESP_GATTS_SET_ATTR_VAL_EVT: /** Do you need to send notifications? */
    case ESP_GATTS_EXEC_WRITE_EVT:   /** Do you receive data that is longer than 20 bytes? */
    case ESP_GATTS_CONF_EVT:         /** Do you need to do anything on ACKs? */
    default:
        ESP_LOGI(BLE_TEMPLATE_SERVICE_EH_TAG, "Event %d not implemented", event);
        break;
    }
    return;
}
