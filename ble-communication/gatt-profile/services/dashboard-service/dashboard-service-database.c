#include "dashboard-service-database.h"

uint8_t DASHBOARD_SERVICE_UUID[16] = CUSTOM_UUID_16(0x11, 0x00);
uint8_t DS_SPEED_CHAR_UUID[16] = CUSTOM_UUID_16(0x11, 0x01);
uint8_t DS_PEDAL_CADENCE_CHAR_UUID[16] = CUSTOM_UUID_16(0x11, 0x02);
uint8_t DS_BATTERY_CHARGE_CHAR_UUID[16] = CUSTOM_UUID_16(0x11, 0x03);
uint8_t DS_FRONT_LIGHT_CHAR_UUID[16] = CUSTOM_UUID_16(0x11, 0x04);
uint8_t DS_WALK_ASSIST_CHAR_UUID[16] = CUSTOM_UUID_16(0x11, 0x05);
uint8_t DS_PEDAL_POWER_CHAR_UUID[16] = CUSTOM_UUID_16(0x11, 0x06);
uint8_t DS_CALORIES_CHAR_UUID[16] = CUSTOM_UUID_16(0x11, 0x07);
uint8_t DS_ODOMETER_CHAR_UUID[16] = CUSTOM_UUID_16(0x11, 0x08);
uint8_t DS_SUPPORT_LEVEL_CHAR_UUID[16] = CUSTOM_UUID_16(0x11, 0x09);
uint8_t DS_CONSOLE_STATUS_CHAR_UUID[16] = CUSTOM_UUID_16(0x11, 0x0A);
uint8_t DS_TIME_CHAR_UUID[16] = CUSTOM_UUID_16(0x11, 0x0B);
uint8_t DS_TRIP_INITIAL_ODOMETER_CHAR_UUID[16] = CUSTOM_UUID_16(0x11, 0x0C);
uint8_t DS_TRIP_INITIAL_TIME_CHAR_UUID[16] = CUSTOM_UUID_16(0x11, 0x0D);

dashboard_service_data_t dashboard_service_data = {};

esp_gatts_attr_db_t dashboard_service_gatt_db[DASHBOARD_SERVICE_NB] = {
    [IDX_DASHBOARD_SVC] = {{ESP_GATT_AUTO_RSP},
                           {ESP_UUID_LEN_16, (uint8_t *)&primary_service_uuid, ESP_GATT_PERM_READ,
                            sizeof(uint16_t), sizeof(DASHBOARD_SERVICE_UUID),
                            (uint8_t *)&DASHBOARD_SERVICE_UUID}},

    [IDX_DS_SPEED_DEC] = {{ESP_GATT_AUTO_RSP},
                          {ESP_UUID_LEN_16, (uint8_t *)&character_declaration_uuid,
                           ESP_GATT_PERM_READ, CHAR_DECLARATION_SIZE, CHAR_DECLARATION_SIZE,
                           (uint8_t *)&char_prop_read_notify}},
    [IDX_DS_SPEED_VAL] = {{ESP_GATT_AUTO_RSP},
                          {ESP_UUID_LEN_128, (uint8_t *)&DS_SPEED_CHAR_UUID, ESP_GATT_PERM_READ,
                           sizeof(dashboard_service_data.speed),
                           sizeof(dashboard_service_data.speed),
                           (uint8_t *)&dashboard_service_data.speed}},
    [IDX_DS_SPEED_CFG] = {{ESP_GATT_AUTO_RSP},
                          {ESP_UUID_LEN_16, (uint8_t *)&character_client_config_uuid,
                           ESP_GATT_PERM_READ | ESP_GATT_PERM_WRITE, sizeof(uint16_t),
                           sizeof(uint16_t), (uint8_t *)&dashboard_service_data.speed_ccc}},

    [IDX_DS_PEDAL_CADENCE_DEC] = {{ESP_GATT_AUTO_RSP},
                                  {ESP_UUID_LEN_16, (uint8_t *)&character_declaration_uuid,
                                   ESP_GATT_PERM_READ, CHAR_DECLARATION_SIZE, CHAR_DECLARATION_SIZE,
                                   (uint8_t *)&char_prop_read_notify}},
    [IDX_DS_PEDAL_CADENCE_VAL] = {{ESP_GATT_AUTO_RSP},
                                  {ESP_UUID_LEN_128, (uint8_t *)&DS_PEDAL_CADENCE_CHAR_UUID,
                                   ESP_GATT_PERM_READ, sizeof(dashboard_service_data.pedal_cadence),
                                   sizeof(dashboard_service_data.pedal_cadence),
                                   (uint8_t *)&dashboard_service_data.pedal_cadence}},
    [IDX_DS_PEDAL_CADENCE_CFG] = {{ESP_GATT_AUTO_RSP},
                                  {ESP_UUID_LEN_16, (uint8_t *)&character_client_config_uuid,
                                   ESP_GATT_PERM_READ | ESP_GATT_PERM_WRITE, sizeof(uint16_t),
                                   sizeof(uint16_t),
                                   (uint8_t *)&dashboard_service_data.pedal_cadence_ccc}},

    [IDX_DS_BATTERY_CHARGE_DEC] = {{ESP_GATT_AUTO_RSP},
                                   {ESP_UUID_LEN_16, (uint8_t *)&character_declaration_uuid,
                                    ESP_GATT_PERM_READ, CHAR_DECLARATION_SIZE,
                                    CHAR_DECLARATION_SIZE, (uint8_t *)&char_prop_read_notify}},
    [IDX_DS_BATTERY_CHARGE_VAL] = {{ESP_GATT_AUTO_RSP},
                                   {ESP_UUID_LEN_128, (uint8_t *)&DS_BATTERY_CHARGE_CHAR_UUID,
                                    ESP_GATT_PERM_READ,
                                    sizeof(dashboard_service_data.battery_charge),
                                    sizeof(dashboard_service_data.battery_charge),
                                    (uint8_t *)&dashboard_service_data.battery_charge}},
    [IDX_DS_BATTERY_CHARGE_CFG] = {{ESP_GATT_AUTO_RSP},
                                   {ESP_UUID_LEN_16, (uint8_t *)&character_client_config_uuid,
                                    ESP_GATT_PERM_READ | ESP_GATT_PERM_WRITE, sizeof(uint16_t),
                                    sizeof(uint16_t),
                                    (uint8_t *)&dashboard_service_data.battery_charge_ccc}},

    [IDX_DS_FRONT_LIGHT_DEC] = {{ESP_GATT_AUTO_RSP},
                                {ESP_UUID_LEN_16, (uint8_t *)&character_declaration_uuid,
                                 ESP_GATT_PERM_READ, CHAR_DECLARATION_SIZE, CHAR_DECLARATION_SIZE,
                                 (uint8_t *)&char_prop_read_write_indicate}},
    [IDX_DS_FRONT_LIGHT_VAL] = {{ESP_GATT_RSP_BY_APP},
                                {ESP_UUID_LEN_128, (uint8_t *)&DS_FRONT_LIGHT_CHAR_UUID,
                                 ESP_GATT_PERM_READ | ESP_GATT_PERM_WRITE,
                                 sizeof(dashboard_service_data.front_light),
                                 sizeof(dashboard_service_data.front_light),
                                 (uint8_t *)&dashboard_service_data.front_light}},

    [IDX_DS_WALK_ASSIST_DEC] = {{ESP_GATT_AUTO_RSP},
                                {ESP_UUID_LEN_16, (uint8_t *)&character_declaration_uuid,
                                 ESP_GATT_PERM_READ, CHAR_DECLARATION_SIZE, CHAR_DECLARATION_SIZE,
                                 (uint8_t *)&char_prop_read_indicate}},
    [IDX_DS_WALK_ASSIST_VAL] = {{ESP_GATT_AUTO_RSP},
                                {ESP_UUID_LEN_128, (uint8_t *)&DS_WALK_ASSIST_CHAR_UUID,
                                 ESP_GATT_PERM_READ, sizeof(dashboard_service_data.walk_assist),
                                 sizeof(dashboard_service_data.walk_assist),
                                 (uint8_t *)&dashboard_service_data.walk_assist}},

    [IDX_DS_PEDAL_POWER_DEC] = {{ESP_GATT_AUTO_RSP},
                                {ESP_UUID_LEN_16, (uint8_t *)&character_declaration_uuid,
                                 ESP_GATT_PERM_READ, CHAR_DECLARATION_SIZE, CHAR_DECLARATION_SIZE,
                                 (uint8_t *)&char_prop_read_notify}},
    [IDX_DS_PEDAL_POWER_VAL] = {{ESP_GATT_AUTO_RSP},
                                {ESP_UUID_LEN_128, (uint8_t *)&DS_PEDAL_POWER_CHAR_UUID,
                                 ESP_GATT_PERM_READ, sizeof(dashboard_service_data.pedal_power),
                                 sizeof(dashboard_service_data.pedal_power),
                                 (uint8_t *)&dashboard_service_data.pedal_power}},
    [IDX_DS_PEDAL_POWER_CFG] = {{ESP_GATT_AUTO_RSP},
                                {ESP_UUID_LEN_16, (uint8_t *)&character_client_config_uuid,
                                 ESP_GATT_PERM_READ | ESP_GATT_PERM_WRITE, sizeof(uint16_t),
                                 sizeof(uint16_t),
                                 (uint8_t *)&dashboard_service_data.pedal_power_ccc}},

    [IDX_DS_CALORIES_DEC] = {{ESP_GATT_AUTO_RSP},
                             {ESP_UUID_LEN_16, (uint8_t *)&character_declaration_uuid,
                              ESP_GATT_PERM_READ, CHAR_DECLARATION_SIZE, CHAR_DECLARATION_SIZE,
                              (uint8_t *)&char_prop_read_notify}},
    [IDX_DS_CALORIES_VAL] = {{ESP_GATT_AUTO_RSP},
                             {ESP_UUID_LEN_128, (uint8_t *)&DS_CALORIES_CHAR_UUID,
                              ESP_GATT_PERM_READ, sizeof(dashboard_service_data.calories),
                              sizeof(dashboard_service_data.calories),
                              (uint8_t *)&dashboard_service_data.calories}},
    [IDX_DS_CALORIES_CFG] = {{ESP_GATT_AUTO_RSP},
                             {ESP_UUID_LEN_16, (uint8_t *)&character_client_config_uuid,
                              ESP_GATT_PERM_READ | ESP_GATT_PERM_WRITE, sizeof(uint16_t),
                              sizeof(uint16_t), (uint8_t *)&dashboard_service_data.calories_ccc}},

    [IDX_DS_ODOMETER_DEC] = {{ESP_GATT_AUTO_RSP},
                             {ESP_UUID_LEN_16, (uint8_t *)&character_declaration_uuid,
                              ESP_GATT_PERM_READ, CHAR_DECLARATION_SIZE, CHAR_DECLARATION_SIZE,
                              (uint8_t *)&char_prop_read_notify}},
    [IDX_DS_ODOMETER_VAL] = {{ESP_GATT_AUTO_RSP},
                             {ESP_UUID_LEN_128, (uint8_t *)&DS_ODOMETER_CHAR_UUID,
                              ESP_GATT_PERM_READ, sizeof(dashboard_service_data.odometer),
                              sizeof(dashboard_service_data.odometer),
                              (uint8_t *)&dashboard_service_data.odometer}},
    [IDX_DS_ODOMETER_CFG] = {{ESP_GATT_AUTO_RSP},
                             {ESP_UUID_LEN_16, (uint8_t *)&character_client_config_uuid,
                              ESP_GATT_PERM_READ | ESP_GATT_PERM_WRITE, sizeof(uint16_t),
                              sizeof(uint16_t), (uint8_t *)&dashboard_service_data.odometer_ccc}},

    [IDX_DS_SUPPORT_LEVEL_DEC] = {{ESP_GATT_AUTO_RSP},
                                  {ESP_UUID_LEN_16, (uint8_t *)&character_declaration_uuid,
                                   ESP_GATT_PERM_READ, CHAR_DECLARATION_SIZE, CHAR_DECLARATION_SIZE,
                                   (uint8_t *)&char_prop_read_write_indicate}},
    [IDX_DS_SUPPORT_LEVEL_VAL] = {{ESP_GATT_RSP_BY_APP},
                                  {ESP_UUID_LEN_128, (uint8_t *)&DS_SUPPORT_LEVEL_CHAR_UUID,
                                   ESP_GATT_PERM_READ | ESP_GATT_PERM_WRITE,
                                   sizeof(dashboard_service_data.support_level),
                                   sizeof(dashboard_service_data.support_level),
                                   (uint8_t *)&dashboard_service_data.support_level}},

    [IDX_DS_CONSOLE_STATUS_DEC] = {{ESP_GATT_AUTO_RSP},
                                   {ESP_UUID_LEN_16, (uint8_t *)&character_declaration_uuid,
                                    ESP_GATT_PERM_READ, CHAR_DECLARATION_SIZE,
                                    CHAR_DECLARATION_SIZE, (uint8_t *)&char_prop_read_indicate}},
    [IDX_DS_CONSOLE_STATUS_VAL] = {{ESP_GATT_AUTO_RSP},
                                   {ESP_UUID_LEN_128, (uint8_t *)&DS_CONSOLE_STATUS_CHAR_UUID,
                                    ESP_GATT_PERM_READ,
                                    sizeof(dashboard_service_data.console_status),
                                    sizeof(dashboard_service_data.console_status),
                                    (uint8_t *)&dashboard_service_data.console_status}},

    [IDX_DS_TIME_DEC] = {{ESP_GATT_AUTO_RSP},
                         {ESP_UUID_LEN_16, (uint8_t *)&character_declaration_uuid,
                          ESP_GATT_PERM_READ, CHAR_DECLARATION_SIZE, CHAR_DECLARATION_SIZE,
                          (uint8_t *)&char_prop_read_write}},
    [IDX_DS_TIME_VAL] = {{ESP_GATT_RSP_BY_APP},
                         {ESP_UUID_LEN_128, (uint8_t *)&DS_TIME_CHAR_UUID,
                          ESP_GATT_PERM_READ | ESP_GATT_PERM_WRITE,
                          sizeof(dashboard_service_data.time), sizeof(dashboard_service_data.time),
                          (uint8_t *)&dashboard_service_data.time}},

    [IDX_DS_TRIP_INITIAL_ODOMETER_DEC] = {{ESP_GATT_AUTO_RSP},
                                          {ESP_UUID_LEN_16, (uint8_t *)&character_declaration_uuid,
                                           ESP_GATT_PERM_READ, CHAR_DECLARATION_SIZE,
                                           CHAR_DECLARATION_SIZE,
                                           (uint8_t *)&char_prop_read_indicate}},
    [IDX_DS_TRIP_INITIAL_ODOMETER_VAL] =
        {{ESP_GATT_AUTO_RSP},
         {ESP_UUID_LEN_128, (uint8_t *)&DS_TRIP_INITIAL_ODOMETER_CHAR_UUID, ESP_GATT_PERM_READ,
          sizeof(dashboard_service_data.trip_initial_odometer),
          sizeof(dashboard_service_data.trip_initial_odometer),
          (uint8_t *)&dashboard_service_data.trip_initial_odometer}},

    [IDX_DS_TRIP_INITIAL_TIME_DEC] = {{ESP_GATT_AUTO_RSP},
                                      {ESP_UUID_LEN_16, (uint8_t *)&character_declaration_uuid,
                                       ESP_GATT_PERM_READ, CHAR_DECLARATION_SIZE,
                                       CHAR_DECLARATION_SIZE, (uint8_t *)&char_prop_read_indicate}},
    [IDX_DS_TRIP_INITIAL_TIME_VAL] = {{ESP_GATT_AUTO_RSP},
                                      {ESP_UUID_LEN_128, (uint8_t *)&DS_TRIP_INITIAL_TIME_CHAR_UUID,
                                       ESP_GATT_PERM_READ,
                                       sizeof(dashboard_service_data.trip_initial_time),
                                       sizeof(dashboard_service_data.trip_initial_time),
                                       (uint8_t *)&dashboard_service_data.trip_initial_time}}};
