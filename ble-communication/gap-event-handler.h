/**
 * @file gap-event-handler.h
 * @brief Implementation of the handler for GAP events
 */

#ifndef BLE_GA_EVENT_HANDLER_H_
#define BLE_GA_EVENT_HANDLER_H_
#include <esp_bt_defs.h>
#include <esp_gap_ble_api.h>

/**
 * @brief Handler for GAP related events.
 *
 * @param event Enum to specify the event.
 * @param param Depends on the event.
 */
void gap_event_handler(esp_gap_ble_cb_event_t event, esp_ble_gap_cb_param_t *param);

#endif /** BLE_GA_EVENT_HANDLER_H_ */