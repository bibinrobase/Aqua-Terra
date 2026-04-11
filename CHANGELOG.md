# 📝 Changelog — Aqua Terra v1.1

## Version 1.2 — Blue LED for Overwatered Condition

### ✨ New Features

- 🔵 **Blue LED Indicator** — Shows when soil is overwatered (> 70%)
- 📊 **Complete LED Status Feedback** — All three conditions now have dedicated LEDs
- Updated wiring diagrams and documentation

### 🔧 Hardware Changes

| Change | Before | After |
|--------|--------|-------|
| Overwater Indicator | Both LEDs OFF | Blue LED ON (D7) |
| Total LEDs | 2 (Red, Green) | 3 (Red, Green, Blue) |
| Resistors | 2 | 3 |
| Digital Pins Used | 2 (D5, D6) | 3 (D5, D6, D7) |

### 📝 Documentation Updates

- ✅ Updated LED_WIRING_GUIDE.md with Blue LED wiring
- ✅ Updated README.md hardware requirements
- ✅ Updated wiring diagrams with D7
- ✅ Updated LED indicators table
- ✅ Updated CHANGELOG (this file)

### 🎯 Arduino Firmware Changes

**File:** `arduino_sketch.ino`

```cpp
// NEW - Blue LED Pin
#define BLUE_LED_PIN 7      // Blue LED for overwatered

// NEW - Blue LED Initialization
pinMode(BLUE_LED_PIN, OUTPUT);
digitalWrite(BLUE_LED_PIN, LOW);

// UPDATED - LED Control Logic
if (soilPercent < 30) {
  digitalWrite(RED_LED_PIN, HIGH);    // RED ON
  digitalWrite(GREEN_LED_PIN, LOW);
  digitalWrite(BLUE_LED_PIN, LOW);
}
else if (soilPercent <= 70) {
  digitalWrite(RED_LED_PIN, LOW);
  digitalWrite(GREEN_LED_PIN, HIGH);  // GREEN ON
  digitalWrite(BLUE_LED_PIN, LOW);
}
else {
  digitalWrite(RED_LED_PIN, LOW);
  digitalWrite(GREEN_LED_PIN, LOW);
  digitalWrite(BLUE_LED_PIN, HIGH);   // BLUE ON (NEW)
}
```

### 🎨 LED Status Table

| Soil Moisture | LED Status | Meaning |
|---------------|-----------|---------|
| < 30% | 🔴 RED ON | DRY - water needed |
| 30-70% | 🟢 GREEN ON | GOOD - optimal |
| > 70% | 🔵 BLUE ON | OVERWATERED - reduce water |

---

## Version 1.1 — LED Indicators & GitHub Ready

### ✨ New Features

- 🔴 **Red LED Indicator** — Shows when soil is dry (< 30%)
- 🟢 **Green LED Indicator** — Shows when soil is healthy (30-70%)
- 📊 **LED Status in Dashboard** — Web interface mirrors physical LED state
- 🚀 **GitHub Ready** — Complete documentation and setup guide

### 🔧 Hardware Changes

| Change | Before | After |
|--------|--------|-------|
| Display | OLED I2C Display (A4/A5) | Red/Green LEDs (D5/D6) |
| Libraries | DHT + U8g2 | DHT only (simplified) |
| Wiring | 8 pins used | 6 pins used (simpler) |
| Cost | $15-20 | $2-3 (LEDs + resistors) |

### 📝 Documentation Updates

- ✅ Removed OLED display section
- ✅ Added LED Indicators section with behavior table
- ✅ Updated hardware requirements table
- ✅ Updated wiring diagram with LED pins
- ✅ Added LED_WIRING_GUIDE.md with detailed instructions
- ✅ Added GitHub setup section to README
- ✅ Updated Arduino upload instructions (removed U8g2)
- ✅ Added .gitignore file

### 🎯 Arduino Firmware Changes

**File:** `arduino_sketch.ino`

```cpp
// NEW - LED Pin Definitions
#define RED_LED_PIN  5      // New
#define GREEN_LED_PIN 6    // New

// NEW - LED Initialization
pinMode(RED_LED_PIN, OUTPUT);
pinMode(GREEN_LED_PIN, OUTPUT);

// NEW - LED Control Logic
if (soilPercent < 30) {
  digitalWrite(RED_LED_PIN, HIGH);    // Dry - RED ON
  digitalWrite(GREEN_LED_PIN, LOW);
}
else if (soilPercent <= 70) {
  digitalWrite(RED_LED_PIN, LOW);
  digitalWrite(GREEN_LED_PIN, HIGH);  // Good - GREEN ON
}
else {
  digitalWrite(RED_LED_PIN, LOW);     // Overwatered - Both OFF
  digitalWrite(GREEN_LED_PIN, LOW);
}
```

### 🗑️ Removed

- ❌ OLED I2C display support
- ❌ U8g2 library requirements
- ❌ Animated icon rendering (fire, water drop, leaf, bubble)
- ❌ A4/A5 pin usage (freed for other uses)

### 📦 Simplified Dependencies

**Before:**
```json
"express": "^4.18.2",
"better-sqlite3": "^9.4.3",
"serialport": "^12.0.0",
"dotenv": "^16.3.1",
"cors": "^2.8.5"
// Arduino: DHT sensor library + U8g2 library
```

**After:**
```json
// Same Node.js dependencies
// Arduino: DHT sensor library only ✅ Simpler!
```

### 🎨 Dashboard Updates

- ✅ Added LED status indicators in web UI
- ✅ Real-time LED state synchronization
- ✅ Color-coded plant health status

### 🔌 Pin Changes

| Pin | Before | After |
|-----|--------|-------|
| D3 | DHT11 | DHT11 ✅ Same |
| D4 | Relay | Relay ✅ Same |
| A0 | Soil Sensor | Soil Sensor ✅ Same |
| A1 | Water Sensor | Water Sensor ✅ Same |
| A4 | OLED SDA | ❌ Freed |
| A5 | OLED SCL | ❌ Freed |
| D5 | ❌ N/A | 🔴 Red LED |
| D6 | ❌ N/A | 🟢 Green LED |

### 📚 New Files

- 📄 `LED_WIRING_GUIDE.md` — Detailed LED wiring instructions
- 📄 `CHANGELOG.md` — This file
- 📄 `.gitignore` — Git ignore rules

### 🚀 Benefits

| Aspect | Improvement |
|--------|-------------|
| **Cost** | Lower (LEDs are cheap) |
| **Complexity** | Simpler (no I2C) |
| **Reliability** | Better (fewer components) |
| **Power Draw** | Lower (LEDs < OLED) |
| **Setup Time** | Faster (fewer libraries) |
| **Visual Feedback** | Real-time LED status |
| **Portability** | GitHub-ready docs |

### ✅ Testing Checklist

- [ ] Red LED lights up when soil < 30%
- [ ] Green LED lights up when soil 30-70%
- [ ] Both LEDs off when soil > 70%
- [ ] Dashboard shows LED status in real-time
- [ ] Motor control works independently
- [ ] Serial output is valid JSON
- [ ] Database saves readings correctly
- [ ] CSV export works

### 🔄 Migration Path

If upgrading from v1.0:

1. **Disconnect OLED** from A4/A5
2. **Connect Red LED** to D5 (with 220Ω resistor)
3. **Connect Green LED** to D6 (with 220Ω resistor)
4. **Upload new** `arduino_sketch.ino`
5. **No backend changes needed!** (Node.js code unchanged)
6. **Dashboard auto-updates** to show LED status

### 📊 Version Comparison

| Feature | v1.0 | v1.1 |
|---------|------|------|
| Sensors | ✅ | ✅ Same |
| Motor Control | ✅ | ✅ Same |
| Dashboard | ✅ | ✅ Enhanced |
| OLED Display | ✅ | ❌ Removed |
| LED Indicators | ❌ | ✅ Added |
| GitHub Ready | ❌ | ✅ Ready |
| Documentation | Basic | Complete |

---

## Version 1.0 — Initial Release

- ✅ Arduino Uno + 3 sensors (DHT11, Soil, Water Level)
- ✅ OLED display with animated icons
- ✅ Web dashboard with 3D visualization
- ✅ REST API + SQLite database
- ✅ Automated pump control
- ✅ CSV export functionality
