cmake_minimum_required(VERSION 3.13)

# Set board type
set(PICO_BOARD pico)

# Include the Pico SDK
include(pico-sdk/pico_sdk_init.cmake)

# Project name
project(cashstick_firmware C CXX ASM)

# Set standards
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Initialize the Pico SDK
pico_sdk_init()

# Add the main executable
add_executable(cashstick_firmware
    src/main.c
    src/led_control.c
    src/se050_interface.c
    src/usb_handler.c
    src/button_handler.c
    src/bitcoin_wallet.c
    src/tamper_detection.c
)

# Include directories
target_include_directories(cashstick_firmware PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# Link libraries
target_link_libraries(cashstick_firmware 
    pico_stdlib
    pico_unique_id
    pico_bootsel_via_double_reset
    pico_multicore
    hardware_gpio
    hardware_i2c
    hardware_uart
    hardware_flash
    hardware_watchdog
    tinyusb_device
)

# Enable USB output
pico_enable_stdio_usb(cashstick_firmware 1)
pico_enable_stdio_uart(cashstick_firmware 0)

# Create UF2 output for drag-and-drop installation
pico_add_extra_outputs(cashstick_firmware)

# Set linker script for custom memory layout if needed
# pico_set_linker_script(cashstick_firmware ${CMAKE_CURRENT_SOURCE_DIR}/memmap_custom.ld)