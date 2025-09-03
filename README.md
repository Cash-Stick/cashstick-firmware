# CashStick Firmware 🔐

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![Build Status](https://img.shields.io/badge/build-passing-brightgreen.svg)](https://github.com/Cash-Stick/cashstick-firmware)
[![Hardware: RP2040](https://img.shields.io/badge/Hardware-RP2040-blue.svg)](https://www.raspberrypi.com/products/rp2040/)
[![Security: SE050](https://img.shields.io/badge/Security-SE050-red.svg)](https://www.nxp.com/products/security-and-authentication/authentication/edgelock-se050-plug-trust-secure-element-family:SE050)
[![Bitcoin](https://img.shields.io/badge/Bitcoin-secp256k1-orange.svg)](https://en.bitcoin.it/wiki/Secp256k1)

> **Open-source firmware for CashStick Bitcoin bearer device combining RP2040 microcontroller with SE050 secure element for hardware-level Bitcoin security.**

<div align="center">
  <img src="https://raw.githubusercontent.com/Cash-Stick/cashstick-firmware/main/docs/images/cashstick-device.jpg" alt="CashStick Bitcoin Bearer Device" width="600"/>
</div>

## 🚀 Features

- **🔐 Hardware Security**: SE050 secure element with EAL 6+ certification
- **₿ Native Bitcoin Support**: secp256k1 key generation using SE050 built-in cryptography
- **🛡️ Tamper-Evident Design**: Physical security reveals keys when seal broken for Bitcoin recovery
- **💡 LED Status Indicators**: Blue (new), Green (sealed), Red (compromised), Yellow (busy)
- **📱 Drag-and-Drop Installation**: No technical knowledge required (.uf2 files)
- **🔍 Open Source & Auditable**: Complete transparency for security verification
- **⚡ USB 2.0 Interface**: Mass storage mode and communication

## ⚡ Quick Start

### 1. Firmware Installation
1. **Download** latest firmware (.uf2 file) from [Releases](https://github.com/Cash-Stick/cashstick-firmware/releases)
2. **Hold BOOT button** while connecting CashStick to USB port
3. **Drag and drop** firmware file to "RPI-RP2" drive that appears
4. **Device reboots** automatically with new firmware installed

### 2. Key Generation
1. **Connect** CashStick after firmware installation
2. **LED shows blue** indicating new/uninitialized state
3. **Press TEST button** to generate Bitcoin keys using SE050
4. **LED turns green** when keys generated and device sealed
5. **Bitcoin address** appears in virtual files for funding

### 3. Using Your CashStick

**Normal Operation:**
- Device shows **green LED** when secure and intact
- Access Bitcoin address through USB mass storage files
- Fund the address with Bitcoin from any wallet

**After Tamper (Key Revelation):**
- Physical seal break triggers tamper detection
- **LED turns red** indicating compromise
- Private key appears in plaintext files for sweeping
- Import private key to any Bitcoin wallet to access funds

## 🛡️ Security Model

### Tamper-Evident Bearer Instrument

CashStick implements a **bearer instrument** security model:

1. **Sealed State** 🟢
   - Device generates Bitcoin keys using SE050 hardware RNG
   - Only **public key** and Bitcoin address are accessible
   - Private key secured within SE050 tamper-resistant boundary

2. **Tamper Detection** 🔴
   - Physical circuit monitoring detects seal compromise
   - SE050 internal tamper detection provides hardware-level security
   - Cryptographic integrity verification ensures authenticity

3. **Key Revelation** 📋
   - When tamper detected, device **reveals** private key in plaintext files
   - Owner can sweep Bitcoin using any standard wallet
   - Device becomes "spent" after tamper - single-use security

### LED Status Indicators

| Color | State | Meaning |
|-------|-------|---------|
| 🔵 **Blue** | New | Device uninitialized, ready for key generation |
| 🟢 **Green** | Sealed | Keys generated, device secure and intact |
| 🔴 **Red** | Compromised | Tamper detected, keys revealed for sweeping |
| 🟡 **Yellow** | Busy | Processing operation in progress |

## 🔧 Hardware Architecture

### Core Components

| Component | Part Number | Function |
|-----------|-------------|----------|
| **Microcontroller** | RP2040 | Dual ARM Cortex-M0+ @ 133MHz |
| **Secure Element** | SE050 | EAL 6+ certified tamper-resistant storage |
| **LED Indicator** | RGB LED | Status visualization (GPIO16) |
| **Interface** | USB 2.0 | Communication and power |
| **Form Factor** | USB Stick | Portable Bitcoin bearer instrument |

## 🏗️ Build Instructions

### Prerequisites

```bash
# Install ARM toolchain
sudo apt install gcc-arm-none-eabi cmake build-essential

# Clone and set up Pico SDK
git clone https://github.com/raspberrypi/pico-sdk.git --recurse-submodules
export PICO_SDK_PATH=/path/to/pico-sdk
```

### Building Firmware

```bash
# Clone the repository
git clone https://github.com/Cash-Stick/cashstick-firmware.git
cd cashstick-firmware

# Create build directory
mkdir build && cd build

# Configure and build
cmake ..
make -j4
```

### Output Files

- `cashstick_firmware.uf2` - Drag-and-drop installation file
- `cashstick_firmware.elf` - Debug binary with symbols
- `cashstick_firmware.bin` - Raw binary for advanced users

## 🏭 Manufacturing

### PCBway.com Integration

CashStick firmware is designed for seamless manufacturing integration:

1. **Assembled PCBs** delivered with bootloader pre-installed
2. **End users download** firmware from [cashstick.org](https://cashstick.org)
3. **Drag-and-drop installation** requires no technical expertise
4. **Quality assurance** built-in testing and verification

## 🔗 Links

- **Website**: [cashstick.org](https://cashstick.org)
- **Community**: [Telegram @bitcoinmint](https://t.me/bitcoinmint)
- **Support**: [GitHub Issues](https://github.com/Cash-Stick/cashstick-firmware/issues)

## 📄 License

This project is licensed under the **MIT License** - see the [LICENSE](LICENSE) file for details.

### Why MIT License?

We chose MIT License to ensure:
- **Maximum transparency** for security auditing
- **Commercial compatibility** for manufacturing
- **Community adoption** without restrictions
- **Innovation encouragement** in Bitcoin hardware security

---

<div align="center">

**Built with ❤️ for Bitcoin security and self-sovereignty**

[⭐ Star this repository](https://github.com/Cash-Stick/cashstick-firmware) if you find it useful!

</div>