#include "gatt-profile.h"
#include "services/services-declarations.h"

// You need the database and event handler for each of the services
#include "services/dashboard-service/dashboard-service-database.h"
#include "services/dashboard-service/dashboard-service-event-handler.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <esp_err.h>
#include <esp_log.h>

#define GATT_PROFILE_TAG "BLE-GATT"
#define SVC_IDX 0 /*< Index of the service definition characteristic within the service table */

static esp_err_t gatt_reg_evt(esp_gatt_if_t gatts_if, esp_ble_gatts_cb_param_t *param);
static esp_err_t gatt_create_attr_tabl_evt(esp_gatt_if_t gatts_if, esp_ble_gatts_cb_param_t *param);
static esp_err_t gatt_client_connect_evt(esp_ble_gatts_cb_param_t *param);
static esp_err_t gatt_client_disconnect_evt(esp_ble_gatts_cb_param_t *param);
static esp_err_t gatt_forward_to_service(esp_gatts_cb_event_t event, esp_gatt_if_t gatts_if,
                                         esp_ble_gatts_cb_param_t *param);

gatt_profile_config_t gatt_cfg;
ble_connection_status_t client_connection = {.status = BLE_CLIENT_DISCONNECTED};

uint8_t advertised_service[ESP_UUID_LEN_128];
esp_gatt_if_t profile_gatts_if; /*< Not in use since we only have one profile at the moment */

/* Profile table. You need to populate with the data of each service. Also, remember to update the
 * enum in the .h to properly reflect the table size */
gatts_profile_inst_t gatt_profile_tab[PROF_IDX_NB] = {
    [PROF_DS_IDX] = {.gatts_cb = dashboard_service_event_handler,
                     .gatts_if = ESP_GATT_IF_NONE,
                     .db_size = DASHBOARD_SERVICE_NB,
                     .handles_table_p = NULL,
                     .app_id = BLE_APP_ID,
                     .svc_id = PROF_DS_IDX,
                     .svc_uuid_p = NULL},
};

ble_connection_status_t ble_get_conn_status() { return client_connection; };

/* If you want to set custom advertising info this would probably be the right place */
void gatt_profile_init(gatt_profile_config_t cfg) {
    gatt_cfg = cfg;

    /** Decide which service to advertise */
    memcpy(advertised_service, DASHBOARD_SERVICE_UUID,
           ESP_UUID_LEN_128 * sizeof(DASHBOARD_SERVICE_UUID[0]));

    adv_data.p_service_uuid = advertised_service;
    adv_data.service_uuid_len = sizeof(advertised_service);

    scan_rsp_data.p_service_uuid = advertised_service;
    scan_rsp_data.service_uuid_len = sizeof(advertised_service);

    /** Initialize each service */
    dashboard_service_init(gatt_cfg.ds_cfg, &(gatt_profile_tab[PROF_DS_IDX].handles_table_p),
                           &(gatt_profile_tab[PROF_DS_IDX].db),
                           &(gatt_profile_tab[PROF_DS_IDX].svc_uuid_p));
}

void gatts_event_handler(esp_gatts_cb_event_t event, esp_gatt_if_t gatts_if,
                         esp_ble_gatts_cb_param_t *param) {
    esp_err_t ret = ESP_OK;

    switch (event) {
    case ESP_GATTS_REG_EVT:
        /** BLE app registered: Create service tables and set advertising data */
        ret = gatt_reg_evt(gatts_if, param);
        break;
    case ESP_GATTS_CREAT_ATTR_TAB_EVT:
        /** Service table created: Copy characteristic handles and start the service */
        ret = gatt_create_attr_tabl_evt(gatts_if, param);
        break;
    case ESP_GATTS_CONNECT_EVT:
        /** Connected to client: Update status */
        ret = gatt_client_connect_evt(param);
        break;
    case ESP_GATTS_DISCONNECT_EVT:
        /** Disconnected from client: Update status and restart advertising */
        ret = gatt_client_disconnect_evt(param);
        break;
    case ESP_GATTS_START_EVT:
        /** Service started. Nothing to do. param->service_handle has the handle to the service */
        break;
    case ESP_GATTS_READ_EVT:
    case ESP_GATTS_WRITE_EVT:
    case ESP_GATTS_EXEC_WRITE_EVT:
    case ESP_GATTS_SET_ATTR_VAL_EVT:
    case ESP_GATTS_CONF_EVT:
        /** Read, Write, multi-messages write and change in characteristic value are handled
         * individually by each service */
        ret = gatt_forward_to_service(event, gatts_if, param);
        break;

    default:
        ESP_LOGE(GATT_PROFILE_TAG, "Event %d not implemented", event);
        break;
    }

    if (ret != ESP_OK) {
        ESP_LOGE(GATT_PROFILE_TAG, "Event (%d) finished with error code %d", event, ret);
    }
}

/************************************************************************************
 *                                                                                  *
 *                     Auxiliar functions for the event handler                     *
 *                                                                                  *
 ************************************************************************************/

esp_err_t gatt_reg_evt(esp_gatt_if_t gatts_if, esp_ble_gatts_cb_param_t *param) {
    esp_err_t ret = ESP_OK;

    /* App is registered, store the gatts_if for future events */
    profile_gatts_if = gatts_if;

    if (param->reg.status != ESP_GATT_OK) {
        /** If there was a registration error avoid creating the services tables */
        ESP_LOGE(GATT_PROFILE_TAG, "Reg app failed, app_id %04x, status %d\n", param->reg.app_id,
                 param->reg.status);
        return ret;
    }

    /** Create the attribute table of each service. If all tables are correct then set the
     * advertising data. */
    for (uint8_t i = 0; i < NUMBER_OF_SERVICES; i++) {
        ret = esp_ble_gatts_create_attr_tab(gatt_profile_tab[i].db, profile_gatts_if,
                                            gatt_profile_tab[i].db_size, SVC_INST_ID);
        if (ret != ESP_OK) {
            ESP_LOGE(GATT_PROFILE_TAG, "Create attr for profile %d failed, error code = %x", i,
                     ret);
            return ret;
        }
    }
    ESP_LOGI(GATT_PROFILE_TAG, "All services registered");

    ret = esp_ble_gap_config_adv_data(&adv_data);
    if (ret != ESP_OK) {
        ESP_LOGE(GATT_PROFILE_TAG, "Advertising config failed, error code = %x", ret);
        return ret;
    }

    ret = esp_ble_gap_config_adv_data(&scan_rsp_data);
    if (ret != ESP_OK) {
        ESP_LOGE(GATT_PROFILE_TAG, "Scan response config failed, error code = %x", ret);
        return ret;
    }

    return ret;
}

esp_err_t gatt_create_attr_tabl_evt(esp_gatt_if_t gatts_if, esp_ble_gatts_cb_param_t *param) {
    esp_err_t ret = ESP_OK;
    uint8_t svc_idx = 0;

    /** First ientify which service is this event about */
    for (uint8_t idx = 0; idx < NUMBER_OF_SERVICES; idx++) {
        /** For that we compare the two unique bytes of the service UUID. */
        if (param->add_attr_tab.svc_uuid.uuid.uuid128[CUSTOM_UUID_BYTE_1_POS] ==
                gatt_profile_tab[idx].svc_uuid_p[CUSTOM_UUID_BYTE_1_POS] &&
            param->add_attr_tab.svc_uuid.uuid.uuid128[CUSTOM_UUID_BYTE_2_POS] ==
                gatt_profile_tab[idx].svc_uuid_p[CUSTOM_UUID_BYTE_2_POS]) {
            svc_idx = idx;
            break;
        }
    }

    /** Check for errors in the service table creation **/
    if (param->add_attr_tab.status != ESP_GATT_OK) {
        ESP_LOGE(GATT_PROFILE_TAG, "Service %d create attribute table failed, error code=0x%x",
                 param->reg.app_id, param->add_attr_tab.status);
        ret = ESP_ERR_INVALID_STATE;
    } else if (param->add_attr_tab.num_handle != gatt_profile_tab[svc_idx].db_size) {
        ESP_LOGE(
            GATT_PROFILE_TAG,
            "Attribute table created abnormally: num_handle (%d) is expected to be S_IDX_NB(%d)",
            param->add_attr_tab.num_handle, gatt_profile_tab[svc_idx].db_size);
        ret = ESP_ERR_INVALID_STATE;
    } else {
        /** If it was created correctly: Save a copy of the handles, and start the service.*/
        memcpy(gatt_profile_tab[svc_idx].handles_table_p, param->add_attr_tab.handles,
               sizeof(param->add_attr_tab.handles[0]) * gatt_profile_tab[svc_idx].db_size);

        ret = esp_ble_gatts_start_service(gatt_profile_tab[svc_idx].handles_table_p[SVC_IDX]);
    }

    return ret;
}

esp_err_t gatt_client_connect_evt(esp_ble_gatts_cb_param_t *param) {
    ESP_LOGI(GATT_PROFILE_TAG, "Connected to client. Conn_id = %d", param->connect.conn_id);

    /** Save remote address */
    memcpy(connection_params.bda, param->connect.remote_bda, sizeof(esp_bd_addr_t));

    /** Update GAP data */
    esp_ble_gap_update_conn_params(&connection_params);

    /** Update public access data */
    client_connection.status = BLE_CLIENT_CONNECTED;
    memcpy(client_connection.client_add, param->connect.remote_bda, sizeof(esp_bd_addr_t));
    client_connection.conn_id = param->connect.conn_id;

    return ESP_OK;
}

esp_err_t gatt_client_disconnect_evt(esp_ble_gatts_cb_param_t *param) {
    ESP_LOGI(GATT_PROFILE_TAG, "Disconnected from client. Reason = 0x%x", param->disconnect.reason);

    /** Restart GAP advertising */
    esp_ble_gap_start_advertising(&adv_params);

    /** Update public access data */
    client_connection.status = BLE_CLIENT_DISCONNECTED;

    return ESP_OK;
}

esp_err_t gatt_forward_to_service(esp_gatts_cb_event_t event, esp_gatt_if_t gatts_if,
                                  esp_ble_gatts_cb_param_t *param) {
    uint16_t attr_handle = 0;

    if (event == ESP_GATTS_EXEC_WRITE_EVT) {
        ESP_LOGE(GATT_PROFILE_TAG, "ESP_GATTS_EXEC_WRITE_EVT not implemented");
        return ESP_ERR_NOT_SUPPORTED;
    }

    if (event == ESP_GATTS_SET_ATTR_VAL_EVT) {
        attr_handle = param->set_attr_val.attr_handle;
    } else if (event == ESP_GATTS_READ_EVT) {
        attr_handle = param->read.handle;
    } else if (event == ESP_GATTS_WRITE_EVT) {
        attr_handle = param->write.handle;
    } else if (event == ESP_GATTS_CONF_EVT) {
        attr_handle = param->conf.handle;
    }

    /** Loop through the services */
    for (uint8_t idx = 0; idx < NUMBER_OF_SERVICES; idx++) {
        uint16_t from = gatt_profile_tab[idx].handles_table_p[0];
        uint16_t to = gatt_profile_tab[idx].handles_table_p[gatt_profile_tab[idx].db_size - 1];

        /** Check if this one contains that handle */
        if (attr_handle >= from && attr_handle <= to) {
            /** If so, call it's internal event handler */
            gatt_profile_tab[idx].gatts_cb(event, gatts_if, param);
            break;
        }
    }
    return ESP_OK;
}