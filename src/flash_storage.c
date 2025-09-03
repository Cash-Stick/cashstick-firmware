#include "cashstick.h"
#include "hardware/flash.h"
#include "hardware/sync.h"

// Flash memory layout
#define FLASH_TARGET_OFFSET (256 * 1024)  // 256KB offset for user data
#define FLASH_SECTOR_SIZE 4096
#define KEYS_SECTOR_OFFSET 0
#define STATE_SECTOR_OFFSET 4096
#define SEAL_SECTOR_OFFSET 8192

// Calculate flash addresses
#define KEYS_FLASH_ADDR (XIP_BASE + FLASH_TARGET_OFFSET + KEYS_SECTOR_OFFSET)
#define STATE_FLASH_ADDR (XIP_BASE + FLASH_TARGET_OFFSET + STATE_SECTOR_OFFSET)
#define SEAL_FLASH_ADDR (XIP_BASE + FLASH_TARGET_OFFSET + SEAL_SECTOR_OFFSET)

// Storage structures with checksums
typedef struct {
    bitcoin_keys_t keys;
    uint32_t checksum;
    uint32_t magic;  // Validation marker
} stored_keys_t;

typedef struct {
    device_state_t state;
    uint32_t timestamp;
    uint32_t checksum;
    uint32_t magic;
} stored_state_t;

#define KEYS_MAGIC 0xBTC12345
#define STATE_MAGIC 0xDEV12345

// Internal functions
static uint32_t calculate_checksum(const uint8_t *data, size_t len);
static bool flash_write_sector(uint32_t offset, const uint8_t *data, size_t len);
static bool flash_read_sector(uint32_t offset, uint8_t *data, size_t len);

bool flash_write_keys(const bitcoin_keys_t *keys) {
    if (!keys) {
        return false;
    }
    
    stored_keys_t stored_keys = {0};
    stored_keys.keys = *keys;
    stored_keys.magic = KEYS_MAGIC;
    stored_keys.checksum = calculate_checksum((uint8_t*)&stored_keys.keys, sizeof(bitcoin_keys_t));
    
    printf("FLASH: Writing Bitcoin keys\n");
    
    bool success = flash_write_sector(KEYS_SECTOR_OFFSET, (uint8_t*)&stored_keys, sizeof(stored_keys));
    
    if (success) {
        printf("FLASH: Keys written successfully\n");
    } else {
        printf("FLASH: Failed to write keys\n");
    }
    
    return success;
}

bool flash_read_keys(bitcoin_keys_t *keys) {
    if (!keys) {
        return false;
    }
    
    stored_keys_t stored_keys = {0};
    
    if (!flash_read_sector(KEYS_SECTOR_OFFSET, (uint8_t*)&stored_keys, sizeof(stored_keys))) {
        printf("FLASH: Failed to read keys sector\n");
        return false;
    }
    
    // Validate magic number
    if (stored_keys.magic != KEYS_MAGIC) {
        printf("FLASH: Invalid keys magic number\n");
        return false;
    }
    
    // Validate checksum
    uint32_t calculated_checksum = calculate_checksum((uint8_t*)&stored_keys.keys, sizeof(bitcoin_keys_t));
    if (stored_keys.checksum != calculated_checksum) {
        printf("FLASH: Keys checksum mismatch\n");
        return false;
    }
    
    *keys = stored_keys.keys;
    printf("FLASH: Keys read successfully\n");
    
    return true;
}

bool flash_write_device_state(device_state_t state) {
    stored_state_t stored_state = {0};
    stored_state.state = state;
    stored_state.timestamp = get_system_time_ms();
    stored_state.magic = STATE_MAGIC;
    stored_state.checksum = calculate_checksum((uint8_t*)&stored_state.state, sizeof(device_state_t) + sizeof(uint32_t));
    
    printf("FLASH: Writing device state: %d\n", state);
    
    bool success = flash_write_sector(STATE_SECTOR_OFFSET, (uint8_t*)&stored_state, sizeof(stored_state));
    
    if (success) {
        printf("FLASH: Device state written successfully\n");
    } else {
        printf("FLASH: Failed to write device state\n");
    }
    
    return success;
}

device_state_t flash_read_device_state(void) {
    stored_state_t stored_state = {0};
    
    if (!flash_read_sector(STATE_SECTOR_OFFSET, (uint8_t*)&stored_state, sizeof(stored_state))) {
        printf("FLASH: Failed to read state sector, defaulting to NEW\n");
        return DEVICE_STATE_NEW;
    }
    
    // Validate magic number
    if (stored_state.magic != STATE_MAGIC) {
        printf("FLASH: Invalid state magic number, defaulting to NEW\n");
        return DEVICE_STATE_NEW;
    }
    
    // Validate checksum
    uint32_t calculated_checksum = calculate_checksum((uint8_t*)&stored_state.state, sizeof(device_state_t) + sizeof(uint32_t));
    if (stored_state.checksum != calculated_checksum) {
        printf("FLASH: State checksum mismatch, defaulting to NEW\n");
        return DEVICE_STATE_NEW;
    }
    
    printf("FLASH: Device state read: %d (timestamp: %d)\n", stored_state.state, stored_state.timestamp);
    
    return stored_state.state;
}

// Tamper seal storage functions
bool flash_write_seal_data(const uint8_t *seal_data, size_t len) {
    if (!seal_data || len > FLASH_SECTOR_SIZE - 8) {  // Leave room for metadata
        return false;
    }
    
    // Add magic number and checksum
    uint8_t sector_data[FLASH_SECTOR_SIZE] = {0};
    uint32_t magic = 0x5EAL1234;
    uint32_t checksum = calculate_checksum(seal_data, len);
    
    memcpy(sector_data, &magic, sizeof(magic));
    memcpy(sector_data + 4, &checksum, sizeof(checksum));
    memcpy(sector_data + 8, seal_data, len);
    
    printf("FLASH: Writing tamper seal data\n");
    
    return flash_write_sector(SEAL_SECTOR_OFFSET, sector_data, FLASH_SECTOR_SIZE);
}

bool flash_read_seal_data(uint8_t *seal_data, size_t len) {
    if (!seal_data) {
        return false;
    }
    
    uint8_t sector_data[FLASH_SECTOR_SIZE];
    
    if (!flash_read_sector(SEAL_SECTOR_OFFSET, sector_data, FLASH_SECTOR_SIZE)) {
        return false;
    }
    
    // Validate magic number
    uint32_t magic;
    memcpy(&magic, sector_data, sizeof(magic));
    if (magic != 0x5EAL1234) {
        printf("FLASH: Invalid seal magic number\n");
        return false;
    }
    
    // Validate checksum
    uint32_t stored_checksum, calculated_checksum;
    memcpy(&stored_checksum, sector_data + 4, sizeof(stored_checksum));
    calculated_checksum = calculate_checksum(sector_data + 8, len);
    
    if (stored_checksum != calculated_checksum) {
        printf("FLASH: Seal checksum mismatch\n");
        return false;
    }
    
    memcpy(seal_data, sector_data + 8, len);
    printf("FLASH: Seal data read successfully\n");
    
    return true;
}

// Internal helper functions

static uint32_t calculate_checksum(const uint8_t *data, size_t len) {
    uint32_t checksum = 0;
    
    for (size_t i = 0; i < len; i++) {
        checksum ^= data[i];
        checksum = (checksum << 1) | (checksum >> 31);  // Rotate left
    }
    
    return checksum;
}

static bool flash_write_sector(uint32_t offset, const uint8_t *data, size_t len) {
    if (len > FLASH_SECTOR_SIZE) {
        return false;
    }
    
    // Disable interrupts during flash operations
    uint32_t ints = save_and_disable_interrupts();
    
    // Erase the sector first
    flash_range_erase(FLASH_TARGET_OFFSET + offset, FLASH_SECTOR_SIZE);
    
    // Write the data
    flash_range_program(FLASH_TARGET_OFFSET + offset, data, FLASH_SECTOR_SIZE);
    
    // Restore interrupts
    restore_interrupts(ints);
    
    // Verify write
    const uint8_t *flash_ptr = (const uint8_t*)(XIP_BASE + FLASH_TARGET_OFFSET + offset);
    bool verify_success = (memcmp(flash_ptr, data, len) == 0);
    
    if (!verify_success) {
        printf("FLASH: Write verification failed\n");
    }
    
    return verify_success;
}

static bool flash_read_sector(uint32_t offset, uint8_t *data, size_t len) {
    if (len > FLASH_SECTOR_SIZE) {
        return false;
    }
    
    const uint8_t *flash_ptr = (const uint8_t*)(XIP_BASE + FLASH_TARGET_OFFSET + offset);
    memcpy(data, flash_ptr, len);
    
    return true;
}

// Utility functions
uint32_t get_system_time_ms(void) {
    return to_ms_since_boot(get_absolute_time());
}

void delay_ms(uint32_t ms) {
    sleep_ms(ms);
}