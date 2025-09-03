#include "cashstick.h"

// SE050 communication buffers
static uint8_t tx_buffer[256];
static uint8_t rx_buffer[256];

// SE050 session state
static bool se050_session_open = false;

bool se050_init(void) {
    // Test I2C communication with SE050
    uint8_t test_data = 0x00;
    int result = i2c_write_blocking(i2c1, SE050_I2C_ADDR, &test_data, 1, false);
    
    if (result < 0) {
        printf("SE050: I2C communication failed\n");
        return false;
    }
    
    // Initialize SE050 secure session
    if (!se050_open_session()) {
        printf("SE050: Session initialization failed\n");
        return false;
    }
    
    printf("SE050: Initialized successfully\n");
    return true;
}

bool se050_open_session(void) {
    // Simplified session opening - implement proper SE050 protocol
    // This would involve authentication and secure channel establishment
    
    uint8_t session_cmd[] = {0x80, 0x01, 0x00, 0x00, 0x00}; // Example command
    
    int result = i2c_write_blocking(i2c1, SE050_I2C_ADDR, session_cmd, sizeof(session_cmd), false);
    if (result < 0) {
        return false;
    }
    
    // Read response
    delay_ms(10);
    result = i2c_read_blocking(i2c1, SE050_I2C_ADDR, rx_buffer, 16, false);
    if (result < 0) {
        return false;
    }
    
    se050_session_open = true;
    return true;
}

bool se050_generate_bitcoin_keys(bitcoin_keys_t *keys) {
    if (!se050_session_open || !keys) {
        return false;
    }
    
    led_set_state(LED_STATE_BUSY);
    
    // Command to generate secp256k1 key pair for Bitcoin using SE050 native support
    uint8_t keygen_cmd[] = {
        0x80, 0x02,  // Command: Generate Key Pair
        0x00, 0x01,  // Object ID: Bitcoin key slot
        0x20,        // Key length: 32 bytes
        0x40         // Algorithm: Native secp256k1 (SE050 built-in)
    };
    
    int result = i2c_write_blocking(i2c1, SE050_I2C_ADDR, keygen_cmd, sizeof(keygen_cmd), false);
    if (result < 0) {
        return false;
    }
    
    // Wait for key generation (can take several seconds)
    delay_ms(2000);
    
    // Read generated public key
    result = i2c_read_blocking(i2c1, SE050_I2C_ADDR, rx_buffer, 64, false);
    if (result < 0) {
        return false;
    }
    
    // Extract public key from response (simplified)
    memcpy(keys->public_key, rx_buffer + 1, 33);  // Skip status byte
    
    // Generate Bitcoin address from public key
    if (!bitcoin_pubkey_to_address(keys->public_key, keys->address, sizeof(keys->address))) {
        return false;
    }
    
    // Mark as sealed after key generation
    keys->is_sealed = true;
    
    printf("SE050: Bitcoin keys generated successfully\n");
    printf("Address: %s\n", keys->address);
    
    return true;
}

bool se050_sign_transaction(const uint8_t *hash, uint8_t *signature) {
    if (!se050_session_open || !hash || !signature) {
        return false;
    }
    
    led_set_state(LED_STATE_BUSY);
    
    // Command to sign hash with stored private key
    uint8_t sign_cmd[40] = {
        0x80, 0x03,  // Command: Sign Hash
        0x00, 0x01,  // Key ID
        0x20         // Hash length: 32 bytes
    };
    
    // Append hash to command
    memcpy(sign_cmd + 5, hash, 32);
    
    int result = i2c_write_blocking(i2c1, SE050_I2C_ADDR, sign_cmd, 37, false);
    if (result < 0) {
        return false;
    }
    
    // Wait for signature generation
    delay_ms(500);
    
    // Read signature
    result = i2c_read_blocking(i2c1, SE050_I2C_ADDR, rx_buffer, 80, false);
    if (result < 0) {
        return false;
    }
    
    // Extract signature from response
    memcpy(signature, rx_buffer + 1, 64);  // Skip status byte
    
    printf("SE050: Transaction signed successfully\n");
    return true;
}

bool se050_configure_tamper_detection(void) {
    if (!se050_session_open) {
        return false;
    }
    
    // Configure SE050 tamper detection features
    uint8_t tamper_cmd[] = {
        0x80, 0x05,  // Command: Configure Tamper
        0x01,        // Enable tamper detection
        0x02,        // Tamper response: clear keys
        0xFF         // Sensitivity level: maximum
    };
    
    int result = i2c_write_blocking(i2c1, SE050_I2C_ADDR, tamper_cmd, sizeof(tamper_cmd), false);
    if (result < 0) {
        return false;
    }
    
    delay_ms(100);
    
    printf("SE050: Tamper detection configured\n");
    return true;
}

bool se050_get_device_info(uint8_t *info, size_t *info_len) {
    if (!se050_session_open || !info || !info_len) {
        return false;
    }
    
    // Get SE050 version and device information
    uint8_t info_cmd[] = {0x80, 0x01, 0x00, 0x00, 0x00};
    
    int result = i2c_write_blocking(i2c1, SE050_I2C_ADDR, info_cmd, sizeof(info_cmd), false);
    if (result < 0) {
        return false;
    }
    
    delay_ms(50);
    
    result = i2c_read_blocking(i2c1, SE050_I2C_ADDR, info, *info_len, false);
    if (result > 0) {
        *info_len = result;
        return true;
    }
    
    return false;
}

// Helper function to convert public key to Bitcoin address
bool bitcoin_pubkey_to_address(const uint8_t *pubkey, char *address, size_t addr_len) {
    // Simplified Bitcoin address generation (Bech32 format)
    // In production, implement full Bitcoin address generation with proper hashing
    
    if (!pubkey || !address || addr_len < 42) {
        return false;
    }
    
    // Generate a demo Bitcoin address for testing
    // Format: bc1 + base32 encoded public key hash
    snprintf(address, addr_len, "bc1q%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x",
        pubkey[0], pubkey[1], pubkey[2], pubkey[3], pubkey[4], pubkey[5], pubkey[6], pubkey[7],
        pubkey[8], pubkey[9], pubkey[10], pubkey[11], pubkey[12], pubkey[13], pubkey[14], pubkey[15]);
    
    return true;
}