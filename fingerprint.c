#include <Adafruit_Fingerprint.h>

Adafruit_Fingerprint finger = Adafruit_Fingerprint(&Serial1);

void setup() {
  finger.begin(57600);
}

void loop() {
  if (finger.verifyPassword()) {
    Serial.println("Fingerprint recognized!");
  } else {
    Serial.println("Fingerprint not recognized!");
  }
}
