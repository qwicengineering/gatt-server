#include "dashboard-service-event-handler.h"
#include "dashboard-service.h"
#include "esp_log.h"
#include "string.h"

#define BLE_DASHBOARD_SERVICE_EH_TAG "BLE-DS-EH"

uint16_t dashboard_service_handles[DASHBOARD_SERVICE_NB];
dashboard_service_cfg_t service_cfg;

static void set_attr_evt(esp_gatt_if_t gatts_if, esp_ble_gatts_cb_param_t *param);
static esp_gatt_status_t write_evt(esp_gatt_if_t gatts_if, esp_ble_gatts_cb_param_t *param,
                                   esp_gatt_rsp_t *rsp);
void read_evt(esp_gatt_if_t gatts_if, esp_ble_gatts_cb_param_t *param, esp_gatt_rsp_t *rsp);

void dashboard_service_init(dashboard_service_cfg_t cfg, uint16_t **profile_handles_p,
                            esp_gatts_attr_db_t **service_gatt_db, uint8_t **uuid) {

    service_cfg = cfg;
    *(profile_handles_p) = dashboard_service_handles;
    *(service_gatt_db) = dashboard_service_gatt_db;
    *(uuid) = DASHBOARD_SERVICE_UUID;
}

void dashboard_service_event_handler(esp_gatts_cb_event_t event, esp_gatt_if_t gatts_if,
                                     esp_ble_gatts_cb_param_t *param) {

    switch (event) {
    case ESP_GATTS_READ_EVT: /** Do you need to send manual responses or do anything else on read?*/
        if (param->read.need_rsp) {
            esp_gatt_rsp_t rsp;

            read_evt(gatts_if, param, &rsp);
            ESP_LOGI(BLE_DASHBOARD_SERVICE_EH_TAG, "%d, %d", rsp.attr_value.len,
                     rsp.attr_value.value[0]);

            esp_ble_gatts_send_response(gatts_if, param->read.conn_id, param->read.trans_id,
                                        ESP_GATT_OK, &rsp);
        }
        break;
    case ESP_GATTS_WRITE_EVT:;
        esp_gatt_rsp_t rsp;
        esp_gatt_status_t validation_result = write_evt(gatts_if, param, &rsp);

        if (param->write.need_rsp) {
            rsp.handle = param->write.handle;
            esp_ble_gatts_send_response(gatts_if, param->write.conn_id, param->write.trans_id,
                                        validation_result, &rsp);
        }
        break;
    case ESP_GATTS_SET_ATTR_VAL_EVT: /** Do you need to send notifications */
        set_attr_evt(gatts_if, param);
        break;
    case ESP_GATTS_CONF_EVT: /** Do you need to do anything on ACKs? */
        // ESP_LOGI(BLE_DASHBOARD_SERVICE_EH_TAG, "Ack");
    case ESP_GATTS_EXEC_WRITE_EVT: /** Do you have writes longer than 20 bytes */
        break;
    default:
        ESP_LOGI(BLE_DASHBOARD_SERVICE_EH_TAG, "Event %d not implemented", event);
        break;
    }
    return;
}

static void set_attr_evt(esp_gatt_if_t gatts_if, esp_ble_gatts_cb_param_t *param) {
    uint8_t needs_confirmation;
    uint8_t *val_p;
    uint16_t handle = param->set_attr_val.attr_handle;
    uint16_t length = 0;
    /** Some chars need confirmation, some chars have ccc field */

    ESP_LOGI(BLE_DASHBOARD_SERVICE_EH_TAG, "Attribute changed. Handle %d", handle);
    ESP_LOGI(BLE_DASHBOARD_SERVICE_EH_TAG,
             "CCC status: speed_ccc = 0x%x, 0x%x, pedal_cadence_ccc = 0x%x, 0x%x, "
             "battery_charge_ccc = 0x%x, 0x%x, pedal_power_ccc = 0x%x, 0x%x, calories_ccc = 0x%x, "
             "0x%x, odometer_ccc = 0x%x, 0x%x",
             dashboard_service_data.speed_ccc[CCCD_NOTIFY_BYTE],
             dashboard_service_data.speed_ccc[CCCD_INDICATE_BYTE],
             dashboard_service_data.pedal_cadence_ccc[CCCD_NOTIFY_BYTE],
             dashboard_service_data.pedal_cadence_ccc[CCCD_INDICATE_BYTE],
             dashboard_service_data.battery_charge_ccc[CCCD_NOTIFY_BYTE],
             dashboard_service_data.battery_charge_ccc[CCCD_INDICATE_BYTE],
             dashboard_service_data.pedal_power_ccc[CCCD_NOTIFY_BYTE],
             dashboard_service_data.pedal_power_ccc[CCCD_INDICATE_BYTE],
             dashboard_service_data.calories_ccc[CCCD_NOTIFY_BYTE],
             dashboard_service_data.calories_ccc[CCCD_INDICATE_BYTE],
             dashboard_service_data.odometer_ccc[CCCD_NOTIFY_BYTE],
             dashboard_service_data.odometer_ccc[CCCD_INDICATE_BYTE]);

    if ((handle == dashboard_service_handles[IDX_DS_SPEED_VAL]) &&
        (dashboard_service_data.speed_ccc[CCCD_NOTIFY_BYTE])) {
        needs_confirmation = true;
        val_p = (uint8_t *)&dashboard_service_data.speed;
        length = dashboard_service_gatt_db[IDX_DS_SPEED_VAL].att_desc.length;

    } else if ((handle == dashboard_service_handles[IDX_DS_PEDAL_CADENCE_VAL]) &&
               (dashboard_service_data.pedal_cadence_ccc[CCCD_NOTIFY_BYTE])) {
        needs_confirmation = true;
        val_p = (uint8_t *)&dashboard_service_data.pedal_cadence;
        length = dashboard_service_gatt_db[IDX_DS_PEDAL_CADENCE_VAL].att_desc.length;
    } else if ((handle == dashboard_service_handles[IDX_DS_BATTERY_CHARGE_VAL]) &&
               (dashboard_service_data.battery_charge_ccc[CCCD_NOTIFY_BYTE])) {
        needs_confirmation = true;
        val_p = (uint8_t *)&dashboard_service_data.battery_charge;
        length = dashboard_service_gatt_db[IDX_DS_BATTERY_CHARGE_VAL].att_desc.length;

    } else if ((handle == dashboard_service_handles[IDX_DS_PEDAL_POWER_VAL]) &&
               (dashboard_service_data.pedal_power_ccc[CCCD_NOTIFY_BYTE])) {
        needs_confirmation = true;
        val_p = (uint8_t *)&dashboard_service_data.pedal_power;
        length = dashboard_service_gatt_db[IDX_DS_PEDAL_POWER_VAL].att_desc.length;

    } else if ((handle == dashboard_service_handles[IDX_DS_CALORIES_VAL]) &&
               (dashboard_service_data.calories_ccc[CCCD_NOTIFY_BYTE])) {
        needs_confirmation = true;
        val_p = (uint8_t *)&dashboard_service_data.calories;
        length = dashboard_service_gatt_db[IDX_DS_CALORIES_VAL].att_desc.length;

    } else if ((handle == dashboard_service_handles[IDX_DS_ODOMETER_VAL]) &&
               (dashboard_service_data.odometer_ccc[CCCD_NOTIFY_BYTE])) {
        needs_confirmation = true;
        val_p = (uint8_t *)&dashboard_service_data.odometer;
        length = dashboard_service_gatt_db[IDX_DS_ODOMETER_VAL].att_desc.length;

    } else if ((handle == dashboard_service_handles[IDX_DS_FRONT_LIGHT_VAL])) {
        needs_confirmation = false;
        val_p = (uint8_t *)&dashboard_service_data.front_light;
        length = dashboard_service_gatt_db[IDX_DS_FRONT_LIGHT_VAL].att_desc.length;

    } else if ((handle == dashboard_service_handles[IDX_DS_WALK_ASSIST_VAL])) {
        needs_confirmation = false;
        val_p = (uint8_t *)&dashboard_service_data.walk_assist;
        length = dashboard_service_gatt_db[IDX_DS_WALK_ASSIST_VAL].att_desc.length;

    } else if ((handle == dashboard_service_handles[IDX_DS_SUPPORT_LEVEL_VAL])) {
        needs_confirmation = false;
        val_p = (uint8_t *)&dashboard_service_data.support_level;
        length = dashboard_service_gatt_db[IDX_DS_SUPPORT_LEVEL_VAL].att_desc.length;

    } else if ((handle == dashboard_service_handles[IDX_DS_CONSOLE_STATUS_VAL])) {
        needs_confirmation = false;
        val_p = (uint8_t *)&dashboard_service_data.console_status;
        length = dashboard_service_gatt_db[IDX_DS_CONSOLE_STATUS_VAL].att_desc.length;

    } else if ((handle == dashboard_service_handles[IDX_DS_TRIP_INITIAL_ODOMETER_VAL])) {
        needs_confirmation = false;
        val_p = (uint8_t *)&dashboard_service_data.trip_initial_odometer;
        length = dashboard_service_gatt_db[IDX_DS_TRIP_INITIAL_ODOMETER_VAL].att_desc.length;

    } else if ((handle == dashboard_service_handles[IDX_DS_TRIP_INITIAL_TIME_VAL])) {
        needs_confirmation = false;
        val_p = (uint8_t *)&dashboard_service_data.trip_initial_time;
        length = dashboard_service_gatt_db[IDX_DS_TRIP_INITIAL_TIME_VAL].att_desc.length;

    } else {
        /** If it was none of those chars, or the ccc is disabled dont send the notification */
        return;
    }

    esp_ble_gatts_send_indicate(gatts_if, param->reg.app_id, handle, length, val_p,
                                needs_confirmation);

    return;
}

esp_gatt_status_t write_evt(esp_gatt_if_t gatts_if, esp_ble_gatts_cb_param_t *param,
                            esp_gatt_rsp_t *rsp) {
    esp_err_t validation = ESP_OK;
    esp_gatt_status_t ret = ESP_GATT_OK;
    cccd_t *dest_cfg = NULL;

    rsp->attr_value.handle = param->read.handle;
    rsp->attr_value.offset = 0;

    if (param->write.handle == dashboard_service_handles[IDX_DS_SUPPORT_LEVEL_VAL]) {
        validation = service_cfg.support_level_write_cb(*(param->write.value));
        if (validation == ESP_OK) {
            dashboard_service_data.support_level = (uint8_t)*param->write.value;
        }
        rsp->attr_value.len = sizeof(dashboard_service_data.support_level);
        memcpy(rsp->attr_value.value, &dashboard_service_data.support_level, rsp->attr_value.len);

    } else if (param->write.handle == dashboard_service_handles[IDX_DS_FRONT_LIGHT_VAL]) {
        validation = service_cfg.front_light_write_cb(*(param->write.value));
        if (validation == ESP_OK) {
            dashboard_service_data.front_light = (uint8_t)*param->write.value;
        }
        rsp->attr_value.len = sizeof(dashboard_service_data.front_light);
        memcpy(rsp->attr_value.value, &dashboard_service_data.front_light, rsp->attr_value.len);

    } else if (param->write.handle == dashboard_service_handles[IDX_DS_TIME_VAL]) {
        validation = service_cfg.time_write_cb(*(param->write.value));
        if (validation == ESP_OK) {
            dashboard_service_data.time = (uint32_t)*param->write.value;
        }
        rsp->attr_value.len = sizeof(dashboard_service_data.time);
        memcpy(rsp->attr_value.value, &dashboard_service_data.time, rsp->attr_value.len);

    } else if (param->write.handle == dashboard_service_handles[IDX_DS_SPEED_CFG]) {
        dest_cfg = &(dashboard_service_data.speed_ccc);
        rsp->attr_value.len = sizeof(cccd_t);
        memcpy(rsp->attr_value.value, &dashboard_service_data.speed_ccc, rsp->attr_value.len);
    } else if (param->write.handle == dashboard_service_handles[IDX_DS_PEDAL_CADENCE_CFG]) {
        dest_cfg = &(dashboard_service_data.pedal_cadence_ccc);
        rsp->attr_value.len = sizeof(cccd_t);
        memcpy(rsp->attr_value.value, &dashboard_service_data.pedal_cadence_ccc,
               rsp->attr_value.len);
    } else if (param->write.handle == dashboard_service_handles[IDX_DS_BATTERY_CHARGE_CFG]) {
        dest_cfg = &(dashboard_service_data.battery_charge_ccc);
        rsp->attr_value.len = sizeof(cccd_t);
        memcpy(rsp->attr_value.value, &dashboard_service_data.battery_charge_ccc,
               rsp->attr_value.len);
    } else if (param->write.handle == dashboard_service_handles[IDX_DS_PEDAL_POWER_CFG]) {
        dest_cfg = &(dashboard_service_data.pedal_power_ccc);
        rsp->attr_value.len = sizeof(cccd_t);
        memcpy(rsp->attr_value.value, &dashboard_service_data.pedal_power_ccc, rsp->attr_value.len);
    } else if (param->write.handle == dashboard_service_handles[IDX_DS_CALORIES_CFG]) {
        dest_cfg = &(dashboard_service_data.calories_ccc);
        rsp->attr_value.len = sizeof(cccd_t);
        memcpy(rsp->attr_value.value, &dashboard_service_data.calories_ccc, rsp->attr_value.len);
    } else if (param->write.handle == dashboard_service_handles[IDX_DS_ODOMETER_CFG]) {
        dest_cfg = &(dashboard_service_data.odometer_ccc);
        rsp->attr_value.len = sizeof(cccd_t);
        memcpy(rsp->attr_value.value, &dashboard_service_data.odometer_ccc, rsp->attr_value.len);
    }

    if (validation != ESP_OK) {
        ret = ESP_GATT_OUT_OF_RANGE;
    }
    if (dest_cfg != NULL) {
        *dest_cfg[CCCD_NOTIFY_BYTE] = param->write.value[CCCD_NOTIFY_BYTE];
        *dest_cfg[CCCD_INDICATE_BYTE] = param->write.value[CCCD_INDICATE_BYTE];
    }

    return ret;
}

void read_evt(esp_gatt_if_t gatts_if, esp_ble_gatts_cb_param_t *param, esp_gatt_rsp_t *rsp) {
    rsp->attr_value.handle = param->read.handle;
    rsp->attr_value.offset = 0;

    if (param->write.handle == dashboard_service_handles[IDX_DS_SUPPORT_LEVEL_VAL]) {

        rsp->attr_value.len = sizeof(dashboard_service_data.support_level);
        memcpy(rsp->attr_value.value, &dashboard_service_data.support_level, rsp->attr_value.len);

    } else if (param->write.handle == dashboard_service_handles[IDX_DS_FRONT_LIGHT_VAL]) {
        rsp->attr_value.len = sizeof(dashboard_service_data.front_light);
        memcpy(rsp->attr_value.value, &dashboard_service_data.front_light, rsp->attr_value.len);

    } else if (param->write.handle == dashboard_service_handles[IDX_DS_TIME_VAL]) {
        rsp->attr_value.len = sizeof(dashboard_service_data.time);
        memcpy(rsp->attr_value.value, &dashboard_service_data.time, rsp->attr_value.len);
    }

    return;
}