/**
 * @file gatt-profile.h
 * @brief Implementation of a GATT profile with multiple services
 *
 */

#ifndef BLE_GATT_PROFILE_H_
#define BLE_GATT_PROFILE_H_

#include "../ble-configuration.h"
#include "gatt-profile-api.h"
#include <esp_gatts_api.h>

#define NUMBER_OF_SERVICES PROF_IDX_NB /*< Number of services installed on the profile */

/**
 * @brief Services list. Used as indexes when creating the profile table.
 *
 */
enum {
    PROF_DS_IDX, /*< Dashboard service */

    PROF_IDX_NB, /*< Size of the profile table since the enum is 0-based */
};

/**
 * @brief Entry for the profile table.
 *
 * @param gatts_cb Event handler for the service.
 * @param gatts_if Default Gatt interface for unregistered services, it's changed on service
 *registration.
 * @param db Pointer to the service database
 * @param db_size Expected Size of the handles_table. Used to check that the table is created.
 * @param handles_table_p Pointer to the table with thehandles of the service.
 * @param app_id Keep the service id at hand to differenciate at callbacs.
 **/
typedef struct {
    esp_gatts_cb_t gatts_cb;
    uint16_t gatts_if;
    esp_gatts_attr_db_t *db;
    uint8_t db_size;
    uint16_t *handles_table_p;
    uint16_t app_id;
    uint8_t svc_id;
    uint8_t *svc_uuid_p;
} gatts_profile_inst_t;

/**
 * @brief Initializes the ble parameters.
 *
 * @param config depndes on which services are configured.
 */
void gatt_profile_init(gatt_profile_config_t config);

/**
 * @brief Handler for the GATT events. Follows the signature required by the bluetooth driver.
 *
 * @param event  Identifies the type of event.
 * @param gatts_if Identifies the service the event is related to.
 * @param param  Depends on the type of event.
 */
void gatts_event_handler(esp_gatts_cb_event_t event, esp_gatt_if_t gatts_if,
                         esp_ble_gatts_cb_param_t *param);

#endif /** BLE_GATT_PROFILE_H_ */