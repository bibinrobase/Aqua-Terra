#include <DHT.h>
#include <U8g2lib.h>

// ---------------- PINS ----------------
#define DHT_PIN 3
#define DHT_TYPE DHT11
#define RELAY_PIN 4
#define SOIL_PIN A0
#define WATER_PIN A1

DHT dht(DHT_PIN, DHT_TYPE);
U8G2_SSD1306_128X64_NONAME_1_HW_I2C display(U8G2_R0, U8X8_PIN_NONE);

// ---------------- VARIABLES ----------------
float humidity = 50.0;
float temperature = 25.0;
int soilPct = 0;
int waterPct = 0;

int relayState = HIGH;
int relayDesiredState = HIGH;
unsigned long lastRelayChangeMs = 0;

uint8_t tempFrame = 0;
uint8_t humFrame = 0;
uint8_t soilFrame = 0;
uint8_t waterFrame = 0;

unsigned long lastSensorReadMs = 0;
unsigned long lastSerialSendMs = 0;
unsigned long lastTempAnimMs = 0;
unsigned long lastHumAnimMs = 0;
unsigned long lastSoilAnimMs = 0;
unsigned long lastWaterAnimMs = 0;

const unsigned long SENSOR_INTERVAL_MS = 2000;
const unsigned long SERIAL_INTERVAL_MS = 2000;
const unsigned long TEMP_ANIM_INTERVAL_MS = 95;
const unsigned long HUM_ANIM_INTERVAL_MS = 140;
const unsigned long SOIL_ANIM_INTERVAL_MS = 170;
const unsigned long WATER_ANIM_INTERVAL_MS = 120;
const unsigned long RELAY_DEBOUNCE_MS = 500;  // 500ms debounce to protect relay

// ---------------- ICONS ----------------

// Fire (temperature flicker)
const uint8_t fireFrames[4][8] PROGMEM = {
  { 0x00, 0x10, 0x38, 0x7c, 0x6c, 0x64, 0x38, 0x00 },
  { 0x00, 0x10, 0x10, 0x38, 0x7c, 0x74, 0x6c, 0x38 },
  { 0x00, 0x20, 0x30, 0x78, 0x7c, 0x6c, 0x38, 0x00 },
  { 0x00, 0x10, 0x38, 0x7c, 0x5c, 0x6c, 0x38, 0x00 },
};

// Bubble pop (humidity)
const uint8_t bubbleFrames[4][8] PROGMEM = {
  { 0x00, 0x00, 0x00, 0x18, 0x18, 0x00, 0x00, 0x00 },
  { 0x00, 0x00, 0x18, 0x24, 0x24, 0x18, 0x00, 0x00 },
  { 0x00, 0x18, 0x24, 0x42, 0x42, 0x24, 0x18, 0x00 },
  { 0x42, 0x81, 0x00, 0x00, 0x00, 0x00, 0x81, 0x42 },
};

// Leaf sway (soil)
const uint8_t leafFrames[4][8] PROGMEM = {
  { 0x08, 0x1c, 0x3c, 0x3e, 0x1e, 0x0c, 0x04, 0x00 },
  { 0x10, 0x18, 0x3c, 0x3e, 0x1e, 0x0c, 0x04, 0x00 },
  { 0x20, 0x70, 0x7c, 0x3e, 0x0e, 0x0c, 0x04, 0x00 },
  { 0x10, 0x18, 0x3c, 0x3e, 0x1e, 0x0c, 0x04, 0x00 },
};

// Water droplet fall (water)
const uint8_t dropFrames[4][8] PROGMEM = {
  { 0x08, 0x1c, 0x3e, 0x3e, 0x1c, 0x00, 0x00, 0x00 },
  { 0x00, 0x08, 0x1c, 0x3e, 0x3e, 0x1c, 0x00, 0x00 },
  { 0x00, 0x00, 0x08, 0x1c, 0x3e, 0x3e, 0x1c, 0x00 },
  { 0x00, 0x00, 0x00, 0x08, 0x1c, 0x3e, 0x3e, 0x1c },
};

// ---------------- SAFE DHT ----------------
void readDHT() {
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  if (!isnan(h)) humidity = h;
  if (!isnan(t)) temperature = t;
}

void readAnalogSensors() {
  int soilRaw = analogRead(SOIL_PIN);
  int waterRaw = analogRead(WATER_PIN);

  soilPct = map(soilRaw, 1023, 300, 0, 100);
  waterPct = map(waterRaw, 0, 500, 0, 100);

  soilPct = constrain(soilPct, 0, 100);
  waterPct = constrain(waterPct, 0, 100);

  // Determine desired relay state based on sensors
  // Motor runs only if soil is dry AND water level is adequate
  relayDesiredState = (soilPct < 30 && waterPct >= 20) ? LOW : HIGH;
}

// Safe relay switching with debouncing to prevent voltage spikes
void updateRelayState() {
  unsigned long now = millis();

  // Only change relay state if debounce period has passed
  if (relayDesiredState != relayState && (now - lastRelayChangeMs >= RELAY_DEBOUNCE_MS)) {
    relayState = relayDesiredState;
    digitalWrite(RELAY_PIN, relayState);
    lastRelayChangeMs = now;
  }
}

void sendSerialJson() {
  Serial.print("{\"humidity\":");
  Serial.print(humidity, 1);
  Serial.print(",\"temperature\":");
  Serial.print(temperature, 1);
  Serial.print(",\"soil\":");
  Serial.print(soilPct);
  Serial.print(",\"waterLevel\":");
  Serial.print(waterPct);
  Serial.print(",\"relay\":");
  Serial.print(relayState == LOW ? 1 : 0);
  Serial.print(",\"motorRunning\":");
  Serial.print(relayState == LOW ? 1 : 0);
  Serial.println("}");
}

void drawIcon8x8(int x, int y, const uint8_t* icon) {
  display.drawXBMP(x, y, 8, 8, icon);
}

// ---------------- DRAW UI ----------------
void drawUI() {

  display.firstPage();
  do {
    char tempStr[10];
    char humStr[10];
    char soilStr[10];
    char waterStr[10];

    dtostrf(temperature, 4, 1, tempStr);
    dtostrf(humidity, 4, 1, humStr);

    sprintf(soilStr, "%3d%%", soilPct);
    sprintf(waterStr, "%3d%%", waterPct);

    display.setFont(u8g2_font_6x12_tr);

    // ---------- TITLE ----------
    display.drawStr(28, 10, "AQUA TERRA");

    int iconX = 6;
    int labelX = 20;
    int valueRightX = 120;

    int y1 = 24;
    int y2 = 36;
    int y3 = 48;
    int y4 = 60;

    // ---------- TEMP ----------
    drawIcon8x8(iconX, y1 - 8, fireFrames[tempFrame]);
    display.drawStr(labelX, y1, "Temp");
    display.drawStr(valueRightX - display.getStrWidth(tempStr) - 10, y1, tempStr);
    display.drawStr(valueRightX - 8, y1, "C");

    // ---------- HUM ----------
    drawIcon8x8(iconX, y2 - 8, bubbleFrames[humFrame]);
    display.drawStr(labelX, y2, "Hum ");
    display.drawStr(valueRightX - display.getStrWidth(humStr) - 10, y2, humStr);
    display.drawStr(valueRightX - 8, y2, "%");

    // ---------- SOIL (SHIFTED LEFT ~3px) ----------
    drawIcon8x8(iconX, y3 - 8, leafFrames[soilFrame]);
    display.drawStr(labelX, y3, "Soil");
    display.drawStr(valueRightX - display.getStrWidth(soilStr) - 3, y3, soilStr);

    // ---------- WATER (SHIFTED LEFT ~3px) ----------
    drawIcon8x8(iconX, y4 - 8, dropFrames[waterFrame]);
    display.drawStr(labelX, y4, "Water");
    display.drawStr(valueRightX - display.getStrWidth(waterStr) - 3, y4, waterStr);

  } while (display.nextPage());
}

// ---------------- SETUP ----------------
void setup() {
  Serial.begin(9600);

  dht.begin();
  display.begin();
  display.setContrast(255);

  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, HIGH);

  readDHT();
  readAnalogSensors();
  sendSerialJson();

  delay(2000);
}

// ---------------- LOOP ----------------
void loop() {
  unsigned long now = millis();

  if (now - lastSensorReadMs >= SENSOR_INTERVAL_MS) {
    lastSensorReadMs = now;
    readDHT();
    readAnalogSensors();
    updateRelayState();  // Safely update relay with debouncing
  }

  if (now - lastSerialSendMs >= SERIAL_INTERVAL_MS) {
    lastSerialSendMs = now;
    sendSerialJson();
  }

  if (now - lastTempAnimMs >= TEMP_ANIM_INTERVAL_MS) {
    lastTempAnimMs = now;
    tempFrame = (tempFrame + 1) % 4;
  }

  if (now - lastHumAnimMs >= HUM_ANIM_INTERVAL_MS) {
    lastHumAnimMs = now;
    humFrame = (humFrame + 1) % 4;
  }

  if (now - lastSoilAnimMs >= SOIL_ANIM_INTERVAL_MS) {
    lastSoilAnimMs = now;
    soilFrame = (soilFrame + 1) % 4;
  }

  if (now - lastWaterAnimMs >= WATER_ANIM_INTERVAL_MS) {
    lastWaterAnimMs = now;
    waterFrame = (waterFrame + 1) % 4;
  }

  drawUI();

  delay(50);
}