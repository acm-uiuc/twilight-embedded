#include <Wire.h>

#define PIN 9
String data = "";
int count = 0;
int byteArray[4];

void setup() {
    Wire.begin(0x12);
    Wire.onReceive(receiveEvent);
    Wire.onRequest(sendData);
    pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {}

void receiveEvent(int howMany) {
  while (1 < Wire.available()) { // loop through all but the last
    char c = Wire.read(); // receive byte as a character
    Serial.print(c);         // print the character
  }
  int x = Wire.read();    // receive byte as an integer
  Serial.println(x);         // print the integer
}

void sendData() {
    Wire.write(data.c_str());
}



