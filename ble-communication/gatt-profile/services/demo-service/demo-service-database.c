#include "demo-service-database.h"

uint8_t DEMO_SERVICE_UUID[16] = {0x4b, 0x91, 0x31, 0xc3, 0xc9, 0xc5, 0xcc, 0x8f,
                                 0x9e, 0x45, 0xb5, 0x1f, 0x01, 0xc2, 0xaf, 0x4f};
const uint8_t DS_ASSIST_LEVEL_UUID[16] = {0xe7, 0x46, 0x8d, 0x02, 0xc3, 0x5a, 0x72, 0x99,
                                          0xe1, 0x46, 0x7d, 0xa3, 0x52, 0x07, 0xcf, 0x98};
const uint8_t DS_SPEED_UUID[16] = {0x71, 0xd1, 0x40, 0x40, 0x2a, 0xe6, 0x53, 0x99,
                                   0x29, 0x45, 0x68, 0xcc, 0x43, 0x51, 0xeb, 0x69};
const uint8_t DS_BATTERY_CHARGE_UUID[16] = {0x6f, 0x48, 0x2f, 0x9e, 0x66, 0xa5, 0x6e, 0xb6,
                                            0x67, 0x49, 0x33, 0x14, 0x5e, 0x2b, 0xbd, 0x70};

demo_service_data_t demo_service_data = {
    .assist_level = 0,
    .assist_level_ccc = {0x00, 0x00},
    .battery_charge = 0,
    .battery_charge_ccc = {0x00, 0x00},
    .speed = 0,
    .speed_ccc = {0x00, 0x00},
};

esp_gatts_attr_db_t demo_service_gatt_db[DEMO_SERVICE_NB] = {
    [IDX_DEMO_SVC] = {{ESP_GATT_AUTO_RSP},
                      {ESP_UUID_LEN_16, (uint8_t *)&primary_service_uuid, ESP_GATT_PERM_READ,
                       sizeof(uint16_t), sizeof(DEMO_SERVICE_UUID), (uint8_t *)&DEMO_SERVICE_UUID}},

    /* Characteristic Declaration */
    [IDX_ASSIST_LEVEL_DEC] = {{ESP_GATT_AUTO_RSP},
                              {ESP_UUID_LEN_16, (uint8_t *)&character_declaration_uuid,
                               ESP_GATT_PERM_READ, CHAR_DECLARATION_SIZE, CHAR_DECLARATION_SIZE,
                               (uint8_t *)&char_prop_read_write_notify}},

    /* Characteristic Value */
    [IDX_ASSIST_LEVEL_VAL] = {{ESP_GATT_RSP_BY_APP},
                              {ESP_UUID_LEN_128, (uint8_t *)&DS_ASSIST_LEVEL_UUID,
                               ESP_GATT_PERM_READ | ESP_GATT_PERM_WRITE, sizeof(uint8_t),
                               sizeof(demo_service_data.assist_level),
                               (uint8_t *)&demo_service_data.assist_level}},

    /* Client Characteristic Configuration Descriptor */
    [IDX_ASSIST_LEVEL_CFG] = {{ESP_GATT_AUTO_RSP},
                              {ESP_UUID_LEN_16, (uint8_t *)&character_client_config_uuid,
                               ESP_GATT_PERM_READ | ESP_GATT_PERM_WRITE, sizeof(uint16_t),
                               sizeof(demo_service_data.assist_level_ccc),
                               (uint8_t *)(demo_service_data.assist_level_ccc)}},

    /* Characteristic Declaration */
    [IDX_BATTERY_CHARGE_DEC] = {{ESP_GATT_AUTO_RSP},
                                {ESP_UUID_LEN_16, (uint8_t *)&character_declaration_uuid,
                                 ESP_GATT_PERM_READ, CHAR_DECLARATION_SIZE, CHAR_DECLARATION_SIZE,
                                 (uint8_t *)&char_prop_read_notify}},

    /* Characteristic Value */
    [IDX_BATTERY_CHARGE_VAL] = {{ESP_GATT_AUTO_RSP},
                                {ESP_UUID_LEN_128, (uint8_t *)&DS_BATTERY_CHARGE_UUID,
                                 ESP_GATT_PERM_READ, sizeof(uint8_t),
                                 sizeof(demo_service_data.battery_charge),
                                 (uint8_t *)&demo_service_data.battery_charge}},

    /* Client Characteristic Configuration Descriptor */
    [IDX_BATTERY_CHARGE_CFG] = {{ESP_GATT_AUTO_RSP},
                                {ESP_UUID_LEN_16, (uint8_t *)&character_client_config_uuid,
                                 ESP_GATT_PERM_READ | ESP_GATT_PERM_WRITE, sizeof(uint16_t),
                                 sizeof(demo_service_data.battery_charge_ccc),
                                 (uint8_t *)(demo_service_data.battery_charge_ccc)}},

    /* Characteristic Declaration */
    [IDX_SPEED_DEC] = {{ESP_GATT_AUTO_RSP},
                       {ESP_UUID_LEN_16, (uint8_t *)&character_declaration_uuid, ESP_GATT_PERM_READ,
                        CHAR_DECLARATION_SIZE, CHAR_DECLARATION_SIZE,
                        (uint8_t *)&char_prop_read_notify}},

    /* Characteristic Valesp_ble_gatt_set_local_mtuue */
    [IDX_SPEED_VAL] = {{ESP_GATT_AUTO_RSP},
                       {ESP_UUID_LEN_128, (uint8_t *)&DS_SPEED_UUID, ESP_GATT_PERM_READ,
                        sizeof(uint16_t), sizeof(demo_service_data.speed),
                        (uint8_t *)&demo_service_data.speed}},

    /* Client Characteristic Configuration Descriptor */
    [IDX_SPEED_CFG] = {{ESP_GATT_AUTO_RSP},
                       {ESP_UUID_LEN_16, (uint8_t *)&character_client_config_uuid,
                        ESP_GATT_PERM_READ | ESP_GATT_PERM_WRITE, sizeof(uint16_t),
                        sizeof(demo_service_data.speed_ccc),
                        (uint8_t *)(demo_service_data.speed_ccc)}}};