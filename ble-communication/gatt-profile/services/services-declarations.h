/**
 * @file services-declarations.h
 * @brief Value declarations common for all services.
 *
 */

#ifndef BLE_SERVICES_DECLARATIONS_H_
#define BLE_SERVICES_DECLARATIONS_H_
#include "esp_err.h"
#include "esp_gatt_defs.h"

/* Declarations for attributes tables */
#define CHAR_DECLARATION_SIZE (sizeof(uint8_t))

#define SVC_INST_ID                                                                                \
    0 /*< Instance id for the services. At the moment is static since we only need one instance of \
         each service.*/

#define CCCD_NOTIFY_BYTE 0   /*< Position of the notify field in the cccd*/
#define CCCD_INDICATE_BYTE 1 /*< Position of the indicate field in the cccd*/

/**
 * @defgroup ServicesStatics The services need pointers to constant arrays for the initialization.
 * These are the defined here so they can be shared among multiple services.
 * @{
 */
extern const uint16_t
    primary_service_uuid; /*< UUID to identify this characteristic as a primary service field */
extern const uint16_t
    secondary_service_uuid; /*< UUID to identify this characteristic as a secondary service field */

extern const uint16_t character_declaration_uuid; /*< UUID to identify this characteristic as a
                                                     characteristic declaration field */
extern const uint16_t
    character_client_config_uuid; /*< UUID to identify this characteristic as a cccd field */

extern const uint8_t
    char_prop_read; /*< Used in the characteristic declaration to set as read only */
extern const uint8_t
    char_prop_write; /*< Used in the characteristic declaration to set as write only */
extern const uint8_t char_prop_read_write;
extern const uint8_t
    char_prop_read_notify; /*< Used in the characteristic declaration to set as read and notify */
extern const uint8_t char_prop_read_indicate;
extern const uint8_t
    char_prop_write_notify; /*< Used in the characteristic declaration to set as write and notify */
extern const uint8_t char_prop_read_write_notify; /*< Used in the characteristic declaration to set
                                                     as read, write and notify */
extern const uint8_t char_prop_read_write_indicate; /*< Used in the characteristic declaration to
                                                       set as read, write and indicate*/

/**@}*/

/**
 * @defgroup ServicesConfigItems  Common types for the configuration structures of the different
 * services
 * @{
 */
typedef esp_err_t (*uint8_char_write_check_t)(
    uint8_t); /*< Validation function for a one byte characteristic */
typedef esp_err_t (*uint16_char_write_check_t)(
    uint16_t); /*< Validation function for a two bytes characteristic */
typedef esp_err_t (*uint32_char_write_check_t)(
    uint32_t);             /*< Validation function for a four bytes characteristic */
typedef uint8_t cccd_t[2]; /*< Used to manage the cccd fields persistance */

/*< Prefix and subfix for the custom 128 bit UUID's. The CUSTOM_UUID_16 macro uses them to easily
 * define UUID's. Keep in mind that the byte order is inverted so with these values you would get an
 * UUID like 23 01 XX XX 18 17 .... */

#define CUSTOM_UUID_SUBFIX 0x01, 0x23
#define CUSTOM_UUID_PREFIX 0x45, 0x67, 0x89, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18

#define CUSTOM_UUID_BYTE_1_POS 13 /*!< Position of first unique byte in the UUID's */
#define CUSTOM_UUID_BYTE_2_POS 12 /*!< Position of second unique byte in the UUID's */

/*< Generates a custom 128bit UUID
 */
#define CUSTOM_UUID_16(uuid16_0, uuid16_1)                                                         \
    { CUSTOM_UUID_PREFIX, uuid16_1, uuid16_0, CUSTOM_UUID_SUBFIX }
/**@}*/

#endif /* BLE_SERVICES_DECLARATIONS_H_ */