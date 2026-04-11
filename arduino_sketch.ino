#include <DHT.h>

// ----------- PINS -----------
#define RELAY_PIN 4
#define WATER_PIN A1
#define SOIL_PIN  A0
#define DHT_PIN   3
#define RED_LED_PIN  5      // Red LED for dry soil
#define GREEN_LED_PIN 6    // Green LED for good soil
#define BLUE_LED_PIN 7     // Blue LED for overwatered

#define DHT_TYPE DHT11

DHT dht(DHT_PIN, DHT_TYPE);

// ----------- THRESHOLDS -----------
int WATER_EMPTY = 300;    // Raw value when tank empty
int WATER_FULL = 600;     // Raw value when tank full
int SOIL_EMPTY = 1023;    // Raw value when soil empty (dry)
int SOIL_FULL = 300;        // Raw value when soil saturated (wet)

// ----------- VARIABLES -----------
float temperature = 0;
float humidity = 0;

void setup() {
  Serial.begin(9600);

  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, HIGH); // Motor OFF

  pinMode(RED_LED_PIN, OUTPUT);
  pinMode(GREEN_LED_PIN, OUTPUT);
  pinMode(BLUE_LED_PIN, OUTPUT);
  digitalWrite(RED_LED_PIN, LOW);
  digitalWrite(GREEN_LED_PIN, LOW);
  digitalWrite(BLUE_LED_PIN, LOW);

  dht.begin();

  Serial.println("Smart Irrigation System with LED Indicators (DHT11 Version)");
}

void loop() {

  // ----------- READ SENSORS -----------
  int soilRaw  = analogRead(SOIL_PIN);
  int waterRaw = analogRead(WATER_PIN);

  // ----------- CONVERT TO PERCENTAGES -----------
  // Water: 150 = 0%, 550 = 100%
  int waterPercent = map(waterRaw, WATER_EMPTY, WATER_FULL, 0, 100);
  waterPercent = constrain(waterPercent, 0, 100);

  // Soil: 1023 = 0% (dry), 0 = 100% (saturated)
  int soilPercent = map(soilRaw, SOIL_EMPTY, SOIL_FULL, 0, 100);
  soilPercent = constrain(soilPercent, 0, 100);

  temperature = dht.readTemperature();
  humidity    = dht.readHumidity();

  // ----------- SOIL STATE DESCRIPTION -----------
  const char* soilState;
  if (soilRaw > 600) {
    soilState = "DRY";
  }
  else if (soilRaw >= 550) {
    soilState = "GOOD";
  }
  else {
    soilState = "OVERWATERED";
  }

  // ----------- CONDITIONS -----------
  bool waterAvailable = waterPercent > 10;   // More than 10% water available
  bool soilDry = soilRaw > 600;              // Soil is dry

  // ----------- LED INDICATORS -----------
  // RED LED: Plant is dry (soil < 30%)
  // GREEN LED: Plant is healthy (soil 30-70%)
  // BLUE LED: Plant is overwatered (soil > 70%)
  if (soilPercent < 30) {
    digitalWrite(RED_LED_PIN, HIGH);    // Soil is DRY - Turn RED LED ON
    digitalWrite(GREEN_LED_PIN, LOW);   // Turn GREEN LED OFF
    digitalWrite(BLUE_LED_PIN, LOW);    // Turn BLUE LED OFF
  } 
  else if (soilPercent <= 70) {
    digitalWrite(RED_LED_PIN, LOW);     // Turn RED LED OFF
    digitalWrite(GREEN_LED_PIN, HIGH);  // Soil is GOOD - Turn GREEN LED ON
    digitalWrite(BLUE_LED_PIN, LOW);    // Turn BLUE LED OFF
  }
  else {
    digitalWrite(RED_LED_PIN, LOW);     // Turn RED LED OFF
    digitalWrite(GREEN_LED_PIN, LOW);   // Turn GREEN LED OFF
    digitalWrite(BLUE_LED_PIN, HIGH);   // OVERWATERED - Turn BLUE LED ON
  }

  // ----------- RELAY CONTROL -----------
  if (waterAvailable && soilDry) {
    digitalWrite(RELAY_PIN, LOW); // Motor ON
  } else {
    digitalWrite(RELAY_PIN, HIGH); // Motor OFF
  }

  // ----------- SERIAL OUTPUT (JSON with percentages) -----------
  Serial.print("{\"temperature\":");
  Serial.print(temperature, 1);
  Serial.print(",\"humidity\":");
  Serial.print(humidity, 1);
  Serial.print(",\"soil\":");
  Serial.print(soilPercent);       // Send percentage, not raw
  Serial.print(",\"waterLevel\":");
  Serial.print(waterPercent);      // Send percentage, not raw
  Serial.print(",\"soilState\":\"");
  Serial.print(soilState);
  Serial.print("\",\"relay\":");
  Serial.print(digitalRead(RELAY_PIN) == LOW ? 1 : 0);
  Serial.print(",\"motorRunning\":");
  Serial.print(digitalRead(RELAY_PIN) == LOW ? 1 : 0);
  Serial.println("}");

  delay(1000);
}