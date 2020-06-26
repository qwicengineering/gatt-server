/**
 * @file dashboard-service-database.h
 * @brief This file contains the functions to manage the different events related to the service and
 * the initialization function.
 *
 */
#ifndef BLE_DASHBOARD_SERVICE_EVENT_HANDLER_H_
#define BLE_DASHBOARD_SERVICE_EVENT_HANDLER_H_

#include "dashboard-service-database.h"
#include "dashboard-service.h"
#include "esp_gatts_api.h"

/**
 * @brief  Handles for the dashboard service table. Used in the dashboard-service.c file to grant
 * access to the values.
 *
 */
extern uint16_t dashboard_service_handles[DASHBOARD_SERVICE_NB];

/**
 * @brief Saves the config for the service and returns the handles table.
 *
 * @param cfg configuration structure for the service
 * @param profile_handles_p Is used to return a poinetr to the service handles table
 * @param gatts_if To which GATT server access interface attach this service
 */
void dashboard_service_init(dashboard_service_cfg_t cfg, uint16_t **profile_handles_p,
                            esp_gatts_attr_db_t **service_gatt_db, uint8_t **uuid);

/**
 * @brief Event handler for this service. Uses the same signature as the gatt event handler for
 * simplicity but it should receive only the events related to this specific service or the ones
 * that affect all of them.
 *
 * @param event
 * @param gatts_if
 * @param param
 */
void dashboard_service_event_handler(esp_gatts_cb_event_t event, esp_gatt_if_t gatts_if,
                                     esp_ble_gatts_cb_param_t *param);

#endif /* BLE_DASHBOARD_SERVICE_EVENT_HANDLER_H_ */