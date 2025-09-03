#include "cashstick.h"

// Global wallet state
static bitcoin_keys_t wallet_keys = {0};
static bool wallet_initialized = false;

bool wallet_generate_new_keys(void) {
    printf("WALLET: Generating new Bitcoin keys\n");
    
    led_set_state(LED_STATE_BUSY);
    
    // Use SE050 to generate cryptographically secure keys
    if (!se050_generate_bitcoin_keys(&wallet_keys)) {
        printf("WALLET: Key generation failed\n");
        led_set_state(LED_STATE_UNSEALED);
        return false;
    }
    
    // Store keys securely in flash
    if (!flash_write_keys(&wallet_keys)) {
        printf("WALLET: Failed to store keys\n");
        led_set_state(LED_STATE_UNSEALED);
        return false;
    }
    
    wallet_initialized = true;
    printf("WALLET: New Bitcoin address: %s\n", wallet_keys.address);
    
    // Seal the device after key generation
    tamper_seal_device();
    
    led_set_state(LED_STATE_SEALED);
    return true;
}

bool wallet_get_address(char *address_out, size_t max_len) {
    if (!wallet_initialized) {
        // Try to load keys from flash
        if (!flash_read_keys(&wallet_keys)) {
            return false;
        }
        wallet_initialized = true;
    }
    
    if (!wallet_keys.is_sealed || strlen(wallet_keys.address) == 0) {
        return false;
    }
    
    strncpy(address_out, wallet_keys.address, max_len - 1);
    address_out[max_len - 1] = '\0';
    
    return true;
}

bool wallet_reveal_private_key(uint8_t *privkey_out) {
    if (!wallet_initialized || !privkey_out) {
        return false;
    }
    
    // Only reveal private key if device has been tampered (seal broken)
    tamper_status_t tamper_status = tamper_check_integrity();
    if (tamper_status.is_intact) {
        printf("WALLET: Device still sealed - private key not accessible\n");
        return false;
    }
    
    printf("WALLET: Revealing private key for owner to sweep\n");
    
    // Copy private key for owner
    memcpy(privkey_out, wallet_keys.private_key, 32);
    wallet_keys.keys_revealed = true;
    flash_write_keys(&wallet_keys);
    
    return true;
}

bool wallet_are_keys_revealed(void) {
    return wallet_keys.keys_revealed;
}

bool wallet_export_public_key(uint8_t *pubkey_out) {
    if (!wallet_initialized || !wallet_keys.is_sealed) {
        return false;
    }
    
    memcpy(pubkey_out, wallet_keys.public_key, 33);
    return true;
}

bool wallet_verify_signature(const uint8_t *message, const uint8_t *signature) {
    // Verify a signature against the wallet's public key
    // This would implement ECDSA verification
    
    if (!wallet_initialized) {
        return false;
    }
    
    // Use SE050 to verify signature (if supported)
    // Otherwise implement software ECDSA verification
    
    return true;  // Placeholder
}

bool wallet_is_initialized(void) {
    return wallet_initialized && wallet_keys.is_sealed;
}

void wallet_get_status(char *status_json, size_t max_len) {
    if (!status_json || max_len < 100) {
        return;
    }
    
    tamper_status_t tamper_status = tamper_check_integrity();
    
    snprintf(status_json, max_len,
        "{"
        "\"initialized\":%s,"
        "\"sealed\":%s,"
        "\"address\":\"%s\","
        "\"tamper_intact\":%s,"
        "\"tamper_count\":%d"
        "}",
        wallet_initialized ? "true" : "false",
        wallet_keys.is_sealed ? "true" : "false",
        wallet_keys.is_sealed ? wallet_keys.address : "",
        tamper_status.is_intact ? "true" : "false",
        tamper_status.tamper_count
    );
}