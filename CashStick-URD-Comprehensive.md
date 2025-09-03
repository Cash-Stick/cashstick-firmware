# 📋 User Requirements Document (URD)
## CashStick - Bitcoin Bearer Device

<div align="center">

![CashStick Logo](https://img.shields.io/badge/CashStick-Bitcoin%20Bearer%20Device-orange?style=for-the-badge&logo=bitcoin&logoColor=white)

**Project Name:** CashStick  
**Product Type:** Bitcoin Bearer Device (USB form)  
**Version:** v1.0  
**Date:** September 3, 2025

</div>

---

## 🎯 Purpose

CashStick is a simple Bitcoin bearer device that works like physical cash - ownership transfers by handing over the device. It's designed to be:
- **Cheap to build** - Anyone can make one
- **Easy to use** - No technical knowledge required  
- **Secure** - Protected until you need to spend
- **Transferable** - Can be passed along many times like an Olympic torch

## 🔄 Core User Flow

### The CashStick Journey (Like an Olympic Torch)

```
1. 🛠️  BUILD/BUY → 2. 💻 FLASH → 3. 💰 LOAD → 4. 🤝 PASS → 5. ✅ VERIFY → 6. 🔄 REPEAT → 7. 💸 SPEND
```

1. **🛠️ Build or Buy** - Get a CashStick PCB cheaply
2. **💻 Flash Firmware** - Drag & drop firmware file to bootloader
3. **💰 Create & Load Wallet** - Device generates Bitcoin address, load with funds
4. **🤝 Pass Along** - Hand device to next person (like Olympic torch)
5. **✅ Verify Seal** - Recipient checks device is intact and unbroken
6. **🔄 Keep Passing** - Can be transferred many times while sealed
7. **💸 Final Spend** - Snap off tab, plug in, sweep private key to hot wallet

---

## ⚙️ Requirements

### 🛠️ Build Requirements
- **Cheap Hardware** - Total cost under $5 per device
- **Open Source** - All designs and firmware freely available
- **Simple Assembly** - Can be built with basic PCB manufacturing

### 💻 Firmware Requirements  
- **Easy Flash** - Drag & drop firmware file to device bootloader
- **Wallet Creation** - Generate secure Bitcoin wallet on first use
- **USB Storage** - Show up as simple USB drive with Bitcoin address

### 🔒 Security Requirements
- **Tamper-Evident** - Physical snap-off tab protects private key
- **Sealed Verification** - Visual inspection shows if device was opened
- **Chain of Custody** - Maintains security through multiple transfers
- **Private Key Protection** - Key only accessible after tab removal

### 🔄 Transfer Requirements
- **Physical Handoff** - Ownership transfers by giving device
- **Visual Verification** - Recipient can verify device is sealed
- **Multiple Transfers** - Same device can be passed many times
- **Offline Operation** - No internet needed for verification or transfer

---

## 🔧 Hardware Specs

### Physical Design
- **Form Factor** - Standard USB-A stick
- **Snap-off Tab** - Extended PCB section with private key revealed when broken
- **Materials** - Basic PCB with simple plastic housing (optional)
- **Cost Target** - Under $5 total build cost

### Electronic Design  
- **USB Interface** - Shows up as mass storage device with Bitcoin address file
- **Secure Storage** - Hardware security element protects private key when sealed
- **Visual Indicator** - LED shows device status (new/loaded/tampered)

---

## 📖 Example Usage

### 🎁 Gift Scenario
1. **Alice** builds a CashStick, flashes firmware, creates wallet, loads 0.01 BTC
2. **Alice** gives sealed CashStick to **Bob** as birthday gift
3. **Bob** plugs in device, verifies it's sealed, checks balance shows 0.01 BTC
4. **Bob** keeps device sealed for future use

### 🔄 Olympic Torch Scenario  
1. **Person A** → **Person B** → **Person C** → **Person D** (device passed along)
2. Each person verifies seal before accepting
3. Each person can check balance without breaking seal
4. **Person D** finally needs to spend: snaps tab, sweeps private key
5. Device is "spent" and no longer usable

### 💸 Final Spend
1. Owner snaps off the tab (permanent action)
2. Plugs device into laptop
3. Private key now visible in file system as text/QR code
4. Uses any hot wallet to sweep the Bitcoin
5. Device is now empty and "burned" (like spending cash)

---

## ✅ Success Criteria

- **Build Cost** - Under $5 per device
- **Ease of Use** - Anyone can flash firmware and use
- **Security** - Zero private key leaks while sealed  
- **Transferability** - Can be passed 10+ times while sealed
- **Finality** - When tab snapped, private key immediately accessible

---

<div align="center">

**🔐 CashStick - Physical Bitcoin for Everyone**

[![GitHub Repository](https://img.shields.io/badge/GitHub-Cash--Stick%2Fcashstick--firmware-blue?style=for-the-badge&logo=github)](https://github.com/Cash-Stick/cashstick-firmware)

*Making Bitcoin as simple as handing someone a coin* 🪙

</div>
