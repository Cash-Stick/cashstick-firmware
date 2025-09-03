#include "cashstick.h"

// RGB LED PWM channels (using GPIO16 with WS2812B or similar RGB LED)
static uint slice_num;

void led_init(void) {
    // Initialize GPIO for LED
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
    gpio_put(LED_PIN, 0);
    
    // Set initial state to busy (yellow) during boot
    led_set_state(LED_STATE_BUSY);
}

void led_set_rgb(uint8_t r, uint8_t g, uint8_t b) {
    // For simplicity using basic GPIO control
    // In production, implement WS2812B protocol or use RGB LED with PWM
    
    if (r > 0 || g > 0 || b > 0) {
        gpio_put(LED_PIN, 1);  // Turn on LED
    } else {
        gpio_put(LED_PIN, 0);  // Turn off LED
    }
}

void led_set_state(led_state_t state) {
    switch (state) {
        case LED_STATE_NEW:
            // Blue - Device is new/uninitialized
            led_set_rgb(0, 0, 255);
            break;
            
        case LED_STATE_SEALED:
            // Green - Device is sealed and secure
            led_set_rgb(0, 255, 0);
            break;
            
        case LED_STATE_UNSEALED:
            // Red - Tamper detected or compromised
            led_set_rgb(255, 0, 0);
            break;
            
        case LED_STATE_BUSY:
            // Yellow - Processing operation
            led_set_rgb(255, 255, 0);
            break;
            
        default:
            led_set_rgb(0, 0, 0);  // Off
            break;
    }
}

void led_blink(led_state_t state, uint16_t duration_ms) {
    led_set_state(state);
    delay_ms(duration_ms);
    led_set_rgb(0, 0, 0);  // Turn off
}