/**
 * @file main.c
 * @brief This is an example used to test the the dashboard service along the static data service.
 * @version 0.1
 *
 */
#include "esp_err.h"
#include "esp_log.h"
#include "sdkconfig.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include <stdio.h>

#include "ble-communication.h"

#define STACK_DEPTH 4096
#define TASK_PRIORITY 5

const uint32_t half_second_ticks = 500 / portTICK_RATE_MS;
const uint32_t one_second_ticks = 1000 / portTICK_RATE_MS;
const uint32_t two_seconds_ticks = 2000 / portTICK_RATE_MS;

void test_ble_task(void *arg);

char BLE_NAME[] = {"My ESP"};
esp_err_t validate_level_change(uint8_t level);
esp_err_t validate_light_change(uint8_t light);
esp_err_t validate_time_change(uint32_t time);

ble_communication_config_t ble_config = {
    .device_name = BLE_NAME,
    .gatt_config = {
        .ds_cfg =
            {
                .support_level_write_cb = validate_level_change,
                .front_light_write_cb = validate_light_change,
                .time_write_cb = validate_time_change,
            },
    }};

/** SDS values */
uint8_t bike_model[2] = {5, 12}; //= 3077
uint8_t frame_number[10] = {'F', 'Y', 'I', '3', '4', '1', '4', '6', '3', '8'};
uint8_t battery_model[2] = {0, 3};
uint8_t battery_sn[16] = {'B', 'A', 'T', '1', '4', 'j', 'e', '4',
                          '4', '9', '6', '3', '8', 'K', '1', '8'};
uint8_t console_sfv[4] = {0, 0, 1, 23};
uint8_t console_hwv[4] = {0, 0, 0, 3};
uint8_t console_sn[16] = {'C', 'O', 'N', '0', '9', 'd', '-', '4',
                          '2', '9', '1', '3', '8', 'a', '-', '8'};
uint8_t motor_fwv[4] = {1, 5, 6, 1};
uint8_t motor_model[4] = {1, 6, 7, 8};
uint8_t motor_sn[16] = {'M', 'O', 'T', '0', '9', '6', '1', '4',
                        'm', '9', '1', 'm', '8', '6', 'i', '8'};
uint8_t battery_capacity[4] = {2, 2, 2, 2};
uint8_t wheel_diameter[2] = {98, 8}; // 2200
uint8_t ble_profilev[2] = {0, 1};
/* DS values */
uint16_t speed = 0;
uint16_t pedal_cadence = 0;
uint8_t battery_charge = 0;
uint8_t front_light = 0;
uint8_t walk_assist = 0;
uint16_t pedal_power = 0;
uint16_t calories = 0;
uint32_t odometer = 0;
uint8_t support_level = 0;
uint16_t console_status = 0;
uint32_t time = 0;
uint32_t trip_initial_odometer = 0;
uint32_t trip_initial_time = 0;

uint32_t time_offset;

void app_main() {
    ble_communication_init(ble_config);
    xTaskCreate(test_ble_task, "BLE TASK", STACK_DEPTH, NULL, TASK_PRIORITY, NULL);
}

void test_ble_task(void *arg) {
    ble_connection_status_t ble_status;
    uint8_t counter = 0;

    speed = 100;
    pedal_cadence = 50;
    battery_charge = 99;
    front_light = 0;
    walk_assist = 0;
    pedal_power = 50;
    calories = 0;
    odometer = 3678;
    support_level = 1;
    console_status = 1;
    trip_initial_odometer = odometer;
    time = 0;
    trip_initial_time = time_offset;

    set_ble_ds_speed(&speed);
    set_ble_ds_pedal_cadence(&pedal_cadence);
    set_ble_ds_battery_charge(&battery_charge);
    set_ble_ds_front_light(&front_light);
    set_ble_ds_walk_assist(&walk_assist);
    set_ble_ds_pedal_power(&pedal_power);
    set_ble_ds_calories(&calories);
    set_ble_ds_odometer(&odometer);
    set_ble_ds_support_level(&support_level);
    set_ble_ds_console_status(&console_status);
    set_ble_ds_trip_initial_odometer(&trip_initial_odometer);
    set_ble_ds_time(&time);
    set_ble_ds_trip_initial_time(&trip_initial_time);

    while (1) {
        ble_status = ble_get_conn_status();
        counter++;
        while (ble_status.status == BLE_CLIENT_DISCONNECTED) {
            vTaskDelay(two_seconds_ticks);
            ESP_LOGI("MAIN", "Waiting for client connection...");
            ble_status = ble_get_conn_status();
        }

        speed = speed - 10 + rand() % 20;
        pedal_cadence = pedal_cadence - 10 + rand() % 20;
        pedal_power = pedal_power - 10 + rand() % 20;
        calories = calories + rand();
        odometer = 3678 + rand() % 1;

        set_ble_ds_speed(&speed);
        set_ble_ds_pedal_cadence(&pedal_cadence);
        set_ble_ds_pedal_power(&pedal_power);

        set_ble_ds_calories(&calories);
        set_ble_ds_odometer(&odometer);

        ESP_LOGI(
            "MAIN",
            "Change speed = %d, pedal_cadence = %d, pedal_power = %d, calories = %d, odometer = %d",
            speed, pedal_cadence, pedal_power, calories, odometer);

        if (counter % 8) { // Slower vars

            battery_charge = 99 - rand() % 2;
            front_light = rand() % 2;
            walk_assist = rand() % 2;
            ESP_LOGI("MAIN", "Change battery_charge = %d, front_light = %d, walk_assist = %d",
                     battery_charge, front_light, walk_assist);

            set_ble_ds_battery_charge(&battery_charge);
            set_ble_ds_front_light(&front_light);
            set_ble_ds_walk_assist(&walk_assist);
        }

        vTaskDelay(one_second_ticks);
    }
}

esp_err_t validate_level_change(uint8_t level) {
    ESP_LOGI("MAIN", "Support level check: %d", level);
    if (level > 5) {
        return ESP_ERR_INVALID_STATE;
    }
    return ESP_OK;
}

esp_err_t validate_light_change(uint8_t light) {
    ESP_LOGI("MAIN", "Light change check: %d", light);
    return ESP_OK;
}
esp_err_t validate_time_change(uint32_t time) {
    ESP_LOGI("MAIN", "Current time received: %d. Configure internal offset to: %d and update trip",
             time, time_offset);

    uint32_t prev_offset = time_offset;
    time_offset = (esp_timer_get_time() / 1000) - time;
    trip_initial_time = time_offset;

    return ESP_OK;
}