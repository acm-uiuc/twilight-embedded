#include <Wire.h>

#define PIN 9
#define SIZE 16
String data = "";
int count = 0;

char byteArray[SIZE];

void setup() {
    Wire.begin(0x12);
    Wire.onReceive(receiveEvent);
    Wire.onRequest(sendData);
    pinMode(LED_BUILTIN, OUTPUT);
}

void loop() { }

void receiveEvent(int howMany) {
    while (Wire.available()) {
        if (count < SIZE) {
            byteArray[count] = Wire.read();
            count++;
        } else {
            count = 0;
            byteArray[count] = Wire.read();
        }
    }
    // print the integer
}

void sendData() {
    Wire.write(byteArray);
}
