# 🌿 AQUA TERRA — Smart Water & Plant Monitoring System

A stunning 3D animated dashboard that reads real-time sensor data from an Arduino Uno
via USB serial port, displays it beautifully with Three.js and Chart.js, and saves
everything to a SQLite database.

![Theme: Bio-Digital Greenhouse](https://img.shields.io/badge/theme-Bio--Digital%20Greenhouse-00ff88?style=for-the-badge&labelColor=050a05)
![Node.js](https://img.shields.io/badge/node-%3E%3D18.0-00ff88?style=flat-square)
![License: MIT](https://img.shields.io/badge/license-MIT-00d4ff?style=flat-square)

---

## ✨ Features

- **3D Animated Dashboard** — Three.js rotating plant pot, floating sensor orbs, particle systems
- **Real-time Sensor Monitoring** — Humidity, Temperature, Soil Moisture, Water Level
- **LED Indicators** — Red/Green/Blue LEDs show plant condition (dry/good/overwatered) in real-time
- **Web Serial API** — Connect Arduino directly from Chrome/Edge browser
- **Demo Mode** — Realistic simulated data when no Arduino is connected
- **Glassmorphism UI** — Dark luxury aesthetic with neon accents and blur effects
- **Historical Charts** — Chart.js multi-line graphs with time range selectors
- **Alert System** — Toast notifications + banner alerts for threshold breaches
- **SQLite Database** — Auto-created, zero-config persistent storage
- **REST API** — Full CRUD endpoints for sensor data
- **CSV Export** — Download historical data as CSV
- **Settings Panel** — Configurable thresholds, refresh intervals, sound alerts
- **Mobile Responsive** — Cards stack beautifully on small screens
- **Mouse Parallax** — 3D scene tilts with cursor movement
- **Automated Watering** — Smart pump control based on soil moisture and water level

---

## 📁 Project Structure

```text
Aqua-Terra/
├── index.html           # Complete frontend (HTML + CSS + JS, single file)
├── server.js            # Node.js Express backend with SerialPort + SQLite
├── database.js          # Database initialization and query helpers
├── arduino_sketch.ino   # Arduino code for all 4 sensors
├── package.json         # npm dependencies
├── .env                 # Environment variables (create this)
└── README.md            # This file
```

---

## 🚀 Quick Start

### Prerequisites

- **Node.js** v18 or higher — [Download](https://nodejs.org/)
- **Chrome** or **Edge** browser (for Web Serial API)
- **Arduino Uno** + sensors (optional — demo mode works without hardware)

### Step 1: Install Dependencies

```bash
cd Aqua-Terra
npm install
```

### Step 2: Configure Environment (Optional)

Create a `.env` file in the project root:

```env
# Server port (default: 3000)
PORT=3000

# Database file path (default: ./aqua_terra.db)
DB_PATH=./aqua_terra.db

# Arduino serial port (leave empty for browser-only mode)
# Windows: COM3, COM4, etc.
# macOS:   /dev/tty.usbmodem*
# Linux:   /dev/ttyUSB0, /dev/ttyACM0
SERIAL_PORT=

# Serial baud rate (default: 9600)
SERIAL_BAUD=9600
```

### Step 3: Start the Server

```bash
npm start
```

The server will start at [http://localhost:3000](http://localhost:3000)

### Step 4: Open the Dashboard

Navigate to <http://localhost:3000> in Chrome or Edge.

- Click **"Connect Arduino"** to use the Web Serial API
- If no Arduino is detected, **Demo Mode** starts automatically after 3 seconds

---

## 🔌 Arduino Setup

### Required Hardware

| Component | Pin Connection |
| --- | --- |
| DHT11 (Temp + Humidity) | Data → D3 |
| Soil Moisture Sensor | Signal → A0 |
| Water Level Sensor | Signal → A1 |
| Relay / Motor Driver | Control → D4 |
| Red LED (Dry Indicator) | Anode → D5, Cathode → GND (+ 220Ω resistor) |
| Green LED (Good Indicator) | Anode → D6, Cathode → GND (+ 220Ω resistor) |
| Blue LED (Overwater Indicator) | Anode → D7, Cathode → GND (+ 220Ω resistor) |

### Wiring Diagram

```text
Arduino Uno
┌──────────────┐
│ 5V ──────────┼── DHT11 VCC, Water Sensor VCC, Soil Sensor VCC
│ GND ─────────┼── All sensor GND, Red LED Cathode (via 220Ω resistor), Green LED Cathode (via 220Ω resistor)
│ D3 ──────────┼── DHT11 DATA (+ 10kΩ pull-up to 5V)
│ D4 ──────────┼── Relay / Motor control
│ D5 ──────────┼── Red LED Anode (220Ω resistor in series)
│ D6 ──────────┼── Green LED Anode (220Ω resistor in series)
│ D7 ──────────┼── Blue LED Anode (220Ω resistor in series)
│ A0 ──────────┼── Soil Moisture Signal
│ A1 ──────────┼── Water Level Signal
└──────────────┘

LED Indicators (Visual Plant Status):
🔴 RED LED (D5)   → ON when soil is DRY (< 30%)
🟢 GREEN LED (D6) → ON when soil is GOOD (30-70%)
🔵 BLUE LED (D7)  → ON when soil is OVERWATERED (> 70%)
```

### Upload the Sketch

1. Install the **DHT sensor library** in Arduino IDE (`Sketch → Include Library → Manage Libraries → Search "DHT sensor library" by Adafruit → Install`)
1. Open `arduino_sketch.ino` in Arduino IDE
1. Select **Board: Arduino Uno** and the correct **Port**
1. Click **Upload**

**VS Code Setup (Optional):**

If using VS Code with the Arduino extension, the `.vscode/c_cpp_properties.json` file is included to enable IntelliSense. The configuration is generic and works with or without Arduino IDE installed. For full IDE integration with Arduino libraries, install the [Arduino extension](https://marketplace.visualstudio.com/items?itemName=vsciot-vscode.vscode-arduino).

### LED Indicators

The system uses visual **LED indicators** to show plant health status in real-time:

| LED Color | Condition | Soil Moisture | Action |
| --- | --- | --- | --- |
| 🔴 **RED** | Plant is DRY | < 30% | Watering needed soon |
| 🟢 **GREEN** | Plant is GOOD | 30-70% | Optimal condition |
| 🔵 **BLUE** | OVERWATERED | > 70% | Reduce watering |

**LED Control Logic:**

```
if SOIL < 30%     → RED LED ON,   GREEN LED OFF     (DRY - needs water)
if 30% ≤ SOIL ≤ 70% → RED LED OFF,  GREEN LED ON     (GOOD - healthy)
if SOIL > 70%     → RED LED OFF,  GREEN LED OFF     (OVERWATERED)
```

The browser dashboard also mirrors these LED states in real-time.

### Serial Output

The Arduino outputs clean JSON data every ~1 second:

```json
{"humidity":65.2,"temperature":24.1,"soil":42,"waterLevel":78,"relay":1,"motorRunning":1}
```

**Notes:**

- `relay` and `motorRunning` are `1` when pumping is active (relay ON / pin LOW)
- Sensor values are constrained to `0-100` for stable dashboard rendering
- All animations use non-blocking `millis()` timing—no delays block the sensor loop
- Each animation frame runs at its own configurable interval (95ms fire, 140ms humidity, etc.)

### 🔌 Relay / Motor Connection

The system includes a **safety-first relay circuit** that controls a water pump or motor for automated watering.

#### Relay Control Logic

The motor **only runs** when BOTH conditions are met:

1. **Soil is dry** — Soil moisture is below 30%
2. **Water is available** — Water level is at least 20%

This prevents the motor from running when water is depleted, protecting the pump from damage.

#### Wiring the Relay Module

**5V Relay Module to Arduino:**

```text
Relay Module     →  Arduino Uno
GND             →  GND
VCC             →  5V
IN (Signal)     →  D4 (RELAY_PIN)
```

**Motor/Pump to Relay:**

```text
Power Supply ──→ Relay Module COM (Common)
Relay NO (normally open) ──→ Motor (+)
Motor (-) ──→ Power Supply GND
Relay GND ──→ Power Supply GND
```

#### Typical Setup

```text
AC/DC Power Supply (12V or 24V recommended)
    │
    ├─→ Relay Module (COM)
    │
    └─→ Motor/Pump 
         │
         ├─→ Relay NO (from relay output)
         │
         └─→ back to Power Supply GND
```

**Arduino Pin D4 Logic:**

- **D4 = LOW** (0V) → Relay energized → Motor ON (pump water)
- **D4 = HIGH** (5V) → Relay de-energized → Motor OFF (stop pump)

#### Safety Features

- ✅ **Water Level Check** — Motor won't run if water tank is empty
- ✅ **Soil Moisture Threshold** — Only triggers pump at 30% soil moisture
- ✅ **Relay Diode Protection** — Use a 1N4007 diode across relay coil to prevent back-EMF damage
- ✅ **Separate Power Supply** — Never power high-current motor from Arduino 5V rail
- ✅ **Relay Debouncing** — 500ms software debounce prevents voltage spikes
- ✅ **Transistor Buffer** — Optional: Use NPN transistor (2N2222) to isolate relay from Arduino

#### ⚡ CRITICAL FIX: Arduino Crash on First Relay Activation

**Problem:** System freezes when relay turns on for the first time.

**Root Cause:** Inrush current from relay coil causes voltage spike that crashes Arduino.

**Hardware Fix (MUST DO):**

1. **Add protection diode across relay coil:**

   ```text
   Relay Coil (+) ←─→ 1N4007 Diode ←─→ Relay Coil (-)
   Cathode stripe → HIGH side
   ```

2. **Add decoupling capacitors to power supply:**

   - 100µF capacitor across main power supply
   - 10µF capacitor across Arduino 5V and GND

3. **Optional but recommended: Use transistor buffer:**

   ```text
   Arduino D4 ──→ [10kΩ resistor] ──→ Base of 2N2222 NPN Transistor
                                          │
                                          Emitter ──→ GND
                                          │
                                          Collector ──→ Relay IN
   ```

4. **Use separate power supply for motor/relay** — 12V or 24V dedicated supply, NOT Arduino 5V rail

**Firmware Fix (Already Implemented):**

The code now includes:

- Software debouncing (500ms delay between relay state changes)
- Separate desired state tracking (`relayDesiredState`)
- Safe relay switching function (`updateRelayState()`)

**Test Procedure:**

1. Add all hardware fixes above
2. Upload updated sketch
3. Test relay activation — should be smooth with NO crashes or freezes

#### Relay Specifications

| Specification | Typical Values |
| --- | --- |
| Operating Voltage | 5V DC |
| Max Current (Control Pin) | 2-3mA |
| Contact Rating | 10A @ 250V AC / 30V DC typical |
| Max Motor Load | ~3-5A (depends on relay specs) |
| Response Time | <30ms |
| Debounce Time | 500ms (software protection) |

---

## 📡 API Endpoints

| Method | Endpoint | Description |
| --- | --- | --- |
| `GET` | `/api/latest` | Most recent sensor reading |
| `GET` | `/api/history?hours=24` | Historical readings |
| `GET` | `/api/history?limit=100` | Last N readings |
| `POST` | `/api/save` | Save a new reading |
| `GET` | `/api/stats?hours=24` | Min/Max/Avg statistics |
| `GET` | `/api/status` | Server & serial status |
| `GET` | `/api/export?hours=168` | Download CSV export |

### Example: Save a Reading

```bash
curl -X POST http://localhost:3000/api/save \
  -H "Content-Type: application/json" \
  -d '{"humidity":65.2,"temperature":24.1,"soil":42,"waterLevel":78,"motorRunning":1}'
```

---

## 🎨 Design System

| Element | Value |
| --- | --- |
| Background | `#050a05` (deep black-green) |
| Primary | `#00ff88` (neon emerald) |
| Accent | `#00d4ff` (electric cyan) |
| Warning | `#ffaa00` (amber) |
| Alert | `#ff3344` (crimson) |
| Display Font | Orbitron |
| Mono Font | JetBrains Mono |
| Body Font | Exo 2 |

---

## 🛠️ Tech Stack

- **Frontend**: HTML5 + CSS3 + Vanilla JavaScript (single file)
- **3D Engine**: Three.js r128
- **Charts**: Chart.js 4.4.1
- **Backend**: Node.js + Express.js
- **Database**: SQLite (via better-sqlite3)
- **Serial**: Web Serial API (browser) + serialport (server)
- **Fonts**: Google Fonts (Orbitron, JetBrains Mono, Exo 2)

---

## � GitHub Setup

This project is ready for GitHub! To deploy:

### 1. **Create a GitHub Repository**

```bash
git init
git add .
git commit -m "Initial commit: Aqua Terra Smart Plant Monitor with LED Indicators"
git branch -M main
git remote add origin https://github.com/YOUR_USERNAME/aqua-terra.git
git push -u origin main
```

### 2. **Project Files**

```
aqua-terra/
├── .gitignore              # Git ignore rules
├── README.md               # This documentation
├── package.json            # Node.js dependencies
├── index.html              # Complete frontend (single file)
├── server.js               # Express backend + Web Serial
├── database.js             # SQLite helpers
├── arduino_sketch.ino      # Arduino firmware
└── LICENSE                 # MIT License
```

### 3. **Key Features to Highlight**

- ✅ Zero external config needed (auto-creates database)
- ✅ Works with or without Arduino (demo mode)
- ✅ Modern tech stack (Three.js, Chart.js, Express, SQLite)
- ✅ Beautiful UI/UX with glassmorphism aesthetic
- ✅ Simple hardware (Arduino Uno + 3 sensors + 2 LEDs + 1 relay)
- ✅ Full API for integration with other systems

### 4. **Recommended README Sections for GitHub**

Your README now includes:
- ✅ Quick start guide
- ✅ Hardware wiring with LED indicators
- ✅ Arduino upload instructions
- ✅ API endpoints
- ✅ Tech stack
- ✅ License information

---

## 📝 Notes

- All sensor values are normalized to **0-100%** for consistency
- LED resistors (220Ω) required for safe operation
- Motor/relay uses separate power supply (not Arduino 5V)
- Demo mode generates realistic simulated data
- Database is auto-created on first run

---

## 🎯 Future Improvements

- [ ] Mobile app version (React Native)
- [ ] Cloud sync (Firebase, AWS)
- [ ] Machine learning watering prediction
- [ ] Multiple plant monitoring
- [ ] Weather API integration
- [ ] Telegram/Discord bot alerts

---

## �📜 License

MIT © 2026
