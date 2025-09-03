#ifndef CASHSTICK_H
#define CASHSTICK_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pico/stdlib.h"
#include "pico/unique_id.h"
#include "pico/bootrom.h"
#include "hardware/gpio.h"
#include "hardware/i2c.h"
#include "hardware/uart.h"
#include "hardware/flash.h"
#include "hardware/watchdog.h"

// Hardware pin definitions from schematic
#define LED_PIN 16              // RGB LED on GPIO16
#define BUTTON_PIN 15           // BOOT/TEST button
#define I2C_SDA_PIN 14          // SE050 I2C SDA
#define I2C_SCL_PIN 15          // SE050 I2C SCL
#define I2C_BAUDRATE 400000     // 400kHz I2C

// SE050 I2C address
#define SE050_I2C_ADDR 0x48

// LED States
typedef enum {
    LED_STATE_NEW = 0,      // Blue - Device is new/uninitialized
    LED_STATE_SEALED = 1,   // Green - Device is sealed and secure
    LED_STATE_UNSEALED = 2, // Red - Tamper detected or unsealed
    LED_STATE_BUSY = 3      // Yellow - Processing operation
} led_state_t;

// Device States
typedef enum {
    DEVICE_STATE_NEW = 0,
    DEVICE_STATE_INITIALIZED = 1,
    DEVICE_STATE_SEALED = 2,
    DEVICE_STATE_COMPROMISED = 3
} device_state_t;

// SE050 Command Types
typedef enum {
    SE050_CMD_GET_VERSION = 0x01,
    SE050_CMD_GENERATE_KEYPAIR = 0x02,
    SE050_CMD_SIGN_HASH = 0x03,
    SE050_CMD_GET_PUBKEY = 0x04,
    SE050_CMD_SET_TAMPER_CONFIG = 0x05
} se050_cmd_t;

// Bitcoin key structure
typedef struct {
    uint8_t private_key[32];
    uint8_t public_key[33];
    char address[42];        // Bitcoin address string
    bool is_sealed;
    bool keys_revealed;      // True when tamper seal broken and keys exposed
} bitcoin_keys_t;

// Tamper detection structure
typedef struct {
    bool is_intact;
    uint32_t tamper_count;
    uint32_t last_check_time;
} tamper_status_t;

// Function declarations

// LED Control
void led_init(void);
void led_set_state(led_state_t state);
void led_set_rgb(uint8_t r, uint8_t g, uint8_t b);
void led_blink(led_state_t state, uint16_t duration_ms);

// SE050 Interface
bool se050_init(void);
bool se050_generate_bitcoin_keys(bitcoin_keys_t *keys);
bool se050_sign_transaction(const uint8_t *hash, uint8_t *signature);
bool se050_get_device_info(uint8_t *info, size_t *info_len);
bool se050_configure_tamper_detection(void);

// USB Handler
void usb_init(void);
void usb_handle_commands(void);
void usb_mass_storage_mode(void);
bool usb_is_connected(void);

// Button Handler
void button_init(void);
bool button_is_pressed(void);
void button_handle_boot_mode(void);
void button_handle_test_mode(void);

// Bitcoin Wallet Functions
bool wallet_generate_new_keys(void);
bool wallet_get_address(char *address_out, size_t max_len);
bool wallet_export_public_key(uint8_t *pubkey_out);
bool wallet_reveal_private_key(uint8_t *privkey_out);
bool wallet_are_keys_revealed(void);

// Tamper Detection
bool tamper_init(void);
tamper_status_t tamper_check_integrity(void);
void tamper_seal_device(void);
bool tamper_is_device_compromised(void);

// Utility Functions
void system_init(void);
void system_shutdown(void);
uint32_t get_system_time_ms(void);
void delay_ms(uint32_t ms);

// Flash Storage Functions
bool flash_write_keys(const bitcoin_keys_t *keys);
bool flash_read_keys(bitcoin_keys_t *keys);
bool flash_write_device_state(device_state_t state);
device_state_t flash_read_device_state(void);

#endif // CASHSTICK_H