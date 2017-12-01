#include <Wire.h>

void setup() {
    Wire.begin(0x12);
    Wire.onReceive(receiveData);
    Wire.onRequest(sendData);
}

void loop() {}

void receiveData(int bytes) {}

void sendData() {
    Wire.write("Hello foo");
}

