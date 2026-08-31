# 🔐 Arduino Password + IR Sensor + LCD + Excel Logging System

A complete access control system with password authentication, IR sensor detection, LCD display, and Excel logging.

---

## 📋 Table of Contents
- [Features](#-features)
- [Hardware Requirements](#-hardware-requirements)
- [Pin Connections](#-pin-connections)
- [Wiring Diagram](#-wiring-diagram)
- [Installation](#-installation)
- [Usage](#-usage)
- [How It Works](#-how-it-works)
- [LCD Messages](#-lcd-messages)
- [Python Output](#-python-output)
- [Excel Log Format](#-excel-log-format)
- [Troubleshooting](#-troubleshooting)
- [File Structure](#-file-structure)

---

## ✨ Features

| Feature | Description |
|---------|-------------|
| 🔑 **Password Authentication** | 4-digit password (default: 6666) |
| 🖥️ **LCD Display** | Shows status, password input as asterisks, countdown |
| 🔒 **Auto-Lock** | Locks after 3 wrong attempts |
| 🔄 **Auto-Reset** | Automatically resets after 5 seconds |
| 📡 **IR Sensor** | Detects objects to lock/unlock |
| ⚙️ **Servo Motor** | Moves to 180° (unlock) and 0° (lock) |
| 💡 **LED Feedback** | Visual feedback for key presses |
| 📊 **Excel Logging** | Saves all events to Excel file |
| 🐍 **Python Logger** | Real-time logging with colored output |

---

## 🛠️ Hardware Requirements

| Component | Quantity |
|-----------|----------|
| Arduino Uno/Nano | 1 |
| 4x4 Keypad | 1 |
| Micro Servo (SG90) | 1 |
| IR Sensor (HC-SR501) | 1 |
| 16x2 LCD with I2C | 1 |
| Jumper Wires | As needed |
| USB Cable | 1 |

---

## 🔌 Pin Connections

### Complete Pin Reference Table

| Component | Pin | Wire Color (Typical) |
|-----------|-----|---------------------|
| **Keypad R1** | Pin 1 | Yellow |
| **Keypad R2** | Pin 2 | Orange |
| **Keypad R3** | Pin 3 | Red |
| **Keypad R4** | Pin 4 | Brown |
| **Keypad C1** | Pin 5 | Green |
| **Keypad C2** | Pin 6 | Blue |
| **Keypad C3** | Pin 7 | Purple |
| **Keypad C4** | Pin 8 | Gray |
| **Servo Signal** | Pin 10 | Orange/Yellow |
| **Servo VCC** | 5V | Red |
| **Servo GND** | GND | Brown/Black |
| **IR Sensor OUT** | Pin 11 | Yellow/White |
| **IR Sensor VCC** | 5V | Red |
| **IR Sensor GND** | GND | Black |
| **LCD VCC** | 5V | Red |
| **LCD GND** | GND | Black |
| **LCD SDA** | A4 | Green/Blue |
| **LCD SCL** | A5 | Yellow/White |
| **Built-in LED** | Pin 13 | (On board) |

---

## 📊 Wiring Diagram

```
┌──────────────────────────────────────────────────────────────────────────────┐
│                                                                              │
│   KEYPAD (4x4)                    ARDUINO UNO                                │
│   ┌─────────────┐                 ┌──────────────────────────────────────┐   │
│   │  1  2  3  A │  R1 ────────►   │ Pin 1                                │   │
│   │  4  5  6  B │  R2 ────────►   │ Pin 2                                │   │
│   │  7  8  9  C │  R3 ────────►   │ Pin 3                                │   │
│   │  *  0  #  D │  R4 ────────►   │ Pin 4                                │   │
│   └─────────────┘                 │ Pin 5 ◄──── C1                       │   │
│         │                         │ Pin 6 ◄──── C2                       │   │
│         C1 C2 C3 C4               │ Pin 7 ◄──── C3                       │   │
│                                   │ Pin 8 ◄──── C4                       │   │
│                                   │                                      │   │
│   SERVO                           │ Pin 10 ◄──── Servo Signal            │   │
│   ┌─────────────┐                 │                                      │   │
│   │  Signal ────┼───────────────► Pin 10                                 │   │
│   │  VCC    ────┼───────────────► 5V                                     │   │
│   │  GND    ────┼───────────────► GND                                    │   │
│   └─────────────┘                 │                                      │   │
│                                   │ Pin 11 ◄──── IR Sensor OUT           │   │
│   IR SENSOR                       │                                      │   │
│   ┌─────────────┐                 │ Pin 13 ◄──── Built-in LED (on board) │   │
│   │  OUT    ────┼─────────────────► Pin 11                               │   │
│   │  VCC    ────┼─────────────────► 5V                                   │   │
│   │  GND    ────┼─────────────────► GND                                  │   │
│   └─────────────┘                 │                                      │   │
│                                   │                                      │   │
│   LCD (I2C)                       │ A4 (SDA) ◄──── LCD SDA               │   │
│   ┌─────────────┐                 │ A5 (SCL) ◄──── LCD SCL               │   │
│   │  VCC    ────┼─────────────────► 5V                                   │   │
│   │  GND    ────┼─────────────────► GND                                  │   │
│   │  SDA    ────┼─────────────────► A4                                   │   │
│   │  SCL    ────┼─────────────────► A5                                   │   │
│   └─────────────┘                 │                                      │   │
│                                   └──────────────────────────────────────┘   │
│                                                                              │
└──────────────────────────────────────────────────────────────────────────────┘
```

---

## 🔌 Power Distribution

```
┌─────────────────────────────────────────────────────────────┐
│                     POWER DISTRIBUTION                      │
├─────────────────────────────────────────────────────────────┤
│                                                             │
│   Arduino 5V  ───► Servo VCC (Red)                          │
│              ───► IR Sensor VCC                             │
│              ───► LCD VCC                                   │
│                                                             │
│   Arduino GND ───► Servo GND (Brown/Black)                  │
│              ───► IR Sensor GND                             │
│              ───► LCD GND                                   │
│                                                             │
│   Arduino A4  ───► LCD SDA                                  │
│   Arduino A5  ───► LCD SCL                                  │
│                                                             │
└─────────────────────────────────────────────────────────────┘
```

---

## 🚀 Installation

### 1️⃣ Create Project Directory
```bash
mkdir -p ~/arduino
cd ~/arduino
```

### 2️⃣ Create Virtual Environment
```bash
python3 -m venv arduino_env
source arduino_env/bin/activate
pip install pyserial openpyxl pandas
```

### 3️⃣ Upload Arduino Code
1. Open Arduino IDE
2. Copy the Arduino code from `arduino_code.ino`
3. Select: **Tools → Board → Arduino Uno**
4. Select: **Tools → Port → /dev/ttyACM0**
5. Click **Upload**

### 4️⃣ Save Python Scripts
Save the following files in `~/arduino/`:
- `arduino_logger.py`
- `view_logs.py`

---

## 📖 Usage

### Run the Logger
```bash
cd ~/arduino
source arduino_env/bin/activate
python arduino_logger.py
```

### View Logs
```bash
python view_logs.py
```

### Open Excel File
```bash
libreoffice access_log.xlsx
```

---

## 🔄 How It Works

### Step 1: Password Authentication
```
1. Enter 6666# on keypad
2. LCD shows asterisks: ****
3. System unlocks → Servo moves to 180°
4. Wait 3 seconds → IR sensor becomes active
```

### Step 2: IR Sensor Operation
```
1. Wave hand in front of IR sensor
2. Servo moves to 0° (locked)
3. System ready for next password
```

### Step 3: Auto-Lock & Reset
```
1. After 3 wrong attempts → System locks
2. LCD shows countdown: "Reset in 5s"
3. Auto-reset after 5 seconds
4. System ready for new attempts
```

---

## 📺 LCD Messages

| Event | Row 1 | Row 2 |
|-------|-------|-------|
| **Startup** | System Ready | Enter Password |
| **Entering Password** | Password: | **** |
| **Clear Password** | Cleared | Enter Password |
| **No Password** | No Password | Enter 6666 |
| **Correct Password** | UNLOCKED | 180 degrees |
| **Wrong Password** | WRONG! | Attempt 1/3 |
| **System Locked** | SYSTEM LOCKED! | Reset in 5s |
| **Auto-Reset** | System Reset | Enter Password |
| **IR Active** | IR Active | Wave hand to lock |
| **IR Detected** | LOCKED | 0 degrees |

---

## 🖥️ Python Output

### Success Output
```
======================================================================
  ARDUINO ACCESS LOGGER v2.0
======================================================================
[+] Connected to /dev/ttyACM0
[*] Enter password on keypad (default: 6666#)
======================================================================

[+] Log file: access_log.xlsx
----------------------------------------------------------------------

[*] Password: ****
[i] Password submitted
[+] ACCESS GRANTED!
[+] [2026-09-01 01:30:25] Access Granted!
[+] UNLOCKING - Moving servo to 180 degrees
[i] 3 seconds passed - Ready for IR detection
[*] IR Sensor - OBJECT DETECTED!
[!] LOCKING - Moving servo to 0 degrees
```

### Wrong Password Output
```
[i] Password submitted
[-] WRONG PASSWORD! (Attempt 1/3)
[-] [2026-09-01 01:31:15] Wrong Password Attempt!
```

### System Locked Output
```
[!] SYSTEM LOCKED!
[!] [2026-09-01 01:31:45] System Locked!
[i] STATUS:Auto-reset in 5 seconds
[i] STATUS:Auto-reset in 4 seconds
[i] STATUS:Auto-reset in 3 seconds
[i] STATUS:Auto-reset in 2 seconds
[i] STATUS:Auto-reset in 1 seconds
[i] STATUS:AUTO-RESET - System unlocked
[+] [2026-09-01 01:31:50] System Reset!
```

---

## 📊 Excel Log Format

| Timestamp | Event |
|-----------|-------|
| 2026-09-01 01:30:25 | Access Granted! |
| 2026-09-01 01:31:15 | Wrong Password Attempt! |
| 2026-09-01 01:31:45 | System Locked! |
| 2026-09-01 01:31:50 | System Reset |

---

## 🐛 Troubleshooting

### Permission Error
```bash
sudo chmod 666 /dev/ttyACM0
```

### Add User to Dialout Group
```bash
sudo usermod -a -G dialout $USER
# Logout and login again
```

### Python Module Not Found
```bash
pip install pyserial openpyxl pandas
```

### LCD Not Showing Text
| Problem | Solution |
|---------|----------|
| No text | Adjust blue contrast screw on I2C module |
| Wrong address | Try: 0x20, 0x27, 0x3F |
| Backlight off | Check VCC and GND connections |
| Garbage characters | Check SDA/A4 and SCL/A5 connections |

### Servo Not Moving
| Problem | Solution |
|---------|----------|
| No power | Check 5V connection |
| No signal | Check wire to Pin 10 |
| No ground | Check common GND |

### IR Sensor Not Working
| Problem | Solution |
|---------|----------|
| Wrong voltage | Try 5V or 3.3V |
| No detection | Adjust potentiometer |
| Always on | Check OUT pin connection |

### Keypad Not Working
| Problem | Solution |
|---------|----------|
| No input | Check pins 1-8 |
| Wrong mapping | Verify keymap array |
| Stuck keys | Check wiring |

---

## 📁 File Structure

```
~/arduino/
├── arduino_code.ino          # Arduino code
├── arduino_logger.py          # Python logger script
├── view_logs.py              # View Excel logs
├── access_log.xlsx           # Excel log file (auto-created)
├── README.md                 # This file
└── arduino_env/              # Virtual environment (auto-created)
```

---

## 📝 License

This project is open source. Feel free to modify and distribute.

---

## 🙏 Credits

Built with Arduino, Python, and lots of patience.

---

**Happy Hacking!** 🚀
