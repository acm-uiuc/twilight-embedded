#include "led_driver.hpp"
#include <Wire.h>

int count = 0;
char byteArray[16];
int byteArrayChanged = 0;

void setup() {

    Wire.begin(0x12);
    Wire.onReceive(receiveEvent);
    Wire.onRequest(sendData);
    
    frame.setup();
    frame.SetColor(0, 255, 0);
    frame.UpdateFrame();
}

void loop() {
    if (byteArrayChanged) {
        frame.SetColor(100, 12, 80);
        frame.UpdateFrame();
        frame.ApplyCommand(byteArray);
        frame.UpdateFrame();
        byteArrayChanged = 0;
    }
}

void receiveEvent(int howMany) {
    count = 0;
    memset(byteArray, 0, 16);
    byteArrayChanged = 1;
    while(Wire.available()) {
        if (count < 16) {
            byteArray[count] = Wire.read();
            count++;
        } else {
            break;
            count = 0;
            byteArray[count] = Wire.read();
        }
    }
}

void sendData() {
    Wire.write(byteArray);
}

