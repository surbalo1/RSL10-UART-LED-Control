# RSL10 UART LED Control

this project is a simple uart-based led control for the on semiconductor rsl10. it allows turning an led on or off from a serial terminal and shows confirmation messages in real time.

## 📋 description

the firmware demonstrates basic uart communication on the rsl10 ble soc. it reads commands from a terminal, controls the onboard led, and sends messages back confirming the action.

### features

* uart bidirectional communication at 115200 baud
* led control with simple commands (`1` = on, `0` = off)
* dma-based transfers for fast communication
* polling-based reception with simple software debouncing
* command confirmation in terminal
* clean code with english comments
* only uses onboard led, no extra hardware needed

## 🔧 hardware requirements

* **board:** rsl10-002gevb
* **microcontroller:** rsl10 ble 5.2, ultra-low power
* **debugger:** j-link (included)
* **cable:** usb type-a to micro-usb

### pin configuration

| function | pin  | description          |
| -------- | ---- | -------------------- |
| uart tx  | dio5 | send data to pc      |
| uart rx  | dio4 | receive data from pc |
| led      | dio6 | onboard led control  |

## 💻 software requirements

* on semiconductor ide (eclipse-based)
* rsl10 sdk v3.6 or higher
* serial terminal (putty, tera term, screen, arduino serial monitor)
* segger j-link software

## 🚀 getting started

### installation

1. clone repo:

```
git clone https://github.com/surbalo1/RSL10-UART-LED-Control.git
cd RSL10-UART-LED-Control
```

2. import in ide: file → import → existing projects → select folder → finish

3. build project: project → build all (ctrl+b)

4. flash to rsl10: run → debug (f11)

### usage

1. open serial terminal: baud 115200, 8n1, no flow control
2. send commands:

   * `1` → led on
   * `0` → led off
3. expected output:

```
=== RSL10 UART LED Control ===
commands:
 1 = turn on led
 0 = turn off led

ready to receive commands...

led on
led off
led on
```

## 📁 project structure

```
RSL10-UART-LED-Control/
├── README.md
├── LICENSE
├── .gitignore
├── docs/terminal-output.png
├── include/app.h
├── include/uart.h
├── src/app.c
└── src/uart.c
```

## 🎓 how it works

the system uses dma uart for fast transfers:

* tx (dio5) → sends confirmation in linear dma mode
* rx (dio4) → receives commands in circular dma mode
* baud → 115200

simple software debouncing: only process command if it’s different from last one.
fifo empty → reset last_char.

main loop pseudocode:

```
while(1) {
  refresh watchdog
  if(rx fifo has data) {
    read char
    if char != last_char {
      if char == '1' → led on + send message
      if char == '0' → led off + send message
      last_char = char
    }
  } else last_char = 0
}
```

## 📊 code stats

* lines: 350+
* source files: 2 (app.c, uart.c)
* headers: 2 (app.h, uart.h)
* functions: 3 main (initialize, main, uart_initialize)
* comments: full english documentation

## 🐛 troubleshooting

* repeated messages → solved with last_char check
* no response → check uart pins dio4/dio5
* garbage chars → check baud 115200
* led not switching → check dio6 output

## 🛠️ technical details

* system clock: 8mhz (48mhz xtal /6)
* uart clock derived from system clock
* dma tx: linear, rx: circular
* power: ~1.5ma idle, +2ma led on
* watchdog enabled

## 🔮 future enhancements

* multiple led / rgb
* pwm led brightness
* blink patterns
* ble remote control
* low-power uart wakeup
* command history buffer
* better error handling

## 🤝 contributing

report bugs, suggest features, send pull requests, improve docs

## 📝 license

mit license

## 👤 author

surbalo1 - github.com/surbalo1

## 🙏 acknowledgments

* based on on semiconductor rsl10 sample code
* uart driver from rsl10 sdk
* thanks to on semiconductor community

**version 1.0.0**
**last updated october 2025**