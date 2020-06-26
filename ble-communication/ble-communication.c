#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"
#include "freertos/task.h"

#include "esp_log.h"
#include "nvs.h"
#include "nvs_flash.h"

#include "esp_bt.h"
#include "esp_bt_main.h"
#include "esp_gatt_common_api.h"

#include "ble-communication.h"

#include "ble-configuration.h"
#include "gap-event-handler.h"
#include "gatt-profile/gatt-profile.h"

#define BLE_COMMUNICATION_TAG "BLE-main"

ble_communication_config_t cfg;

esp_err_t ble_communication_init(ble_communication_config_t ble_config) {
    esp_err_t ret;
    cfg = ble_config;

    /* Initialize NVS. */
    ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }

    /* Disable bluetooth classic */
    ESP_ERROR_CHECK(esp_bt_controller_mem_release(ESP_BT_MODE_CLASSIC_BT));

    ret = esp_bt_controller_init(&bt_controller_cfg);
    if (ret != ESP_OK) {
        ESP_LOGE(BLE_COMMUNICATION_TAG, "%s enable controller failed: %s", __func__,
                 esp_err_to_name(ret));
        return ret;
    }

    ret = esp_bt_controller_enable(ESP_BT_MODE_BLE);
    if (ret != ESP_OK) {
        ESP_LOGE(BLE_COMMUNICATION_TAG, "%s enable controller failed: %s", __func__,
                 esp_err_to_name(ret));
        return ret;
    }

    ret = esp_bluedroid_init();
    if (ret != ESP_OK) {
        ESP_LOGE(BLE_COMMUNICATION_TAG, "%s init bluetooth failed: %s", __func__,
                 esp_err_to_name(ret));
        return ret;
    }

    ret = esp_bluedroid_enable();
    if (ret != ESP_OK) {
        ESP_LOGE(BLE_COMMUNICATION_TAG, "%s enable bluetooth failed: %s", __func__,
                 esp_err_to_name(ret));
        return ret;
    }

    ESP_LOGI(BLE_COMMUNICATION_TAG, "Ble name: %s", cfg.device_name);
    ret = esp_ble_gap_set_device_name(cfg.device_name);
    if (ret != ESP_OK) {
        ESP_LOGE(BLE_COMMUNICATION_TAG, "Set device name failed. Error code = %x", ret);
        return ret;
    }

    gatt_profile_init(cfg.gatt_config);
    ret = esp_ble_gatts_register_callback(gatts_event_handler);
    if (ret != ESP_OK) {
        ESP_LOGE(BLE_COMMUNICATION_TAG, "gatts register error. Error code = %x", ret);
        return ret;
    }

    ret = esp_ble_gap_register_callback(gap_event_handler);
    if (ret != ESP_OK) {
        ESP_LOGE(BLE_COMMUNICATION_TAG, "gap register error. Error code = %x", ret);
        return ret;
    }

    ret = esp_ble_gap_config_local_privacy(true);
    if (ret != ESP_OK) {
        ESP_LOGE(BLE_COMMUNICATION_TAG, "config local privacy error. Error code = %x", ret);
        return ret;
    }

    ret = esp_ble_gatts_app_register(BLE_APP_ID);
    if (ret != ESP_OK) {
        ESP_LOGE(BLE_COMMUNICATION_TAG, "gatts battery app register error. Error code = %x", ret);
        return ret;
    }

    ret = esp_ble_gatt_set_local_mtu(BLE_LOCAL_MTU);
    if (ret) {
        ESP_LOGE(BLE_COMMUNICATION_TAG, "set local  MTU failed, error code = %x", ret);
    }

    return ret;
}
