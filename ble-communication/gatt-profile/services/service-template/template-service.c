#include "template-service.h"
#include "template-service-database.h"
#include "template-service-event-handler.h"

uint8_t get_ble_char_1() { return service_data.char_1; }

void set_ble_char_1(uint8_t char_1) {
    service_data.char_1 = char_1;
    esp_ble_gatts_set_attr_value(service_handles[IDX_TEMPLATE_CHAR_1_VAL],
                                 sizeof(service_data.char_1), (uint8_t *)&service_data.char_1);
}
