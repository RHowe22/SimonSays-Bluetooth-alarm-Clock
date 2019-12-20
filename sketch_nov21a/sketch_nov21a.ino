#include <SoftwareSerial.h>

const int txPin = 12;
const int rxPin = 13;

SoftwareSerial BTSerial(rxPin, txPin); // RX, TX

void setup() {
  Serial.begin(9600);
  Serial.println("Enter AT commands:");
  BTSerial.begin(38400);
}

void loop() {
  BTSerial.write(64);
  while(true)
  {
    if(BTSerial.available()){
      Serial.print(BTSerial.read());
      break;
    }
  }
  Serial.print("Finished");
  while(true);
}
