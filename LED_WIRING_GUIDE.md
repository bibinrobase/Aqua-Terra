# 🔌 LED Wiring Guide — Aqua Terra Smart Plant Monitor

## Physical LED Setup

### Components Needed

| Component | Quantity | Notes |
|-----------|----------|-------|
| Red LED (5mm) | 1 | Common cathode, long leg is anode (+) |
| Green LED (5mm) | 1 | Common cathode, long leg is anode (+) |
| Blue LED (5mm) | 1 | Common cathode, long leg is anode (+) |
| 220Ω Resistor | 3 | For current limiting (470Ω also works) |
| Breadboard | 1 | For easy prototyping |
| Jumper wires | 12+ | Male-to-male connections |

### Wiring Steps

#### Step 1: Red LED (D5)

```
Arduino D5 
   ↓
   [220Ω Resistor]
   ↓
Red LED Anode (long leg, +)
   ↓
Red LED Cathode (short leg, -)
   ↓
Arduino GND
```

#### Step 2: Green LED (D6)

```
Arduino D6 
   ↓
   [220Ω Resistor]
   ↓
Green LED Anode (long leg, +)
   ↓
Green LED Cathode (short leg, -)
   ↓
Arduino GND
```

#### Step 3: Blue LED (D7)

```
Arduino D7 
   ↓
   [220Ω Resistor]
   ↓
Blue LED Anode (long leg, +)
   ↓
Blue LED Cathode (short leg, -)
   ↓
Arduino GND
```

#### Step 3: Full Arduino Layout

```
     Arduino Uno
┌────────────────────┐
│ 5V                 │ ──→ DHT11 VCC
│ GND                │ ──→ All GND
│ D3                 │ ──→ DHT11 DATA
│ D4                 │ ──→ Relay Control
│ D5  ──[220Ω]──→ Red LED (+)
│ D6  ──[220Ω]──→ Green LED (+)
│ D7  ──[220Ω]──→ Blue LED (+)
│ A0                 │ ──→ Soil Sensor
│ A1                 │ ──→ Water Sensor
└────────────────────┘
     │
     └──→ GND (Red LED -, Green LED -, Blue LED -)
```

### LED Behavior

| Soil Moisture | Status | Red LED | Green LED | Blue LED |
|---------------|--------|---------|-----------|----------|
| < 30% | 🔴 **DRY** | ✅ ON | ❌ OFF | ❌ OFF |
| 30-70% | 🟢 **GOOD** | ❌ OFF | ✅ ON | ❌ OFF |
| > 70% | 🔵 **OVERWATERED** | ❌ OFF | ❌ OFF | ✅ ON |

## Troubleshooting

### LEDs Not Lighting

**Check these:**
1. ✅ Verify wiring (red/green polarity matters!)
2. ✅ Test with 5V directly from Arduino to confirm LED works
3. ✅ Check resistor value (should be 220Ω, not 220k)
4. ✅ Upload latest sketch with LED code

### One LED Always On/Off

**Possible causes:**
1. ❌ Wrong pin in code (verify D5 and D6)
2. ❌ Sensor not reading correctly (check soil sensor)
3. ❌ Resistor too high (>1kΩ makes LED dim)

### LED Too Dim

**Solutions:**
1. Try 150Ω resistor (brighter, but hotter)
2. Check if sensor reading is in valid range
3. Verify 5V supply is stable

## Arduino Code Reference

```cpp
// LED initialization
pinMode(RED_LED_PIN, OUTPUT);      // D5
pinMode(GREEN_LED_PIN, OUTPUT);    // D6

// LED control logic
if (soilPercent < 30) {
  digitalWrite(RED_LED_PIN, HIGH);    // RED ON
  digitalWrite(GREEN_LED_PIN, LOW);   // GREEN OFF
}
else if (soilPercent <= 70) {
  digitalWrite(RED_LED_PIN, LOW);     // RED OFF
  digitalWrite(GREEN_LED_PIN, HIGH);  // GREEN ON
}
else {
  digitalWrite(RED_LED_PIN, LOW);     // BOTH OFF
  digitalWrite(GREEN_LED_PIN, LOW);
}
```

## Dashboard LED Indicators

The web dashboard also displays LED status in real-time:
- 🔴 **Red indicator** appears when soil is dry
- 🟢 **Green indicator** appears when soil is good
- Both indicators turn off when overwatered

### Why 220Ω Resistor?

- **LEDs need current limiting** to prevent burnout
- 220Ω × 20mA = 4.4V drop (safe for 5V supply)
- Standard value, widely available
- Good brightness without excessive heating

Formula: R = (V_supply - V_led) / I_led
- V_supply = 5V (Arduino)
- V_led = 2V (typical red/green LED)
- I_led = 20mA (safe operating current)
- R = (5 - 2) / 0.020 = **150Ω** (use 220Ω for safety margin)

---

**✅ Ready to wire up your LEDs! Follow the diagram above and upload the latest arduino_sketch.ino**
