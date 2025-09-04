# ⚙️ Technical Specification Document (TSD)
## CashStick - Bitcoin Bearer Device

<div align="center">

![CashStick Technical](https://img.shields.io/badge/CashStick-Technical%20Implementation-green?style=for-the-badge&logo=microchip&logoColor=white)

**Project Name:** CashStick  
**Product Type:** Bitcoin Bearer Device (USB form)  
**Version:** v1.0  
**Date:** September 3, 2025  
**Team:** Manager: Kongzi, Hardware: Raheel, Firmware: Sajjad

</div>

---

## 🎯 Implementation Goal

Build a **simple, cheap Bitcoin bearer device** that implements the Olympic torch concept:
- **🛠️ Easy to build** - Under $5 cost with basic components
- **💻 Simple firmware** - Drag & drop flashing to bootloader
- **🔄 Multiple transfers** - Secure handoff between people
- **💸 Final spend** - Snap tab to reveal private key

---

## 🏗️ Simple System Design

### Core Components (Minimal BOM)
```
┌─────────────────────────────────────────────────────────┐
│                  CashStick Device                       │
├─────────────────┬───────────────────┬───────────────────┤
│   USB Plug      │    RP2040 MCU     │  SE050 Chip       │
│                 │                   │                   │
│ • Shows as      │ • Runs firmware   │ • Stores Bitcoin  │
│   USB drive     │ • Controls LED    │   private key     │
│ • Address file  │ • I2C master      │ • Secure random   │
├─────────────────┼───────────────────┼───────────────────┤
│  Snap-off Tab   │   Power & Clock   │  Status LED       │
│                 │                   │                   │
│ • Extended PCB  │ • USB power       │ • Green = sealed  │
│ • Breaks to     │ • 12MHz crystal   │ • Red = tampered  │
│   reveal key    │ • 3.3V regulator  │ • Blue = new      │
└─────────────────┴───────────────────┴───────────────────┘
```

### 💰 Target Cost Breakdown (10k+ units)
- RP2040 MCU: **$0.70**
- SE050 Secure Element: **$1.50** 
- Passive components: **$0.20**
- PCB (2-layer): **$0.25**
- Assembly: **$0.30**
- **Total: $2.95** (under $3 target)

---

## 🔧 Hardware Design

### PCB Layout (Simple 2-Layer Design)
```
USB-A Connector                    Total Length: 55mm
┌─────────────────────────────────────────┐ ← PCB Width: 18mm  
│  [USB Pins] [Power] [Crystal]           │
│                                         │
│  [RP2040]    [SE050]    [LED]           │ ← Component Area
│                                         │
│  [3.3V Reg]  [Caps]    [Resistors]      │   30mm
│                                         │
├═════════════════════════════════════════┤ ← V-Score Break Line
│          SNAP-OFF TAB                   │
│                                         │
│   🔑 Private Key (Hidden)               │ ← Tab Area  
│   📱 QR Code (Hidden)                   │   15mm
│                                         │
└─────────────────────────────────────────┘
```

### Snap-off Tab Mechanism
- **V-Score Depth:** 0.4mm each side (50% PCB thickness)
- **Break Force:** 3-4 kg (intentional action required)
- **Key Revelation:** Private key revealed in file system, visible after snap
- **Format:** Both QR code and text for easy sweeping

### Power System (Simple & Cheap)
- **Input:** USB 5V 
- **Regulator:** Simple linear LDO → 3.3V
- **Current:** <50mA total (well under USB limit)
- **No switching regulator needed** (keeps cost down)

---

## 💻 Firmware Design (Keep It Simple)

### User Experience Flow
```
1. 🛒 Buy device → Ships with bootloader only
2. 💾 Flash firmware → Drag & drop UF2 file to device  
3. 🔘 Press TEST → Generate Bitcoin wallet on first boot
4. 📁 Check address → Open USB drive, see ADDRESS.TXT
5. 💰 Load Bitcoin → Send to the address
6. 🔄 Pass device → Hand to next person (Olympic torch)
7. ✅ Verify sealed → Recipient checks LED is green  
8. 💸 Final spend → Snap tab, private key appears
```

### Simple Firmware Features
- **USB Mass Storage** - Shows up as drive, no special drivers
- **One File** - `ADDRESS.TXT` contains the Bitcoin address
- **LED Status** - Green=sealed, Red=tampered, Blue=new device  
- **Drag & Drop Flash** - Standard RP2040 bootloader (UF2 files)
- **BOOT/TEST Button** - Initially BOOT mode, becomes TEST after firmware flash
- **No Complex UI** - Just works when plugged in

### BOOT/TEST Button Functionality
- **Production State:** Button works as standard RP2040 BOOT button for firmware flashing
- **After Firmware Flash:** Button becomes **TEST** button with new functions and prevents unauthorized flashing:
  - **First Boot:** Press TEST button to generate Bitcoin wallet 
  - **Later Use:** Press TEST button to test circuit and verify device functionality
  - **Visual Feedback:** LED blinks to confirm TEST button operations

### Core Functions
```c
void main() {
    // 1. Initialize hardware
    hardware_init();
    se050_init();
    
    // 2. Check if first boot (no wallet yet)
    if (first_boot) {
        set_led_blue();  // New device, waiting for TEST button
        wait_for_test_button_press(); // User must press TEST to generate wallet
        generate_bitcoin_wallet();
    }
    
    // 3. Check tamper status
    if (check_tamper_tab()) {
        set_led_red();   // Tampered
        reveal_private_key();
    } else {
        set_led_green(); // Sealed/safe
        show_address_only();
    }
    
    // 4. Start USB mass storage
    start_usb_drive();
    
    // 5. Main loop
    while(1) {
        handle_usb();
        handle_test_button();  // TEST button for circuit testing
        update_led();
    }
}
```

### File Contents When Plugged In
**Before Tab Snap (Sealed):**
```
📁 CASHSTICK/
└── ADDRESS.TXT  → "bc1qxy2kgdygjrsqtzq2n0yrf2493p83kkfjhx0wlh"
```

**After Tab Snap (Private Key Revealed):**
```  
📁 CASHSTICK/
├── ADDRESS.TXT  → "bc1qxy2kgdygjrsqtzq2n0yrf2493p83kkfjhx0wlh"
└── PRIVATE.TXT  → "L1aW4aubDFB7yfras2S1mN3bqg9nwySY8nkoLmJebSxMuHB5gQeY"
```

---

## 🔐 Security Implementation

### SE050 Integration (Simple Setup)
- **I2C Connection** - Two wires from RP2040 to SE050
- **Key Generation** - SE050 creates random Bitcoin private key
- **Key Protection** - Private key locked inside SE050, can't be read
- **Address Derivation** - Public key generates Bitcoin address
- **Tamper Detection** - SE050 reports if physically attacked

### Bitcoin Key Management
```c
// First boot - create new wallet
1. SE050 generates random private key (secp256k1)
2. Derive public key from private key  
3. Calculate Bitcoin address from public key
4. Lock private key in SE050 (read-only)
5. Store address in RP2040 flash for USB file
```

### Tamper Protection 
- **Before Snap:** Private key secure in SE050, only address visible
- **GPIO Detection:** Tab connected to ground via resistor
- **After Snap:** Tab breaks, GPIO goes high, reveals private key
- **Visual Evidence:** PCB trace visibly broken, can't be hidden

---

## 💡 Status LED (Simple 3-Color)

### LED States
| Color | Meaning | When |
|-------|---------|------|
| 🔵 **Blue** | New device | No wallet created yet |
| 🟢 **Green** | Sealed & loaded | Device has Bitcoin, tab intact |
| 🔴 **Red** | Tampered/Spent | Tab broken, private key revealed |

### Simple Circuit
- **One RGB LED** connected to 3 RP2040 GPIO pins
- **Current limiting resistors** (220Ω each)
- **Software PWM** for color control
- **Always-on indication** so user knows device status

---

## 🏭 Simple Manufacturing

### DIY-Friendly Design
- **4-Layer PCB** - Cheap to manufacture anywhere
- **Large components** - 0603 resistors/caps (not tiny 0402)
- **Standard parts** - All components available from Digikey/Mouser
- **No BGA chips** - Everything is QFN or larger

### Essential Components Only
| Part | Price (1k units) | Description |
|------|------------------|-------------|
| RP2040 | $0.70 | Main controller |
| SE050 | $1.50 | Security chip |
| 12MHz Crystal | $0.05 | Clock source |
| 3.3V Regulator | $0.08 | Power supply |
| RGB LED | $0.10 | Status indicator |
| Resistors/Caps | $0.15 | Supporting parts |
| PCB (2-layer) | $0.25 | Circuit board |
| **Total** | **$2.95** | **Under $3 target!** |

### Production Flow
1. **🏭 Order PCBs** - Send gerber files to any PCB fab
2. **📦 Source components** - Standard parts from distributors  
3. **🔧 Assembly** - SMT pick & place + reflow oven
4. **💻 Bootloader Only** - Flash only RP2040 bootloader (safety requirement)
5. **🔍 Test** - Basic hardware test (power, USB enumeration as bootloader)
6. **📋 QC** - Visual inspection of snap-off tab v-score
7. **📦 Package** - Anti-static bag with firmware download instructions

**Note:** Device ships with **bootloader only** - users flash firmware locally for security

---

## 🧪 Testing (Keep It Simple)

### Basic Production Tests
1. **🔌 Power On** - Device enumerates as USB drive
2. **💡 LED Check** - Blue light (new device)
3. **📁 File Check** - ADDRESS.TXT appears with valid Bitcoin address
4. **🔐 SE050 Test** - Security chip responds to I2C commands
5. **🔨 Tab Test** - V-score breaks cleanly with 3-4 kg force

### User Acceptance Testing
- **Plug into Windows laptop** - Shows up as drive, no driver needed
- **Plug into MacBook** - Same behavior, universal compatibility
- **Read address file** - Valid Bitcoin address format
- **Send small amount** - 0.001 BTC, verify on block explorer
- **Pass to friend** - They verify LED is green (sealed)
- **Final spend** - Snap tab, private key appears, sweep works

---

## 📅 Development Plan

### Phase 1: Prototype (6 weeks)
- **Week 1-2:** PCB design, order prototype boards
- **Week 3-4:** Basic firmware (USB mass storage + SE050)
- **Week 5-6:** Test prototype, verify core functionality

### Phase 2: Production (8 weeks)  
- **Week 1-2:** Refine design, order production PCBs
- **Week 3-4:** Production firmware with all features
- **Week 5-6:** Assembly setup, test procedures
- **Week 7-8:** First production run, quality validation

### Phase 3: Release (2 weeks)
- **Week 1:** Final testing, documentation
- **Week 2:** Open source release, first shipments

---

<div align="center">

**⚙️ CashStick Technical Implementation Guide**

[![GitHub Repository](https://img.shields.io/badge/GitHub-Cash--Stick%2Fcashstick--firmware-blue?style=for-the-badge&logo=github)](https://github.com/Cash-Stick/cashstick-firmware)

*Detailed technical specifications for a secure physical Bitcoin bearer device* 🔐

**Team:** 👨‍💼 Manager: Kongzi | ⚡ Hardware: Raheel | 💻 Firmware: Sajjad

</div>
