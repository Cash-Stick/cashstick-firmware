#include "cashstick.h"

// Tamper detection state
static tamper_status_t tamper_state = {0};
static uint32_t tamper_check_pin = 17;  // Example tamper detect pin

bool tamper_init(void) {
    // Initialize tamper detection circuitry
    gpio_init(tamper_check_pin);
    gpio_set_dir(tamper_check_pin, GPIO_IN);
    gpio_pull_up(tamper_check_pin);
    
    // Initialize tamper status from flash or SE050
    tamper_state.is_intact = true;
    tamper_state.tamper_count = 0;
    tamper_state.last_check_time = get_system_time_ms();
    
    // Configure SE050 tamper detection
    if (!se050_configure_tamper_detection()) {
        printf("TAMPER: SE050 configuration failed\n");
        return false;
    }
    
    printf("TAMPER: Detection system initialized\n");
    return true;
}

tamper_status_t tamper_check_integrity(void) {
    printf("TAMPER: Running integrity check\n");
    
    // Update check time
    tamper_state.last_check_time = get_system_time_ms();
    
    // Method 1: Check physical tamper detection circuit
    bool circuit_intact = gpio_get(tamper_check_pin);
    
    // Method 2: Check SE050 tamper registers
    bool se050_intact = se050_check_tamper_status();
    
    // Method 3: Verify cryptographic sealing
    bool crypto_intact = verify_cryptographic_seal();
    
    // Device is intact only if all checks pass
    tamper_state.is_intact = circuit_intact && se050_intact && crypto_intact;
    
    if (!tamper_state.is_intact) {
        tamper_state.tamper_count++;
        printf("TAMPER: Seal broken! Revealing keys for owner - Count: %d\n", tamper_state.tamper_count);
        
        // Reveal keys in filesystem for owner to sweep
        tamper_reveal_keys_to_filesystem();
    } else {
        printf("TAMPER: Device integrity verified - keys remain sealed\n");
    }
    
    return tamper_state;
}

void tamper_seal_device(void) {
    printf("TAMPER: Sealing device\n");
    
    // Create cryptographic seal using SE050
    if (create_cryptographic_seal()) {
        tamper_state.is_intact = true;
        tamper_state.tamper_count = 0;
        
        // Update device state to sealed
        flash_write_device_state(DEVICE_STATE_SEALED);
        
        printf("TAMPER: Device sealed successfully\n");
        led_set_state(LED_STATE_SEALED);
    } else {
        printf("TAMPER: Sealing failed\n");
        led_set_state(LED_STATE_UNSEALED);
    }
}

bool tamper_is_device_compromised(void) {
    return !tamper_state.is_intact;
}

// Internal helper functions

bool se050_check_tamper_status(void) {
    // Query SE050 tamper status registers
    uint8_t tamper_query[] = {0x80, 0x06, 0x00, 0x00};
    
    int result = i2c_write_blocking(i2c1, SE050_I2C_ADDR, tamper_query, sizeof(tamper_query), false);
    if (result < 0) {
        return false;
    }
    
    delay_ms(10);
    
    result = i2c_read_blocking(i2c1, SE050_I2C_ADDR, rx_buffer, 8, false);
    if (result > 0) {
        // Check tamper status byte (simplified)
        return (rx_buffer[1] & 0x01) == 0;  // Bit 0 = tamper detected
    }
    
    return false;
}

bool verify_cryptographic_seal(void) {
    // Verify that the cryptographic seal is intact
    // This involves checking a signature or HMAC stored during sealing
    
    uint8_t seal_data[32];
    if (!flash_read_seal_data(seal_data, sizeof(seal_data))) {
        printf("TAMPER: No seal data found\n");
        return false;
    }
    
    // Verify seal using SE050 cryptographic operations
    uint8_t expected_seal[32];
    if (!se050_compute_seal_verification(expected_seal)) {
        return false;
    }
    
    // Compare seals
    return memcmp(seal_data, expected_seal, 32) == 0;
}

bool create_cryptographic_seal(void) {
    // Create tamper-evident cryptographic seal
    uint8_t seal_data[32];
    
    // Generate seal using device-specific data + SE050 crypto
    if (!se050_generate_device_seal(seal_data)) {
        return false;
    }
    
    // Store seal in flash
    return flash_write_seal_data(seal_data, sizeof(seal_data));
}

void tamper_reveal_keys_to_filesystem(void) {
    printf("TAMPER: Revealing Bitcoin keys to filesystem for owner\n");
    
    // Load the stored keys
    bitcoin_keys_t keys;
    if (flash_read_keys(&keys)) {
        // Mark keys as revealed
        keys.keys_revealed = true;
        flash_write_keys(&keys);
        
        // Create plaintext files with key data for owner to sweep
        usb_create_key_reveal_files(&keys);
        
        printf("TAMPER: Keys revealed - owner can now sweep Bitcoin\n");
    }
    
    // Set device state to compromised/revealed
    flash_write_device_state(DEVICE_STATE_COMPROMISED);
}

// Flash storage helpers for seal data
bool flash_write_seal_data(const uint8_t *seal_data, size_t len) {
    // Write seal to a dedicated flash sector
    // Implementation depends on flash layout
    return true;  // Placeholder
}

bool flash_read_seal_data(uint8_t *seal_data, size_t len) {
    // Read seal from flash
    // Implementation depends on flash layout  
    return true;  // Placeholder
}