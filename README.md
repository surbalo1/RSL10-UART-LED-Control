<div align="center">

# 💡 RSL10 UART LED Control

[![C](https://img.shields.io/badge/C-A8B9CC?style=for-the-badge&logo=c&logoColor=black)](https://en.wikipedia.org/wiki/C_(programming_language))
[![RSL10](https://img.shields.io/badge/ON_Semi_RSL10-00A651?style=for-the-badge&logo=onsemi&logoColor=white)](https://www.onsemi.com/products/wireless-connectivity/bluetooth-low-energy/rsl10)
[![License](https://img.shields.io/badge/License-MIT-green?style=for-the-badge)](LICENSE)

**Simple UART-based LED control for RSL10 with bidirectional serial communication.**

*DMA transfers • Command confirmation • No external hardware required*

</div>

---

## 📋 Overview

A minimal firmware demonstrating bidirectional UART communication on the ON Semiconductor RSL10 BLE SoC. Control the onboard LED via simple serial commands with real-time confirmation messages.

---

## ✨ Features

| Feature | Description |
|---------|-------------|
| **📡 UART Communication** | Bidirectional @ 115200 baud |
| **💡 LED Control** | Simple commands (`1` = on, `0` = off) |
| **⚡ DMA Transfers** | Fast, efficient data handling |
| **✅ Confirmation** | Real-time command feedback |
| **🔧 No External HW** | Uses onboard LED only |

---

## 🛠️ Hardware

| Component | Description |
|-----------|-------------|
| **Board** | RSL10-002GEVB |
| **MCU** | RSL10 BLE 5.2 Ultra-Low Power |
| **Debugger** | J-Link (included) |
| **Cable** | USB Type-A to Micro-USB |

### Pin Configuration

| Function | Pin | Description |
|:--------:|:---:|-------------|
| **UART TX** | DIO5 | Send data to PC |
| **UART RX** | DIO4 | Receive data from PC |
| **LED** | DIO6 | Onboard LED control |

---

## 🚀 Quick Start

```bash
# Clone repository
git clone https://github.com/surbalo1/RSL10-UART-LED-Control.git
cd RSL10-UART-LED-Control

# Import in ON Semiconductor IDE
# Build: Ctrl+B
# Flash: F11
```

### Usage

1. Open serial terminal @ **115200 baud, 8N1**
2. Send commands:
   - `1` → LED ON
   - `0` → LED OFF

### Expected Output

```
=== RSL10 UART LED Control ===
commands:
 1 = turn on led
 0 = turn off led

ready to receive commands...

led on
led off
```

---

## 📄 License

MIT License

---

<div align="center">

[![GitHub](https://img.shields.io/badge/Star_on_GitHub-181717?style=for-the-badge&logo=github&logoColor=white)](https://github.com/surbalo1/RSL10-UART-LED-Control)

</div>