#include <TinyGSM.h>
#include <Adafruit_Fingerprint.h>
#include <MFRC522.h>

// Replace with your actual pin connections and phone number
#define GSM_SERIAL_PORT SerialAT
#define FINGERPRINT_SERIAL_PORT Serial1
#define RFID_SS_PIN 10
#define RFID_RST_PIN 9
const char* PHONE_NUMBER = "+1234567890";

// Authorized fingerprint IDs and RFID tag IDs
int authorizedFingerprintIDs[] = {1, 2, 3}; // Example IDs
byte authorizedTagIDs[][4] = {
  {0x01, 0x23, 0x45, 0x67},
  {0x78, 0x9a, 0xbc, 0xde}
};

TinyGSM gsm;
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&FINGERPRINT_SERIAL_PORT);
MFRC522 mfrc522(RFID_SS_PIN, RFID_RST_PIN);

void setup() {
  Serial.begin(9600);
  GSM_SERIAL_PORT.begin(9600);

  gsm.begin(&GSM_SERIAL_PORT);
  finger.begin(57600);
  SPI.begin();
  mfrc522.PCD_Init();

  // Add more initialization code as needed
}

void loop() {
  // Check for fingerprint
  finger.checkPrint();
  if (finger.fingerID > 0) {
    if (isFingerprintAuthorized(finger.fingerID)) {
      Serial.println("Authorized fingerprint!");
      // Grant access and perform actions
    } else {
      Serial.println("Unauthorized fingerprint!");
      sendSMSNotification("Unauthorized fingerprint access attempt!");
    }
  }

  // Check for RFID tag
  if (mfrc522.PICC_IsNewCardPresent()) {
    mfrc522.PICC_ReadCardSerial();
    if (isRFIDTagAuthorized(mfrc522.uid.uidByte)) {
      Serial.println("Authorized RFID tag!");
      // Grant access and perform actions
    } else {
      Serial.println("Unauthorized RFID tag!");
      sendSMSNotification("Unauthorized RFID tag access attempt!");
    }
  }
}

bool isFingerprintAuthorized(int id) {
  for (int authorizedID : authorizedFingerprintIDs) {
    if (id == authorizedID) {
      return true;
    }
  }
  return false;
}

bool isRFIDTagAuthorized(byte tagID[]) {
  for (byte authorizedTag[] : authorizedTagIDs) {
    if (memcmp(tagID, authorizedTag, 4) == 0) {
      return true;
    }
  }
  return false;
}

void sendSMSNotification(const char* message) {
  gsm.sendSMS(PHONE_NUMBER, message);
}
