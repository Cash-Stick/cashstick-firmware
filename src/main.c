#include "cashstick.h"

// Global state variables
static device_state_t current_device_state = DEVICE_STATE_NEW;
static bitcoin_keys_t device_keys = {0};
static bool firmware_mode_active = false;

// Main initialization function
void system_init(void) {
    // Initialize Pico SDK
    stdio_init_all();
    
    // Initialize hardware components
    led_init();
    button_init();
    usb_init();
    
    // Set initial LED state
    led_set_state(LED_STATE_BUSY);
    
    // Initialize I2C for SE050
    i2c_init(i2c1, I2C_BAUDRATE);
    gpio_set_function(I2C_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA_PIN);
    gpio_pull_up(I2C_SCL_PIN);
    
    // Initialize SE050 secure element
    if (!se050_init()) {
        // SE050 initialization failed - indicate error
        led_set_state(LED_STATE_UNSEALED);
        return;
    }
    
    // Read device state from flash
    current_device_state = flash_read_device_state();
    
    // Initialize tamper detection
    if (!tamper_init()) {
        led_set_state(LED_STATE_UNSEALED);
        return;
    }
    
    // Check tamper status
    tamper_status_t tamper_status = tamper_check_integrity();
    if (!tamper_status.is_intact) {
        current_device_state = DEVICE_STATE_COMPROMISED;
        led_set_state(LED_STATE_UNSEALED);
        return;
    }
    
    // Set LED based on device state
    switch (current_device_state) {
        case DEVICE_STATE_NEW:
            led_set_state(LED_STATE_NEW);        // Blue
            break;
        case DEVICE_STATE_SEALED:
            led_set_state(LED_STATE_SEALED);     // Green
            break;
        case DEVICE_STATE_COMPROMISED:
            led_set_state(LED_STATE_UNSEALED);   // Red
            break;
        default:
            led_set_state(LED_STATE_NEW);
            break;
    }
}

// Main firmware loop
int main() {
    // System initialization
    system_init();
    
    // Check if device should enter firmware update mode
    // This happens on first boot or when BOOT button is held during power-on
    if (current_device_state == DEVICE_STATE_NEW || button_is_pressed()) {
        // Enter USB mass storage mode for drag-and-drop firmware installation
        usb_mass_storage_mode();
        firmware_mode_active = true;
    }
    
    printf("CashStick Firmware v1.0.0 - Starting...\n");
    printf("Device State: %d\n", current_device_state);
    
    // Main event loop
    while (true) {
        // Handle button presses
        if (button_is_pressed()) {
            led_set_state(LED_STATE_BUSY);
            
            if (firmware_mode_active) {
                // BOOT mode - handle firmware operations
                button_handle_boot_mode();
            } else {
                // TEST mode - run tamper integrity check
                button_handle_test_mode();
                
                tamper_status_t tamper_status = tamper_check_integrity();
                if (tamper_status.is_intact) {
                    // Flash green 3 times for "integrity OK"
                    for (int i = 0; i < 3; i++) {
                        led_blink(LED_STATE_SEALED, 200);
                        delay_ms(200);
                    }
                    led_set_state(LED_STATE_SEALED);
                } else {
                    // Flash red 5 times for "tamper detected"
                    for (int i = 0; i < 5; i++) {
                        led_blink(LED_STATE_UNSEALED, 200);
                        delay_ms(200);
                    }
                    led_set_state(LED_STATE_UNSEALED);
                    current_device_state = DEVICE_STATE_COMPROMISED;
                    flash_write_device_state(current_device_state);
                }
            }
            
            // Wait for button release
            while (button_is_pressed()) {
                delay_ms(10);
            }
        }
        
        // Handle USB communication
        if (usb_is_connected()) {
            usb_handle_commands();
        }
        
        // Watchdog feed
        watchdog_update();
        
        // Small delay to prevent busy waiting
        delay_ms(10);
    }
    
    return 0;
}

// System shutdown (called before reset/power off)
void system_shutdown(void) {
    // Set LED to indicate shutdown
    led_set_state(LED_STATE_BUSY);
    
    // Save any pending state to flash
    flash_write_device_state(current_device_state);
    
    // Turn off LED
    led_set_rgb(0, 0, 0);
    
    printf("CashStick shutting down...\n");
}