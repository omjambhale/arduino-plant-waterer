// =====================================================
// Plant Waterer - Arduino UNO R4 WiFi
// Senses soil moisture, decides if the plant is dry,
// and waters it with a small pump.
//   Sense  -> capacitive soil sensor on A0
//   Think  -> compare moisture to a threshold
//   Act    -> red/green LEDs + pump via S8050 transistor
// =====================================================

// true  = fake soil that slowly dries out (demo, no sensor needed)
// false = real soil sensor (set DRY_READING / WET_READING first)
const bool DEMO_MODE = true;

// Pins
const int SENSOR_PIN = A0;
const int RED_LED    = 2;
const int GREEN_LED  = 3;
const int PUMP_PIN   = 11;

// Calibration: replace with your own sensor readings
const int DRY_READING = 520;   // sensor held in air
const int WET_READING = 260;   // sensor in a glass of water
const int WATER_BELOW = 30;    // water when moisture is below 30 %

int fakeMoisture = 60;

void setup() {
  Serial.begin(9600);
  pinMode(RED_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(PUMP_PIN, OUTPUT);

  // Self-test on every power-up: red, green, pump for 1 s
  Serial.println("Self-test");
  digitalWrite(RED_LED, HIGH);   delay(700);  digitalWrite(RED_LED, LOW);
  digitalWrite(GREEN_LED, HIGH); delay(700);  digitalWrite(GREEN_LED, LOW);
  digitalWrite(PUMP_PIN, HIGH);  delay(1000); digitalWrite(PUMP_PIN, LOW);
}

// Returns soil moisture as 0-100 %
int readMoisture() {
  if (DEMO_MODE) return fakeMoisture;
  int raw = analogRead(SENSOR_PIN);              // 0-1023, lower = wetter
  Serial.print("raw "); Serial.print(raw); Serial.print("  ");
  return constrain(map(raw, DRY_READING, WET_READING, 0, 100), 0, 100);
}

void loop() {
  int moisture = readMoisture();                 // SENSE
  Serial.print("moisture "); Serial.print(moisture); Serial.println("%");

  if (moisture < WATER_BELOW) {                  // THINK: dry
    Serial.println("DRY -> watering");
    digitalWrite(GREEN_LED, LOW);
    digitalWrite(RED_LED, HIGH);                 // ACT
    digitalWrite(PUMP_PIN, HIGH); delay(3000);   // pump for 3 s
    digitalWrite(PUMP_PIN, LOW);
    if (DEMO_MODE) fakeMoisture = 70;            // pretend the soil got wet
    delay(5000);                                 // let the water soak in
  } else {                                       // wet enough
    digitalWrite(RED_LED, LOW);
    digitalWrite(GREEN_LED, HIGH);
    if (DEMO_MODE) fakeMoisture -= 10;           // pretend the soil dries out
    delay(2000);
  }
}
