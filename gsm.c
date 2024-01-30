#include <TinyGSM.h>

TinyGSM gsm;

void setup() {
  gsm.begin(&SerialAT); // Replace with your serial port if using a different one
  gsm.sendSMS("+1234567890", "Hello from Arduino!");
}
