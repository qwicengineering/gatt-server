#include "template-service-database.h"

// Definition of all the UUID's (service and characteristics)
uint8_t TEMPLATE_SERVICE_UUID[16] = CUSTOM_UUID_16(0xFF, 0xFF);
uint8_t TS_CHAR_1_UUID[16] = CUSTOM_UUID_16(0xFF, 0xFE);

// Instance to hold the current values on the service
service_data_t service_data = {};

// Database needed to create the service using the ESP-IDF driver
esp_gatts_attr_db_t template_service_gatt_db[TEMPLATE_SERVICE_NB] = {

    [IDX_TEMPLATE_SVC] = {{ESP_GATT_AUTO_RSP},
                          {ESP_UUID_LEN_16, (uint8_t *)&primary_service_uuid, ESP_GATT_PERM_READ,
                           sizeof(uint16_t), sizeof(TEMPLATE_SERVICE_UUID),
                           (uint8_t *)&TEMPLATE_SERVICE_UUID}},

    [IDX_TEMPLATE_CHAR_1_DEC] = {{ESP_GATT_AUTO_RSP},
                                 {ESP_UUID_LEN_16, (uint8_t *)&character_declaration_uuid,
                                  ESP_GATT_PERM_READ, CHAR_DECLARATION_SIZE, CHAR_DECLARATION_SIZE,
                                  (uint8_t *)&char_prop_read_write_notify}},
    [IDX_TEMPLATE_CHAR_1_VAL1] = {{ESP_GATT_AUTO_RSP},
                                  {ESP_UUID_LEN_128, (uint8_t *)&TS_CHAR_1_UUID, ESP_GATT_PERM_READ,
                                   sizeof(service_data_t.char_1), sizeof(service_data_t.char_1),
                                   (uint8_t *)&service_data.char_1}},
    [IDX_TEMPLATE_CHAR_1_CFG] = {{ESP_GATT_AUTO_RSP},
                                 {ESP_UUID_LEN_16, (uint8_t *)&character_client_config_uuid,
                                  ESP_GATT_PERM_READ | ESP_GATT_PERM_WRITE, sizeof(uint16_t),
                                  sizeof(uint16_t), (uint8_t *)&dashboard_service_data.char_1_ccc}},

};