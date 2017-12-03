#include "led_driver.hpp"
#include <Wire.h>

#define SIZE 16
int count = 0;
char byteArray[SIZE];
int byteArrayChanged = 0;

void setup() {

    Wire.begin(0x12);
    Wire.onReceive(receiveEvent);
    Wire.onRequest(sendData);
    
    frame.setup();
    frame.SetColor(128, 128, 0);
    frame.UpdateFrame();
}

void loop() {
    if (byteArrayChanged) {
        frame.ApplyCommand(byteArray);
        frame.UpdateFrame();
        byteArrayChanged = 0;
    }
}
void receiveEvent(int howMany) {
    count = 0;
    memset(byteArray, 0, SIZE);
    byteArrayChanged = 1;
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

