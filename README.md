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
- **Real-time Sensor Monitoring** — Humidity, Temperature, Soil Moisture, Water Level, Air Quality
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
| OLED 1.3" DST-013 | SDA → A4, SCL → A5 |

### Wiring Diagram

```text
Arduino Uno
┌──────────────┐
│ 5V ──────────┼── DHT11 VCC, Water Sensor VCC, Soil Sensor VCC, OLED VCC
│ GND ─────────┼── All sensor GND
│ D3 ──────────┼── DHT11 DATA (+ 10kΩ pull-up to 5V)
│ D4 ──────────┼── Relay / Motor control
│ A0 ──────────┼── Soil Moisture Signal
│ A1 ──────────┼── Water Level Signal
│ A4 ──────────┼── OLED SDA
│ A5 ──────────┼── OLED SCL
└──────────────┘
```

### Upload the Sketch

1. Install the **DHT sensor library** in Arduino IDE (`Sketch → Include Library → Manage Libraries → Search "DHT sensor library" by Adafruit → Install`)
1. Install the **U8g2** library in Arduino IDE (`Sketch → Include Library → Manage Libraries → Search "U8g2" by olikraus → Install`)
1. Open `arduino_sketch.ino` in Arduino IDE
1. Select **Board: Arduino Uno** and the correct **Port**
1. Click **Upload**

### OLED Display

The Arduino features a real-time **OLED display** with animated 8x8 pixel icons:

| Icon | Sensor | Animation |
| --- | --- | --- |
| 🔥 **Fire** | Temperature | Flickering flame indicates heat |
| 💧 **Water Drop** | Water Level | Falling droplet shows water flow |
| 🍃 **Leaf** | Soil Moisture | Swaying leaf indicates soil hydration |
| 💫 **Bubble** | Humidity | Growing bubble that pops when humid |

Each icon animates independently with smooth non-blocking frame updates at 50Hz, ensuring the sensor loop is never blocked.

**Display Layout:**

```text
     AQUA TERRA
  🔥 Temp 24.5°C
  💧 Hum  65.2%
  🍃 Soil 42%
  💫 Water 78%
```

### Serial Output

The Arduino outputs clean JSON data every ~2 seconds:

```json
{"humidity":65.2,"temperature":24.1,"soil":42,"waterLevel":78,"relay":1,"motorRunning":1}
```

**Notes:**

- `relay` and `motorRunning` are `1` when pumping is active (relay ON / pin LOW)
- Sensor values are constrained to `0-100` for stable dashboard rendering
- All animations use non-blocking `millis()` timing—no delays block the sensor loop
- Each animation frame runs at its own configurable interval (95ms fire, 140ms humidity, etc.)

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

## 📜 License

MIT © 2026
