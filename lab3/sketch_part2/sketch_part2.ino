
const int PHOTO_PIN = A0;

const int TRIG_PIN = 7;
const int ECHO_PIN = 8;

const int BLUE_PIN  = 9;
const int RED_PIN   = 10;
const int GREEN_PIN = 11;

const bool COMMON_ANODE = false;

// HC-SR04 = ~2cm to 400cm
// 0 - 100 cm instead of 398 cm
// 10 - 40 --> 30 cm --> ~ 1 ft
const int MIN_CM = 10;
const int MAX_CM = 40;

unsigned long lastUpdateMs = 0;

long readUltrasonicMicroseconds() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  long duration = pulseIn(ECHO_PIN, HIGH, 30000);

  return duration;
}

int clampInt(int x, int lo, int hi) {
  if (x < lo) return lo;
  if (x > hi) return hi;
  return x;
}

void writeRGB(int r, int g, int b) {
  r = clampInt(r, 0, 255);
  g = clampInt(g, 0, 255);
  b = clampInt(b, 0, 255);

  if (COMMON_ANODE) {
    r = 255 - r;
    g = 255 - g;
    b = 255 - b;
  }

  analogWrite(RED_PIN, r);
  analogWrite(GREEN_PIN, g);
  analogWrite(BLUE_PIN, b);
}

void setup() {
  Serial.begin(9600);

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);
}

void loop() {
  unsigned long now = millis();
  if (now - lastUpdateMs >= 1000) {
    lastUpdateMs = now;

    int photoRaw = analogRead(PHOTO_PIN);
    int brightness = photoRaw / 4;

    long echoUs = readUltrasonicMicroseconds();

    float distanceCm = 0;
    if (echoUs > 0) {
      distanceCm = echoUs / 58.0;
    }

    int distInt;

    if (echoUs > 0) {
      distInt = (int)(distanceCm + 0.5);
    } else {
      distInt = MAX_CM;
    }
    distInt = clampInt(distInt, MIN_CM, MAX_CM);

    int dist255 = map(distInt, MIN_CM, MAX_CM, 0, 255);

    // close -> green (0 => green=255, red=0)
    // far   -> red   (255 => red=255, green=0)
    int baseR = dist255;
    int baseG = 255 - dist255;
    int baseB = 0;

    int outR = (baseR * brightness) / 255;
    int outG = (baseG * brightness) / 255;
    int outB = (baseB * brightness) / 255;

    writeRGB(outR, outG, outB);

    Serial.print(" Brightness(0-255): ");
    Serial.print(brightness);
    Serial.print(" | ");

    Serial.print(" Dist(cm): ");
    Serial.print(distInt);
    Serial.print(" | ");
    Serial.print(" Dist255: ");
    Serial.print(dist255);
    Serial.println();

  }
}