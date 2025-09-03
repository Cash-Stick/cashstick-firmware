# User Requirements Document (URD)

**Project Name:** CashStick  
**Product Type:** Bitcoin Bearer Device (USB form)  
**Version:** v1.0   
**Date:** September 3, 2025

---

## 1. Introduction

### 1.1 Purpose

The purpose of this document is to define the comprehensive user requirements for CashStick, a portable Bitcoin bearer device in the form of a USB stick. The device functions as a digital equivalent of a physical coin or banknote: ownership is transferred by physically handing over the device.

### 1.2 Scope

CashStick enables users to store and transfer Bitcoin offline without intermediaries, internet connection, or software setup. It is designed for simplicity, security, and trustless ownership transfer. The device features a tamper-evident snap-off tab mechanism that irreversibly reveals the private key when removed.

### 1.3 Document Structure

This URD covers functional requirements, hardware specifications, security considerations, user experience, manufacturing requirements, and compliance considerations for the CashStick Bitcoin bearer device.

---

## 2. Goals & Objectives

### 2.1 Primary Goals
- Provide a physical bearer instrument for Bitcoin, where possession equals ownership
- Allow users to check balances easily via a website or any block explorer
- Enable offline storage and portability without reliance on centralized wallets or custodians
- Ensure durability and affordability so the device can be used widely, even for micro-transactions
- Create an intuitive user experience requiring no technical expertise
- Provide clear visual indicators of device integrity

### 2.2 Secondary Goals
- Establish trust through tamper-evident design
- Enable global distribution through open-source hardware design


---

## 3. Stakeholders

### 3.1 Primary Stakeholders
- **End Users:** Individuals using Bitcoin for savings, gifting, or hand-to-hand transfers
- **Merchants & Traders:** Businesses or individuals accepting Bitcoin in physical form
- **Gift Recipients:** People receiving CashStick as presents or payments

### 3.2 Secondary Stakeholders
- **Distributors/Resellers:** Entities selling CashSticks unflashed or preloaded with Bitcoin.
- **Manufacturers:** Hardware fabrication partners
- **Open Source Community:** Developers and makers creating derivative products
- **Regulators/Authorities:** May review the device as a financial instrument

---

## 4. Functional Requirements

### 4.1 Bitcoin Storage and Security

#### FR-001: Private Key Storage
- Each CashStick MUST securely store a Bitcoin private key in tamper-resistant hardware
- The private key MUST be generated using cryptographically secure random number generation
- Private key MUST never be accessible through USB interface when device is intact

#### FR-002: Public Key Access
- Device MUST present only the Bitcoin address (public key) when connected to a computer
- Balance checking MUST be possible without exposing private information
- Device MUST appear as a simple USB storage device with minimal data

#### FR-003: Snap-off Tab Mechanism
- Device MUST include a physical snap-off tab that, when removed, irreversibly exposes the private key
- Tab removal MUST create visible, permanent damage to the PCB circuit
- Once tab is removed, private key MUST be readable through connecting it to a host computer, in the file manager.
- Tab removal MUST be a one-way operation that cannot be reversed or disguised

### 4.2 Balance Verification

#### FR-004: USB Balance Check
- Users MUST be able to plug in the device or type the address manually and check the balance via:
  - A dedicated website (e.g., cashstick.org)
  - Standard block explorers (e.g., mempool.space, blockchain.info)
- Device MUST present address information in standard text and in QR file format
- No software installation MUST be required


### 4.3 Ownership Transfer

#### FR-005: Physical Transfer
- No digital action MUST be required to transfer ownership
- Giving the device physically MUST transfer complete Bitcoin ownership
- Transfer MUST work offline without internet connectivity
- Device integrity MUST be verifiable by recipient before accepting transfer

### 4.4 Device Loading

#### FR-006: Bitcoin Loading Options
- Users MUST be able to load their own Bitcoin onto a fresh CashStick
- Loading MUST be possible by sending Bitcoin to the device's address

### 4.5 Tamper Evidence

#### FR-007: Tamper Detection
- Device MUST show visible evidence if someone attempts to access internals
- Any attempt to read private key without tab removal MUST be detectable
- Device MUST include tamper-evident seal.
- Tamper attempts MUST render device permanently marked as compromised

---

## 5. Hardware Requirements

### 5.1 Physical Design

#### HR-001: Form Factor
- Device MUST be USB-A form factor for universal compatibility
- Dimensions MUST not exceed standard USB stick size 
- Weight MUST be under 50 grams
- Device MUST include lanyard/keychain attachment point

#### HR-002: Snap-off Tab Design
- Snap-off tab MUST be clearly marked and intentionally designed for removal
- Tab MUST require deliberate force to remove (preventing accidental activation)
- Tab removal MUST break a circuit trace that previously protected private key access
- Broken circuit area MUST be clearly visible after tab removal
- Private key MUST be exposed via QR code and text after tab removal

#### HR-003: Materials
- Housing MUST be durable plastic suitable for daily handling
- PCB MUST be high-quality substrate
- Components MUST be rated for minimum 10-year lifespan
- Materials MUST be RoHS compliant

### 5.2 Electronic Design

#### HR-004: Secure Storage
- Private key MUST be stored in hardware security element or equivalent
- Secure element MUST have tamper resistance features
- Key generation MUST occur during first setup in secure environment
- No wireless communication capabilities (WiFi, Bluetooth, NFC)

#### HR-005: USB Interface
- Device MUST enumerate as USB Mass Storage Device
- MUST present text and QR files containing Bitcoin address
- USB interface MUST be read-only to prevent malware infection
- MUST be compatible with USB 2.0 and higher standards

### 5.3 Visual Elements

#### HR-006: Identification Markings
- Device MUST include clear "CashStick" branding
- Bitcoin logo MUST be prominently displayed
- Serial number MUST be included for tracking and verification

---

## 6. Security Requirements

### 6.1 Cryptographic Security

#### SR-001: Key Generation
- Private keys MUST be generated using approved random number generators
- Key generation MUST occur in secure, air-gapped environment
- No key MUST be predictable or derived from device serial numbers
- Entropy source MUST provide minimum 256 bits of entropy

#### SR-002: Key Protection
- Private key MUST be protected against physical extraction when device is intact
- Secure element MUST implement anti-tampering measures
- Key MUST be encrypted at rest using device-specific encryption
- Physical attacks MUST result in clear irreversible tamper evidence

### 6.2 Manufacturing Security

#### SR-003: Supply Chain Security
- Key generation and device programming MUST occur in secure facility
- Manufacturing process MUST be auditable and documented
- Chain of custody MUST be maintained from production to distribution
- Quality assurance processes MUST verify security implementation

### 6.3 User Security

#### SR-004: User Education
- Clear warnings MUST be provided about bearer instrument risks
- Device packaging MUST include security best practices
- Website MUST provide comprehensive security guidance
- Physical device MUST include warning labels about tamper detection

---

## 7. User Experience Requirements

### 7.1 Ease of Use

#### UX-001: Plug-and-Play Operation
- Device MUST work with only firmware installation
- Balance checking MUST complete within 10 seconds of USB insertion
- Website interface MUST load in under 3 seconds
- No user account creation MUST be required

#### UX-002: Clear Instructions
- Device packaging MUST include simple 3-step usage instructions
- Visual guides MUST show proper handling and verification
- Website MUST provide instant balance display with large, clear text
- Error messages MUST be user-friendly and actionable

### 7.2 Accessibility

#### UX-003: Universal Compatibility
- Device MUST work on Windows, macOS, and Linux without drivers
- Website MUST be mobile-responsive and screen-reader compatible
- QR codes MUST be scannable by standard smartphone cameras
- Instructions MUST be available in multiple languages

### 7.3 Visual Design

#### UX-004: Professional Appearance
- Device MUST look trustworthy and professionally manufactured
- Color scheme MUST convey security and reliability
- Branding MUST be consistent across device, packaging, and website
- Design MUST clearly distinguish loaded from unloaded devices

---

## 8. Manufacturing and Quality Requirements

### 8.1 Production Standards

#### MQ-001: Quality Control
- Every device MUST pass functional testing before shipment
- USB functionality MUST be tested on multiple operating systems
- Visual inspection MUST verify all markings

#### MQ-002: Scalability
- Manufacturing process MUST support production of 10,000+ units annually
- Unit cost MUST remain under $3 at scale
- Lead times MUST not exceed 30 days for standard orders
- Quality MUST remain consistent across production batches

### 8.2 Open Source Requirements

#### MQ-003: Documentation
- Complete hardware design files MUST be published under open license
- PCB layouts, schematics, and bill of materials MUST be provided
- Manufacturing instructions MUST be documented
- Reference firmware MUST be available

#### MQ-004: Customization Support
- Design MUST allow for easy customization and rebranding
- Alternative form factors MUST be feasible using same core design
- Third-party manufacturing MUST be supported with provided documentation

---

## 9. Compliance and Regulatory Requirements

### 9.1 Hardware Compliance

#### CR-001: Electronic Standards
- Device MUST comply with FCC Part 15 Class A emissions requirements
- CE marking MUST be obtained for European distribution
- RoHS compliance MUST be maintained for all materials
- USB-IF compliance testing MUST be completed

### 9.2 Financial Regulations

#### CR-002: Bearer Instrument Considerations
- Legal status as bearer instrument MUST be researched per jurisdiction

### 9.3 International Distribution

#### CR-003: Export Controls
- Export control classifications MUST be obtained
- Import requirements MUST be researched for target markets
- Customs documentation MUST be standardized

---

## 10. Use Cases and User Stories

### 10.1 Primary Use Cases

#### UC-001: Bitcoin Gift
**Actor:** Gift Giver  
**Goal:** Give Bitcoin as a physical present  
**Scenario:**
1. Purchase and loads a fresh CashStick.
2. Verify balance using USB connection or QR code scan
3. Wrap device as gift with included instruction card
4. Recipient verifies device integrity and balance
5. Recipient owns Bitcoin through physical possession

#### UC-002: Offline Bitcoin Storage
**Actor:** Bitcoin Owner  
**Goal:** Store Bitcoin offline securely  
**Scenario:**
1. Purchase fresh CashStick
2. Send desired Bitcoin amount to device address
3. Verify transfer completion and device balance
4. Store device in secure location
5. Periodically verify balance without compromising security

#### UC-003: Peer-to-Peer Transfer
**Actor:** Bitcoin Seller  
**Goal:** Sell Bitcoin for cash in person  
**Scenario:**
1. Load CashStick with Bitcoin amount for sale
2. Meet buyer and demonstrate device integrity
3. Buyer verifies balance using their own device
4. Exchange CashStick for cash payment
5. Buyer takes physical possession and ownership

### 10.2 Advanced Use Cases

#### UC-004: Private Key Extraction
**Actor:** Device Owner  
**Goal:** Extract private key for wallet import  
**Scenario:**
1. Verify device has not been tampered with
2. Carefully remove snap-off tab following instructions
3. Private key becomes visible on file manager
4. Import private key into preferred Bitcoin wallet
5. Transfer Bitcoin out of device to new address

#### UC-005: Merchant Acceptance
**Actor:** Merchant  
**Goal:** Accept CashStick as payment  
**Scenario:**
1. Customer offers CashStick as payment
2. Merchant verifies device integrity and authenticity
3. Merchant checks balance matches purchase amount
4. Merchant accepts device, taking possession
5. Merchant later consolidates Bitcoin or resells device

---

## 11. Success Criteria

### 11.1 User Adoption Metrics
- 90% of users can successfully check balance within first attempt
- Device can be used by individuals with no Bitcoin experience
- Less than 1% of devices fail during normal use within first year
- User satisfaction rating above 4.5/5 for ease of use

### 11.2 Security Metrics  
- Zero reported cases of private key extraction without tab removal
- 100% of tamper attempts result in visible evidence
- No successful counterfeiting of devices reported
- All security audits pass with minimal findings

### 11.3 Business Metrics
- Manufacturing cost below $2.50 per unit at 10k+ volume
- Distribution network established in minimum 10 countries
- Open source adoption by minimum 5 independent manufacturers
- Recognition as standard for physical Bitcoin transfer

---

## 12. Risk Assessment and Mitigation

### 12.1 Technical Risks

#### R-001: Private Key Compromise
**Risk:** Private key exposed through hardware vulnerability  
**Probability:** Low  
**Impact:** High  
**Mitigation:** Use certified secure elements, conduct security audits, implement multiple tamper detection methods

#### R-002: Counterfeiting
**Risk:** Fake devices enter circulation  
**Probability:** Medium  
**Impact:** High  
**Mitigation:** Include anti-counterfeiting features, establish verification website, educate users on authentication

### 12.2 Business Risks

#### R-003: Regulatory Prohibition
**Risk:** Governments ban bearer Bitcoin devices  
**Probability:** Medium  
**Impact:** High  
**Mitigation:** Engage with users early, emphasize legitimate users, prepare for jurisdiction-specific measurments.

#### R-004: User Loss/Theft
**Risk:** Users lose devices or have them stolen  
**Probability:** High  
**Impact:** Medium (expected for bearer instruments)  
**Mitigation:** Clear education about risks, recommend insurance for high-value devices, suggest secure storage methods

### 12.3 Market Risks

#### R-005: Low Adoption
**Risk:** Users don't understand or trust physical Bitcoin  
**Probability:** Medium  
**Impact:** High  
**Mitigation:** Comprehensive education campaign, start with Bitcoin-savvy early adopters, demonstrate security through third-party audits

---

## 13. Appendices

### 13.1 Glossary
- **Bearer Instrument:** Financial instrument where ownership is determined by physical possession
- **Snap-off Tab:** Removable PCB section that breaks circuit protection when removed
- **Tamper Evidence:** Visual indicators that show if device has been compromised
- **Private Key:** Cryptographic key that provides spending authority for Bitcoin address

### 13.2 Reference Standards
- USB 2.0 Specification
- Bitcoin BIP-32/39/44 Standards
- FIPS 140-2 Security Requirements
- ISO 27001 Information Security Management

### 13.3 Contact Information
- Manager: Kongzi
- Hardware: Raheel
- Firmware: Sajjad


---

