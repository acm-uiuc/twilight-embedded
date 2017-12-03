#include "led_driver.hpp"
#include <Wire.h>

#define SIZE 64
#define EOT  4
int count = 0;
char byteArray[SIZE];
char outputStr[SIZE];
int byteArrayChanged = 0;

void setup() {
    memset(byteArray, 0, SIZE);
  
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

    while (Wire.available()) {
        int readByte = Wire.read();
        if (readByte == EOT) {
            byteArray[count] = 0;
            count = 0;
            byteArrayChanged = 1;
            break;
        }

        byteArrayChanged = 0;
        byteArray[count] = readByte;
        count++;

        if (count == SIZE) {
            count = 0;
        }
    }
}


void sendData() {
    Wire.write(byteArray);
}

