#include "cashstick.h"
#include "pico/bootrom.h"

// USB device state
static bool usb_connected = false;
static bool mass_storage_active = false;

void usb_init(void) {
    // USB is initialized by pico_sdk automatically
    usb_connected = false;
    mass_storage_active = false;
    
    printf("USB: Interface initialized\n");
}

bool usb_is_connected(void) {
    // Check USB connection status
    // This would check the actual USB state from TinyUSB
    return usb_connected;
}

void usb_mass_storage_mode(void) {
    printf("USB: Entering mass storage mode for firmware update\n");
    
    led_set_state(LED_STATE_BUSY);
    mass_storage_active = true;
    
    // Create virtual files for drag-and-drop interface
    usb_create_virtual_filesystem();
    
    // Wait in mass storage mode until firmware is updated
    while (mass_storage_active) {
        usb_handle_mass_storage_operations();
        delay_ms(100);
    }
}

void usb_handle_commands(void) {
    // Handle USB commands when device is in normal operation mode
    if (!usb_connected) {
        return;
    }
    
    // Example command protocol:
    // - Get device status
    // - Get Bitcoin address  
    // - Sign transaction
    // - Check tamper status
    
    // This would implement a simple command/response protocol
    // over USB serial or custom HID interface
}

void usb_create_virtual_filesystem(void) {
    // Create virtual files that appear when device is connected
    // These files allow drag-and-drop firmware installation
    
    printf("USB: Creating virtual filesystem\n");
    
    // Virtual files:
    // - README.txt: Instructions for firmware installation
    // - device_info.txt: Device serial number and status
    // - address.txt: Bitcoin address (if keys are generated)
    // - status.txt: Tamper and security status
    
    // Implementation would create TinyUSB mass storage device
    // with these virtual files accessible from host computer
}

void usb_handle_mass_storage_operations(void) {
    // Handle file operations in mass storage mode
    
    // Check for firmware file being written
    if (usb_check_for_firmware_file()) {
        printf("USB: New firmware detected\n");
        
        // Flash new firmware (this would be handled by bootloader)
        if (usb_install_firmware()) {
            printf("USB: Firmware installation successful\n");
            
            // After successful firmware install, exit mass storage mode
            // and reboot into normal operation
            mass_storage_active = false;
            
            // Set device to normal operation mode
            led_set_state(LED_STATE_NEW);
            
            // Reboot to new firmware
            watchdog_enable(100, 1);
            while(1);
        }
    }
    
    // Handle file read operations (device status, address, etc.)
    usb_handle_file_reads();
}

bool usb_check_for_firmware_file(void) {
    // Check if a .uf2 firmware file has been written to the device
    // This would interface with TinyUSB mass storage to detect file writes
    return false;  // Placeholder
}

bool usb_install_firmware(void) {
    // Install new firmware using Pico bootloader
    // This would validate the firmware and flash it
    printf("USB: Installing firmware...\n");
    
    // LED feedback during installation
    for (int i = 0; i < 10; i++) {
        led_blink(LED_STATE_BUSY, 200);
        delay_ms(200);
    }
    
    return true;  // Placeholder
}

void usb_handle_file_reads(void) {
    // Handle when host computer reads virtual files
    // This would provide device information back to the host
    
    // Examples of virtual file contents:
    // README.txt: "CashStick Bitcoin Bearer Device v1.0.0\n..."
    // device_info.txt: Device serial, state, tamper status
    // address.txt: Current Bitcoin address if available
}

// USB Serial Communication Functions
void usb_send_response(const char *response) {
    if (usb_connected) {
        printf("%s\n", response);
    }
}

void usb_send_device_status(void) {
    char status_json[512];
    
    snprintf(status_json, sizeof(status_json),
        "{"
        "\"device_id\":\"%08x\","
        "\"state\":%d,"
        "\"tamper_intact\":%s,"
        "\"keys_present\":%s,"
        "\"firmware_version\":\"1.0.0\""
        "}",
        get_device_serial(),
        current_device_state,
        tamper_check_integrity().is_intact ? "true" : "false",
        device_keys.is_sealed ? "true" : "false"
    );
    
    usb_send_response(status_json);
}

// Device identification
void usb_create_key_reveal_files(const bitcoin_keys_t *keys) {
    if (!keys || !keys->keys_revealed) {
        return;
    }
    
    printf("USB: Creating key reveal files for owner\n");
    
    // This creates virtual files in mass storage mode containing:
    // - private_key.txt: Private key in WIF format for import
    // - public_key.txt: Public key for verification  
    // - address.txt: Bitcoin address
    // - sweep_instructions.txt: How to sweep the Bitcoin
    
    // Implementation would create TinyUSB mass storage files
    // containing the revealed key data for the owner
}

uint32_t get_device_serial(void) {
    // Get unique device ID from RP2040
    pico_unique_board_id_t board_id;
    pico_get_unique_board_id(&board_id);
    
    // Convert to 32-bit serial number
    uint32_t serial = 0;
    for (int i = 0; i < 4; i++) {
        serial ^= ((uint32_t*)board_id.id)[i];
    }
    
    return serial;
}