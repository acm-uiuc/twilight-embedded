#include <Wire.h>

#define PIN 9
String data = "";

void setup() {
    Wire.begin(0x12);
    Wire.onReceive(receiveData);
    Wire.onRequest(sendData);
    pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {}

void receiveData(int bytes) {
    data = "";
    while( Wire.available()){
        data += (char)Wire.read();
    }
    digitalWrite(PIN, HIGH);   // turn the LED on (HIGH is the voltage level)
}
void sendData() {
    Wire.write(data.c_str());
}



