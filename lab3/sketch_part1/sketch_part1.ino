// More light on the photocell should make ADC go UP

// Less light (cover it) should make ADC go DOWN

// The LED should get brighter as ADC goes up

const int PHOTO_PIN = A0;
const int BLUE_PIN  = 9;

unsigned long lastPrint = 0;

void setup() {
  Serial.begin(9600);
  pinMode(BLUE_PIN, OUTPUT);
}

void loop() {

  int rawValue = analogRead(PHOTO_PIN);

  int brightness = rawValue / 4;

  analogWrite(BLUE_PIN, brightness);

  if (millis() - lastPrint >= 1000) {
    lastPrint = millis();

    Serial.print("ADC: ");
    Serial.print(rawValue);
    Serial.print(" ");
    Serial.print("PWM: ");
    Serial.println(brightness);
  }
}