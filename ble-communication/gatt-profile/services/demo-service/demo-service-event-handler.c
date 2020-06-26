#include "demo-service-event-handler.h"
#include "demo-service.h"
#include "esp_log.h"
#include "string.h"

#define BLE_DEMO_SERVICE_EH_TAG "BLE-DS-EH"

uint16_t demo_service_handles[DEMO_SERVICE_NB];
demo_service_cfg_t demo_service_cfg;

esp_gatt_status_t validate_characteristic_data(esp_ble_gatts_cb_param_t *param);

void demo_service_init(demo_service_cfg_t cfg, uint16_t **profile_handles_p,
                       esp_gatts_attr_db_t **service_gatt_db, uint8_t **uuid) {

    demo_service_cfg = cfg;
    *(profile_handles_p) = demo_service_handles;
    *(service_gatt_db) = demo_service_gatt_db;
    *(uuid) = DEMO_SERVICE_UUID;
}

void demo_service_event_handler(esp_gatts_cb_event_t event, esp_gatt_if_t gatts_if,
                                esp_ble_gatts_cb_param_t *param) {

    switch (event) {
    case ESP_GATTS_READ_EVT:
        /** Does the event need any action? */
        ESP_LOGI(BLE_DEMO_SERVICE_EH_TAG, "ESP_GATTS_READ_EVT");
        if (param->read.need_rsp) {
            ESP_LOGI(BLE_DEMO_SERVICE_EH_TAG, "Read needs rsp");
            esp_gatt_rsp_t rsp;

            rsp.attr_value.handle = param->read.handle;
            rsp.attr_value.len = sizeof(demo_service_data.assist_level);
            memcpy(rsp.attr_value.value, &demo_service_data.assist_level, rsp.attr_value.len);
            rsp.attr_value.offset = 0;

            esp_ble_gatts_send_response(gatts_if, param->read.conn_id, param->read.trans_id,
                                        ESP_GATT_OK, &rsp);
        }
        break;

        /* Write event is the most complex one, thus sometimes it'll need an extra file with
         * function definitions to help with the validation and management of the event */
    case ESP_GATTS_WRITE_EVT:
        ESP_LOGI(BLE_DEMO_SERVICE_EH_TAG, "ESP_GATTS_WRITE_EVT");
        if (!param->write.is_prep) {
            esp_gatt_status_t validation_status;
            esp_gatt_rsp_t rsp;
            rsp.handle = param->write.handle;
            /* If it's a write event, validate and update the data according to the rules of the
             * particular characteristic */
            validation_status = validate_characteristic_data(param);
            ESP_LOGI(BLE_DEMO_SERVICE_EH_TAG, "Validation result %d", validation_status);

            if (param->write.need_rsp) {
                rsp.attr_value.handle = param->write.handle;
                rsp.attr_value.len = sizeof(demo_service_data.assist_level);
                memcpy(rsp.attr_value.value, &demo_service_data.assist_level, rsp.attr_value.len);
                rsp.attr_value.offset = 0;

                esp_ble_gatts_send_response(gatts_if, param->write.conn_id, param->write.trans_id,
                                            validation_status, &rsp);
            }
        } else {
            /** "prepare write" event case. This is used when the characterstic might need more than
             * 20 bytes*/
        }
        break;

    case ESP_GATTS_SET_ATTR_VAL_EVT:
        if (demo_service_handles[IDX_ASSIST_LEVEL_VAL] == param->set_attr_val.attr_handle) {
            /* Check if indicates are enabled. TODO: Verify which byte correspond to indicate and
             * which to notify */
            if (demo_service_data.assist_level_ccc[CCCD_NOTIFY_BYTE] == 0x00) {
                return;
            }
            esp_ble_gatts_send_indicate(gatts_if, param->reg.app_id,
                                        demo_service_handles[IDX_ASSIST_LEVEL_VAL],
                                        demo_service_gatt_db[IDX_ASSIST_LEVEL_VAL].att_desc.length,
                                        (uint8_t *)&demo_service_data.assist_level, false);

        } else if (demo_service_handles[IDX_BATTERY_CHARGE_VAL] ==
                   param->set_attr_val.attr_handle) {
            if (demo_service_data.battery_charge_ccc[CCCD_NOTIFY_BYTE] == 0x00) {
                return;
            }

            esp_ble_gatts_send_indicate(
                gatts_if, param->reg.app_id, demo_service_handles[IDX_BATTERY_CHARGE_VAL],
                demo_service_gatt_db[IDX_BATTERY_CHARGE_VAL].att_desc.length,
                (uint8_t *)&demo_service_data.battery_charge, false);

        } else if (demo_service_handles[IDX_SPEED_VAL] == param->set_attr_val.attr_handle) {
            if (demo_service_data.speed_ccc[CCCD_NOTIFY_BYTE] == 0x00) {
                return;
            }

            esp_ble_gatts_send_indicate(gatts_if, param->reg.app_id,
                                        demo_service_handles[IDX_SPEED_VAL],
                                        demo_service_gatt_db[IDX_SPEED_VAL].att_desc.length,
                                        (uint8_t *)&demo_service_data.speed, false);
        }
        break;

    case ESP_GATTS_EXEC_WRITE_EVT:
        ESP_LOGI(BLE_DEMO_SERVICE_EH_TAG, "ESP_GATTS_EXEC_WRITE_EVT");
        break;
    case ESP_GATTS_CONF_EVT:
        ESP_LOGI(BLE_DEMO_SERVICE_EH_TAG, "ESP_GATTS_CONF_EVT");
        break;
    default:
        ESP_LOGI(BLE_DEMO_SERVICE_EH_TAG, "Event %d not implemented", event);
        break;
    }
    return;
}

esp_gatt_status_t validate_characteristic_data(esp_ble_gatts_cb_param_t *param) {
    esp_err_t res;

    if (param->write.handle == demo_service_handles[IDX_ASSIST_LEVEL_VAL]) {
        /* FIRST: Check proper lenght of the var. This might need changes if a var is more than 20
         * bytes since it would come in two separate write events. That case should be taken care in
         * the event handler with `if (!param->write.is_prep)` but I'm not sure of that yet.
         */

        if (param->write.len != demo_service_gatt_db[IDX_ASSIST_LEVEL_VAL].att_desc.length) {
            return ESP_GATT_INVALID_ATTR_LEN;
        }

        res = demo_service_cfg.assist_level_write_check(*(param->write.value));
        if (res != ESP_OK) {
            return ESP_GATT_OUT_OF_RANGE;
        }

        demo_service_data.assist_level = param->write.value[0];
        /* else if (a_different_char) ...*/
        /* finally, characteristics configuration if needed*/
    } else if (param->write.handle == demo_service_handles[IDX_ASSIST_LEVEL_CFG]) {
        demo_service_data.assist_level_ccc[CCCD_INDICATE_BYTE] =
            param->write.value[CCCD_INDICATE_BYTE];
        demo_service_data.assist_level_ccc[CCCD_NOTIFY_BYTE] = param->write.value[CCCD_NOTIFY_BYTE];
    } else if (param->write.handle == demo_service_handles[IDX_BATTERY_CHARGE_CFG]) {
        demo_service_data.battery_charge_ccc[CCCD_INDICATE_BYTE] =
            param->write.value[CCCD_INDICATE_BYTE];
        demo_service_data.battery_charge_ccc[CCCD_NOTIFY_BYTE] =
            param->write.value[CCCD_NOTIFY_BYTE];
    } else if (param->write.handle == demo_service_handles[IDX_SPEED_CFG]) {
        demo_service_data.speed_ccc[CCCD_INDICATE_BYTE] = param->write.value[CCCD_INDICATE_BYTE];
        demo_service_data.speed_ccc[CCCD_NOTIFY_BYTE] = param->write.value[CCCD_NOTIFY_BYTE];
    } else {
        // Security check. In case the validation of a characteristic is missing
        return ESP_GATT_UNKNOWN_ERROR;
    }

    return ESP_GATT_OK;
}
