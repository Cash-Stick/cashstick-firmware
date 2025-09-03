# Technical Specification Document (TSD)

**Project Name:** CashStick  
**Product Type:** Bitcoin Bearer Device (USB form)  
**Version:** v1.0  
**Date:** September 3, 2025  
**Team:** Manager: Kongzi, Hardware: Raheel, Firmware: Sajjad

---

## 1. Introduction

### 1.1 Purpose

This Technical Specification Document (TSD) defines the detailed technical implementation requirements for CashStick, a Bitcoin bearer device based on the approved User Requirements Document (URD) v1.0. This document serves as the definitive guide for hardware design, firmware development, and manufacturing implementation.

### 1.2 Scope

This TSD covers:
- System architecture using RP2040 microcontroller and NXP SE050 secure element
- Hardware design including snap-off tab mechanism
- Firmware architecture and USB mass storage implementation
- Security implementation and cryptographic operations
- Manufacturing specifications and testing procedures
- Component specifications and bill of materials

### 1.3 Reference Documents

- CashStick User Requirements Document v1.0
- RP2040 Datasheet (Raspberry Pi Foundation)
- NXP SE050 Secure Element Specification
- USB 2.0 Specification
- Bitcoin BIP-32/39/44 Standards

---

## 2. System Architecture

### 2.1 High-Level Architecture

```
┌─────────────────────────────────────────────────────────┐
│                    CashStick System                     │
├─────────────────┬───────────────────┬───────────────────┤
│   USB Interface │    RP2040 MCU     │  NXP SE050 SE     │
│                 │                   │                   │
│ • Mass Storage  │ • USB Device      │ • Private Key     │
│ • Device Class  │ • I2C Master      │ • Crypto Ops      │
│ • File System   │ • GPIO Control    │ • Tamper Detect   │
│                 │ • LED Control     │                   │
├─────────────────┼───────────────────┼───────────────────┤
│  Physical Layer │   Support Chips   │  Visual/Security  │
│                 │                   │                   │
│ • USB-A Conn.   │ • Crystal Osc     │ • RGB LED         │
│ • Snap-off Tab  │ • Power Reg       │ • Tamper Seal     │
│ • PCB Substrate │ • Decoupling      │                   │
└─────────────────┴───────────────────┴───────────────────┘
```

### 2.2 Core Components

#### 2.2.1 Primary Processing Unit
- **MCU:** Raspberry Pi RP2040
- **Architecture:** Dual ARM Cortex-M0+ @ 133MHz
- **Flash:** 16MB external QSPI (W25Q128)
- **RAM:** 264KB internal SRAM
- **Package:** QFN-56

#### 2.2.2 Security Element
- **SE:** NXP SE050C2HQ1/Z01DZ
- **Interface:** I2C (up to 3.4MHz)
- **Security:** CC EAL 6+ certified
- **Features:** Hardware RNG, AES, ECC, RSA
- **Package:** HVQFN32

#### 2.2.3 Visual Indicator
- **LED:** RGB LED (Common Anode)
- **Type:** WS2812B-compatible or discrete RGB
- **Control:** PWM via RP2040 GPIO
- **Colors:** Green (sealed), Red (unsealed), Blue (new), Yellow (busy)

---

## 3. Hardware Design Specifications

### 3.1 PCB Design Requirements

#### 3.1.1 Board Dimensions
- **Overall Length:** 55mm (±0.5mm)
- **Overall Width:** 18mm (±0.2mm) 
- **Thickness:** 1.6mm (standard PCB)
- **USB Connector Overhang:** 12mm
- **Snap-off Tab Length:** 15mm

#### 3.1.2 Layer Stack-up
```
Layer 1 (Top):    Component placement, routing
Layer 2 (GND):    Ground plane
Layer 3 (PWR):    Power plane (+3.3V)
Layer 4 (Bottom): Routing, test points
```

#### 3.1.3 PCB Material Specifications
- **Substrate:** FR4, Tg ≥ 170°C
- **Copper Weight:** 1oz (35μm) per layer
- **Surface Finish:** HASL or ENIG
- **Solder Mask:** Orange, matte finish
- **Silkscreen:** White, both sides

### 3.2 Snap-off Tab Design

#### 3.2.1 Physical Implementation
- **Tab Location:** Bottom 15mm of PCB
- **Separation Method:** V-scoring on both sides (0.3mm depth)
- **Break Force:** 3-5 kg (designed for intentional removal)
- **Visual Indicators:** Clear "SNAP TO REVEAL KEY" silkscreen marking

#### 3.2.2 Circuit Protection Implementation
```
Main PCB Section:
┌──────────────────┐
│  RP2040 + SE050  │
│                  │
│  Circuit Trace   │ ──┐
│                  │   │ Protected Section
└──────┬───────────┘   │
       │               │
       │ V-Score Line  │
       │               │
┌──────┴───────────┐   │
│   Snap-off Tab   │ ──┘
│                  │
│                  │ ← Private Key revealed after snap
│                  │
│                  │
└──────────────────┘
```

#### 3.2.3 Key Revelation Mechanism
- **Before Snap:** Private key traces covered by solder mask
- **After Snap:** breaking the seal reveal private key
- **Format:** QR code + human-readable text
- **Protection:** Pin burnoff for permanent evidence.

### 3.3 Component Placement

#### 3.3.1 Main Section Layout
```
USB-A Connector
┌─────────────────────────────────────────┐ ← Top of PCB
│  [USB Data Lines and Power]             │
│                                         │
│  [Crystal]  [RP2040]      [RGB LED]     │
│            [16MB Flash]                 │ 
│                                         │
│  [Power Reg] [SE050] [Decoupling Caps]  │
│                                         │
├═════════════════════════════════════════┤ ← V-Score Line
│             Snap-off Tab                │
│                                         │
│    [Serial QR Code Area]           │
│    [Human Readable Key Text]            │
│                                         │
└─────────────────────────────────────────┘ ← Bottom of PCB
```

### 3.4 Power System Design

#### 3.4.1 Power Requirements
- **Input:** USB VBUS (5V ±5%)
- **System Voltage:** 3.3V regulated
- **Total Current:** <100mA (USB 2.0 compliant)
- **SE050 Current:** <15mA active, <5μA standby
- **RP2040 Current:** <30mA active, <1mA sleep

#### 3.4.2 Power Regulation
- **Regulator:** AMS1117-3.3 or equivalent
- **Input Capacitor:** 10μF tantalum
- **Output Capacitor:** 22μF ceramic + 100nF
- **Ripple:** <50mV peak-to-peak

### 3.5 USB Interface Design

#### 3.5.1 USB Connector
- **Type:** USB-A plug
- **Specifications:** USB 2.0 compliant
- **Shell Material:** Nickel-plated steel
- **Contact Material:** Gold-plated copper

#### 3.5.2 USB Signal Integrity
- **Differential Impedance:** 90Ω ±10%
- **Trace Width:** 0.2mm (with ground plane)
- **Trace Spacing:** 0.15mm
- **Length Matching:** ±0.1mm between D+ and D-

---

## 4. Firmware Architecture

### 4.1 Software Stack

```
┌─────────────────────────────────────────────┐
│           Application Layer                 │
├─────────────────────────────────────────────┤
│  File System  │  Crypto API  │  LED Control │
├─────────────────────────────────────────────┤
│      USB Mass Storage Device Class          │
├─────────────────────────────────────────────┤
│           TinyUSB Stack                     │
├─────────────────────────────────────────────┤
│     Hardware Abstraction Layer (HAL)       │
├─────────────────────────────────────────────┤
│            RP2040 SDK                       │
└─────────────────────────────────────────────┘
```

### 4.2 Core Firmware Modules

#### 4.2.1 USB Mass Storage Implementation
- **Device Class:** Mass Storage (0x08)
- **Protocol:** Bulk-Only Transport (BOT)
- **File System:** FAT12 (minimal overhead)
- **Storage Size:** 1MB virtual storage
- **Read-Only:** Enforced at USB and filesystem level

#### 4.2.2 File System Layout
```
/CASHSTICK (1MB Virtual Drive)
├── ADDRESS.TXT      (Bitcoin address, 35 bytes)
├── ADDRESS.QR.BMP   (QR code bitmap, ~2KB)
├── README.TXT       (Usage instructions, <1KB)
└── [Hidden Area]    (Unused FAT sectors)
```

#### 4.2.3 Secure Element Interface
- **Protocol:** I2C @ 400kHz standard mode
- **Library:** SE05X Middleware from NXP
- **Key Storage:** Private key in SE050 non-volatile memory
- **Operations:** Key generation, digital signatures, random number generation

### 4.3 Security Implementation

#### 4.3.1 Key Management
```c
// Key Generation Process (Manufacturing)
1. SE050_Initialize()
2. SE050_GenerateECCKeyPair(secp256k1, keyID)
3. SE050_ReadPublicKey(keyID) -> Bitcoin Address
4. SE050_LockKeySlot(keyID) -> Read-only access
5. Store address in filesystem
```

#### 4.3.2 Tamper Detection
- **Method:** Periodic SE050 self-test via I2C
- **Frequency:** Every USB enumeration
- **Response:** LED indication (Red = tampered)
- **Logging:** No persistent logging (stateless device)

#### 4.3.3 Snap-off Tab Detection
- **Method:** GPIO input with pull-up resistor
- **Circuit:** Tab removal breaks connection to GND
- **Firmware Response:** 
  - Before snap: Normal operation
  - After snap: Switch to private key revelation mode

### 4.4 USB Device Behavior

#### 4.4.1 Device Enumeration
```
USB Device Descriptor:
- Vendor ID: 0x2E8A (Raspberry Pi Foundation)
- Product ID: 0x0003 (Custom application)
- Device Class: Mass Storage
- String Descriptors: "CashStick", "Bitcoin Bearer Device"
```

#### 4.4.2 Mass Storage Response
- **SCSI Commands:** READ_10, INQUIRY, TEST_UNIT_READY
- **Block Size:** 512 bytes
- **Total Blocks:** 2048 (1MB total)
- **Write Protection:** Always enabled

---

## 5. Secure Element Integration

### 5.1 NXP SE050 Configuration

#### 5.1.1 Physical Interface
- **Connection:** I2C on RP2040 pins GP4 (SDA), GP5 (SCL)
- **Pull-up Resistors:** 4.7kΩ to 3.3V
- **Clock Speed:** 400kHz (standard mode)
- **Power:** Shared 3.3V rail with RP2040

#### 5.1.2 Security Configuration
```
SE050 Applet Configuration:
- Applet Version: 03.XX.XX or later  
- Authentication: Platform SCP03
- Key Storage: 10 ECC key pairs maximum
- CashStick Usage: 1 ECC key pair (secp256k1)
- Lifecycle: Production locked
```

#### 5.1.3 Key Provisioning Process
```
Setup Flow:
1. Plug in device
2. Install firmware
3. Generate Bitcoin key pair (secp256k1)
4. Derive Bitcoin address from public key
5. Lock key slot (prevent deletion/modification)
6. Generate QR code and text representation
7. Store address data in RP2040 flash filesystem
8. Perform final functional test
```

### 5.2 Cryptographic Operations

#### 5.2.1 Supported Algorithms
- **ECC Curves:** secp256k1 (Bitcoin standard)
- **Hash Functions:** SHA-256, SHA-1
- **Random Number Generator:** TRNG (True Random)
- **Key Derivation:** ECDH (for future features)

#### 5.2.2 Bitcoin Address Generation
```
Process Flow:
1. SE050 generates 256-bit private key
2. Calculate public key point (x, y) on secp256k1
3. Compress public key (33 bytes)
4. SHA-256 hash of public key
5. RIPEMD-160 hash of SHA-256 result
6. Add version byte (0x00 for mainnet)
7. Calculate checksum (double SHA-256)
8. Base58 encode final address
```

---

## 6. RGB LED Implementation

### 6.1 Hardware Connection

#### 6.1.1 LED Configuration
- **Type:** Common cathode RGB LED or WS2812B
- **Connection:** 3 GPIO pins (R, G, B) from RP2040
- **Current Limiting:** 220Ω resistors per channel
- **Maximum Current:** 20mA per channel

#### 6.1.2 Pin Assignment
```
RP2040 GPIO Connections:
- GP26 → Red LED (PWM Channel A)
- GP27 → Green LED (PWM Channel B)  
- GP28 → Blue LED (PWM Channel A)
- GP29 → Yellow LED (PWM Channel B)
- Common cathode → GND
```

### 6.2 LED Status Indicators

#### 6.2.1 Status Color Codes
```
Device States:
┌──────────────────┬─────────┬─────────────────────┐
│ Device State     │ Color   │ Pattern             │
├──────────────────┼─────────┼─────────────────────┤
│ Sealed           │ Green   │ Solid               │
│ New              │ Blue    │ Brief flash         │
│ Tamper Detected  │ Red     │ Solid               │
│ Busy             │ Yellow  │ Slow blink (1Hz)    │
│ Error/Fault      │ Red     │ Fast blink (5Hz)    │
│                  │         │                     │
└──────────────────┴─────────┴─────────────────────┘
```

#### 6.2.2 Firmware LED Control
```c
// LED Control Functions
typedef struct {
    uint8_t red;
    uint8_t green; 
    uint8_t blue;
    uint16_t duration_ms;
    uint8_t pattern;
} led_state_t;

// Status indication functions
void led_set_healthy(void);      // Solid green
void led_set_activity(void);     // Brief blue flash
void led_set_tampered(void);     // Solid red
void led_set_busy                // Yellow blink
void led_set_error(void);        // Red fast blink
```

---

## 7. Manufacturing Specifications

### 7.1 PCB Fabrication Requirements

#### 7.1.1 Manufacturing Tolerances
```
PCB Specifications:
- Board thickness: 1.6mm ±10%
- Copper weight: 1oz ±5%
- Via size: 0.2mm drill, 0.4mm pad
- Minimum trace: 0.15mm width, 0.1mm spacing
- V-score depth: 0.3mm ±0.05mm per side
```

#### 7.1.2 Surface Mount Assembly
- **Component Placement Accuracy:** ±0.05mm
- **Solder Paste:** SAC305 lead-free
- **Reflow Profile:** Standard RoHS profile
- **AOI Inspection:** 100% coverage required

### 7.2 Snap-off Tab Manufacturing

#### 7.2.1 V-Score Specifications
```
V-Score Parameters:
- Angle: 30° ±2°
- Depth: 0.3mm per side (37.5% PCB thickness)
- Width: 0.4mm ±0.1mm
- Position accuracy: ±0.1mm
- Surface finish: Smooth, no burrs
```

#### 7.2.2 Serial Number Marking
- **Method:** Laser etching after final test
- **QR Code Size:** 10mm x 10mm
- **Text Size:** 0.5mm character height
- **Resolution:** Minimum 600 DPI
- **Contrast:** High contrast on PCB substrate

### 7.3 Quality Control Process

#### 7.3.1 Electrical Testing
```
Test Sequence:
1. Power-on self test (POST)
2. USB enumeration verification
3. SE050 communication test
4. LED functionality check
5. V-score integrity verification
6. Final functional test with host PC
```

#### 7.3.2 Programming and Provisioning
```
Production Programming:
1. Flash bootloader to RP2040
2. Flash main application firmware
3. Initialize SE050 secure element
4. Generate Bitcoin key pair
5. Program address data to filesystem
6. Laser etch serial on tab area
7. Apply tamper-evident seal
8. Final verification and packaging
```

---

## 8. Component Specifications and BOM

### 8.1 Primary Components

#### 8.1.1 Core Components
| Component | Part Number | Manufacturer | Package | Qty | Description |
|-----------|------------|--------------|---------|-----|-------------|
| MCU | RP2040 | Raspberry Pi | QFN-56 | 1 | Dual ARM Cortex-M0+ |
| Flash | W25Q128JVSIQ | Winbond | SOIC-8 | 1 | 16MB QSPI Flash |
| SE | SE050C2HQ1/Z01DZ | NXP | HVQFN32 | 1 | Secure Element |
| Crystal | ABM8-12.000MHZ | Abracon | 3.2x2.5mm | 1 | 12MHz Crystal |
| Regulator | AMS1117-3.3 | AMS | SOT-223 | 1 | 3.3V LDO |
| LED | WS2812B | WorldSemi | 5050 | 1 | RGB LED |
| USB Conn | 10118194-0001LF | Amphenol | Through-hole | 1 | USB-A Plug |

#### 8.1.2 Passive Components  
| Component | Value | Package | Qty | Tolerance | Description |
|-----------|--------|---------|-----|-----------|-------------|
| Resistor | 27Ω | 0603 | 2 | 1% | USB termination |
| Resistor | 1kΩ | 0603 | 1 | 5% | LED current limit |
| Resistor | 4.7kΩ | 0603 | 2 | 5% | I2C pull-up |
| Resistor | 10kΩ | 0603 | 3 | 5% | GPIO pull-up |
| Capacitor | 100nF | 0603 | 8 | 10% | Decoupling |
| Capacitor | 10μF | 0805 | 2 | 20% | Power filtering |
| Capacitor | 22μF | 1206 | 1 | 20% | Power output |
| Capacitor | 22pF | 0603 | 2 | 5% | Crystal load |

### 8.2 Cost Analysis

#### 8.2.1 Component Costs (10k+ quantity)
```
Cost Breakdown:
- RP2040 MCU: $1.20
- W25Q128 Flash: $0.45
- SE050 Secure Element: $1.80
- Passive Components: $0.25
- USB Connector: $0.15
- RGB LED: $0.10
- PCB (4-layer): $0.35
- Assembly: $0.40
- Testing: $0.15
- Packaging: $0.10
Total: $4.95 per unit
```

#### 8.2.2 Cost Reduction Opportunities
- Volume pricing at 50k+ units: 15% reduction
- Alternative SE (SE051): 20% cost reduction
- 2-layer PCB option: 30% PCB cost reduction
- Automated testing: 50% testing cost reduction

---

## 9. Testing and Validation

### 9.1 Hardware Testing

#### 9.1.1 Production Test Sequence
```
Test Flow:
1. Visual inspection (automated AOI)
2. In-circuit test (ICT) - power, connectivity
3. Functional test - USB enumeration
4. SE050 communication verification  
5. Bitcoin address generation test
6. LED functionality verification
7. Snap-off tab force test (sample basis)
8. Final system integration test
```

#### 9.1.2 Environmental Testing
- **Operating Temperature:** -10°C to +60°C
- **Storage Temperature:** -40°C to +85°C
- **Humidity:** 95% RH non-condensing
- **Shock:** 50G peak, 3 axis, 5 times each
- **Vibration:** 10-500Hz, 2G acceleration

### 9.2 Firmware Testing

#### 9.2.1 Unit Testing
- USB mass storage device class compliance
- File system integrity and read-only enforcement
- SE050 cryptographic operations
- Bitcoin address format validation
- LED status indication accuracy

#### 9.2.2 Integration Testing
- Multi-platform USB compatibility (Windows/macOS/Linux)
- Block explorer address verification
- QR code scanning compatibility
- Tamper detection functionality
- Power consumption verification

### 9.3 Security Testing

#### 9.3.1 Penetration Testing
- Physical tamper resistance
- Side-channel attack resistance
- USB interface security
- Private key protection verification
- Supply chain security audit

#### 9.3.2 Certification Requirements
- USB-IF compliance testing
- FCC Part 15 Class A emissions
- CE marking compliance
- RoHS material compliance
- Security evaluation (Common Criteria EAL4+)

---

## 10. Development Timeline

### 10.1 Hardware Development

#### 10.1.1 PCB Design Phase (4 weeks)
- Week 1: Schematic design and component selection
- Week 2: PCB layout and routing
- Week 3: Design review and optimization
- Week 4: Manufacturing file preparation

#### 10.1.2 Prototype Phase (6 weeks)
- Week 1-2: PCB fabrication and component procurement
- Week 3-4: Assembly and initial testing
- Week 5-6: Design validation and revision

### 10.2 Firmware Development

#### 10.2.1 Core Development (8 weeks)
- Week 1-2: Development environment setup
- Week 3-4: USB mass storage implementation
- Week 5-6: SE050 integration and crypto functions
- Week 7-8: Testing and debugging

#### 10.2.2 Integration and Testing (4 weeks)
- Week 1-2: Hardware/firmware integration
- Week 3-4: System testing and validation

### 10.3 Production Preparation

#### 10.3.1 Manufacturing Setup (6 weeks)
- Week 1-2: Supply chain setup
- Week 3-4: Production line configuration
- Week 5-6: Quality system implementation

---

## 11. Risk Assessment

### 11.1 Technical Risks

#### 11.1.1 Hardware Risks
| Risk | Probability | Impact | Mitigation |
|------|-------------|---------|------------|
| SE050 supply shortage | Medium | High | Qualify alternative secure elements |
| USB compatibility issues | Low | Medium | Extensive compatibility testing |
| Snap-off mechanism failure | Low | High | Mechanical stress testing |
| Power consumption exceed USB limit | Low | Medium | Power analysis and optimization |

#### 11.1.2 Firmware Risks
| Risk | Probability | Impact | Mitigation |
|------|-------------|---------|------------|
| USB stack bugs | Medium | High | Use proven USB library (TinyUSB) |
| SE050 integration issues | Medium | Medium | Early prototyping and testing |
| File system corruption | Low | Medium | Read-only implementation |
| Security vulnerabilities | Low | High | Security review and testing |

### 11.2 Manufacturing Risks

#### 11.2.1 Production Risks
- **Component availability:** Multi-source critical components
- **Assembly yield:** Conservative design margins
- **Test coverage:** Automated testing for 100% coverage
- **Quality consistency:** Statistical process control

---

## 12. Compliance and Standards

### 12.1 Regulatory Compliance

#### 12.1.1 Electronic Equipment Standards
- **FCC Part 15 Class A:** Electromagnetic emissions
- **CE Marking:** European conformity
- **RoHS Directive:** Restriction of hazardous substances
- **REACH Regulation:** Chemical substance compliance

#### 12.1.2 USB Compliance
- **USB 2.0 Specification:** Full compliance required
- **USB-IF Testing:** Logo certification
- **Mass Storage Class:** BOT specification compliance

### 12.2 Security Standards

#### 12.2.1 Cryptographic Standards
- **FIPS 140-2:** Cryptographic module validation
- **Common Criteria:** Security evaluation (target EAL4+)
- **NIST SP 800-90A:** Random number generation
- **Bitcoin BIP Standards:** Address format compliance

---

## 13. Documentation Deliverables

### 13.1 Design Documentation
- Schematic diagrams (PDF and native format)
- PCB layout files (Gerber, pick & place, BOM)
- 3D mechanical drawings
- Component placement drawings

### 13.2 Firmware Documentation
- Source code with comprehensive comments
- API documentation
- Build instructions and toolchain setup
- Programming and debugging procedures

### 13.3 Manufacturing Documentation
- Assembly drawings and procedures
- Test procedures and acceptance criteria
- Calibration and setup instructions
- Quality control checkpoints

### 13.4 User Documentation
- Quick start guide
- Safety and handling instructions
- Troubleshooting guide
- Technical support procedures

---

**Document Control:**
- Author: Kongzi
- Technical Review: Sajjad (Firmware), Raheel (Hardware)
- Version Control: Git repository
- Next Review: Upon prototype completion

---

**Revision History:**
| Version | Date | Author | Changes |
|---------|------|--------|---------|
| 1.0 | 2025-09-03 | Team | Initial comprehensive TSD |
