#include "cashstick.h"

static uint32_t button_press_start_time = 0;
static bool button_was_pressed = false;

void button_init(void) {
    gpio_init(BUTTON_PIN);
    gpio_set_dir(BUTTON_PIN, GPIO_IN);
    gpio_pull_up(BUTTON_PIN);  // Active low button
}

bool button_is_pressed(void) {
    return !gpio_get(BUTTON_PIN);  // Active low
}

void button_handle_boot_mode(void) {
    printf("BOOT mode: Firmware update operations available\n");
    
    // In BOOT mode, button can trigger firmware operations
    // such as entering USB mass storage mode or resetting device
    
    if (button_is_pressed()) {
        uint32_t press_duration = 0;
        uint32_t start_time = get_system_time_ms();
        
        // Wait for button release and measure duration
        while (button_is_pressed()) {
            delay_ms(10);
            press_duration = get_system_time_ms() - start_time;
            
            // Visual feedback during long press
            if (press_duration > 3000) {
                led_blink(LED_STATE_BUSY, 100);
            }
        }
        
        printf("BOOT: Button pressed for %d ms\n", press_duration);
        
        if (press_duration > 5000) {
            // Long press (5+ seconds) - Factory reset
            printf("BOOT: Factory reset initiated\n");
            led_blink(LED_STATE_UNSEALED, 500);
            
            // Clear all stored keys and reset device
            bitcoin_keys_t empty_keys = {0};
            flash_write_keys(&empty_keys);
            flash_write_device_state(DEVICE_STATE_NEW);
            
            // Restart device
            watchdog_enable(100, 1);
            while(1);
            
        } else if (press_duration > 1000) {
            // Medium press (1-5 seconds) - Enter USB mass storage
            printf("BOOT: Entering USB mass storage mode\n");
            usb_mass_storage_mode();
        }
    }
}

void button_handle_test_mode(void) {
    printf("TEST mode: Running tamper integrity check\n");
    
    // In TEST mode (after firmware flash), button tests tamper-evident circuit
    led_set_state(LED_STATE_BUSY);
    
    // Run comprehensive tamper check
    tamper_status_t tamper_status = tamper_check_integrity();
    
    printf("TEST: Tamper check result - intact: %s, count: %d\n", 
           tamper_status.is_intact ? "true" : "false", 
           tamper_status.tamper_count);
    
    // Visual feedback based on tamper status
    if (tamper_status.is_intact) {
        // Device integrity OK - flash green 3 times
        printf("TEST: Device integrity PASSED\n");
        for (int i = 0; i < 3; i++) {
            led_set_state(LED_STATE_SEALED);
            delay_ms(200);
            led_set_rgb(0, 0, 0);
            delay_ms(200);
        }
        led_set_state(LED_STATE_SEALED);
        
    } else {
        // Tamper detected - flash red 5 times
        printf("TEST: Device integrity FAILED - tamper detected\n");
        for (int i = 0; i < 5; i++) {
            led_set_state(LED_STATE_UNSEALED);
            delay_ms(300);
            led_set_rgb(0, 0, 0);
            delay_ms(200);
        }
        led_set_state(LED_STATE_UNSEALED);
        
        // Update device state
        current_device_state = DEVICE_STATE_COMPROMISED;
        flash_write_device_state(current_device_state);
    }
}

// Advanced button handling with debouncing
bool button_check_with_debounce(uint16_t debounce_ms) {
    if (button_is_pressed()) {
        delay_ms(debounce_ms);
        return button_is_pressed();
    }
    return false;
}

// Get button press duration
uint32_t button_get_press_duration(void) {
    if (!button_is_pressed()) {
        return 0;
    }
    
    uint32_t start_time = get_system_time_ms();
    while (button_is_pressed()) {
        delay_ms(10);
    }
    
    return get_system_time_ms() - start_time;
}