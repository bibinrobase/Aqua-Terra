# 🚀 Quick Start — Aqua Terra LED Edition

## What's Changed?

### ✅ Removed OLED
- No more I2C display
- Simpler wiring
- Fewer libraries

### ✅ Added Red/Green/Blue LEDs
- **Red LED (D5)** = Soil is DRY (< 30%)
- **Green LED (D6)** = Soil is GOOD (30-70%)
- **Blue LED (D7)** = Soil is OVERWATERED (> 70%)

---

## 🔧 Hardware Setup (5 minutes)

### Components

```
✓ Arduino Uno
✓ DHT11 (temp + humidity)
✓ Soil Moisture Sensor
✓ Water Level Sensor
✓ 1 Red LED (5mm)
✓ 1 Green LED (5mm)
✓ 1 Blue LED (5mm)
✓ 3x 220Ω Resistors
✓ Water Pump + Relay Module (optional)
```

### Wiring

```
Arduino    Component          Notes
─────────────────────────────────────
5V      → DHT11 VCC, Sensors VCC
GND     → All GND
D3      → DHT11 DATA
D4      → Relay IN
D5      → Red LED (+) with 220Ω resistor
D6      → Green LED (+) with 220Ω resistor
D7      → Blue LED (+) with 220Ω resistor
A0      → Soil Moisture Signal
A1      → Water Level Signal
```

**See LED_WIRING_GUIDE.md for detailed diagrams**

---

## 💻 Software Setup (10 minutes)

### 1. Upload Arduino Sketch

```bash
# In Arduino IDE:
1. Install DHT library (Sketch → Include Library → Manage Libraries)
2. Open arduino_sketch.ino
3. Select Board: Arduino Uno
4. Click Upload
```

### 2. Install Node.js Backend

```bash
# In terminal:
npm install
```

### 3. Start Server

```bash
npm start
```

Dashboard at: **http://localhost:3000**

---

## 📊 LED Status Indicators

| Soil Moisture | RED | GREEN | BLUE | Meaning |
|---------------|-----|-------|------|---------|
| < 30% | ✅ ON | ❌ OFF | ❌ OFF | 🔴 DRY - Water soon |
| 30-70% | ❌ OFF | ✅ ON | ❌ OFF | 🟢 GOOD - Optimal |
| > 70% | ❌ OFF | ❌ OFF | ✅ ON | 🔵 OVERWATERED |

---

## 📱 Web Dashboard Features

- ✅ Real-time sensor data
- ✅ LED status mirror
- ✅ Historical charts
- ✅ Motor control
- ✅ CSV export
- ✅ 3D visualization

---

## ✅ Testing Checklist

- [ ] Red LED lights up when you dry out soil
- [ ] Green LED lights up when soil is moist
- [ ] Both LEDs turn off when you water excessively
- [ ] Dashboard shows matching LED status
- [ ] Pump turns on when soil is dry and water available
- [ ] Serial port connects in dashboard
- [ ] Historical data saves to database

---

## 🐛 Troubleshooting

### LEDs Not Working

```
1. Check polarity (RED LONG = +)
2. Verify resistor (should be 220Ω, not 220k)
3. Test with 5V direct from Arduino
4. Check soil sensor reading (0-100%)
```

### Serial Not Connecting

```
1. Check Arduino USB cable
2. Verify COM port in Windows Device Manager
3. Restart browser and server
4. Check .env SERIAL_PORT setting
```

### Motor Not Running

```
1. Check relay wiring
2. Verify water level > 10%
3. Confirm soil < 30%
4. Test relay with manual 5V
```

---

## 📂 Project Files

```
aqua-terra/
├── README.md                 ← Start here
├── LED_WIRING_GUIDE.md       ← LED setup
├── CHANGELOG.md              ← What changed
├── arduino_sketch.ino        ← Upload this
├── server.js                 ← Node.js backend
├── database.js               ← SQLite helper
├── index.html                ← Web dashboard
├── package.json              ← Dependencies
├── .env                      ← Configuration
└── .gitignore                ← Git ignore
```

---

## 🌐 GitHub Deployment

### Initialize Git

```bash
git init
git add .
git commit -m "Aqua Terra: Smart Plant Monitor with LED Indicators"
git branch -M main
git remote add origin https://github.com/YOUR_USERNAME/aqua-terra.git
git push -u origin main
```

### GitHub Features

- ✅ Complete documentation
- ✅ Wiring guides
- ✅ Arduino code
- ✅ Node.js backend
- ✅ MIT License
- ✅ .gitignore ready

---

## 🎯 Next Steps

1. ✅ Wire up LEDs (see LED_WIRING_GUIDE.md)
2. ✅ Upload Arduino sketch
3. ✅ Install Node.js packages
4. ✅ Start server (npm start)
5. ✅ Open dashboard (http://localhost:3000)
6. ✅ Test LED indicators
7. ✅ Push to GitHub

---

**Ready to go! Questions? Check the README or LED_WIRING_GUIDE.md** 🌿💡
