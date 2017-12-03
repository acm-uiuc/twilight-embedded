#include "led_driver.hpp"
#include <Wire.h>

int count = 0;
char byteArray[16];
int byteArrayChanged = 0;

void setup() {
    Wire.begin(0x12);
    Wire.onReceive(receiveEvent);
    Wire.onRequest(sendData);
}

void loop() {
    if (byteArrayChanged) {
        //apply_frame_command(String(byteArray));
        byteArrayChanged = 0;
    }
}

void receiveEvent(int howMany) {
 while(Wire.available()) {
      if(count < 16){
        byteArray[count] = Wire.read();
        count++;
      }
      else{
        count = 0;
        byteArray[count] = Wire.read();
      }
    }
    count = 0;        // print the integer
    byteArrayChanged = 1;
}

void sendData() {
    Wire.write(byteArray);
}

