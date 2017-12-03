#include "led_driver.hpp"
#include <Wire.h>

int count = 0;
char byteArray[32];

void setup() {
    Wire.begin(0x12);
    Wire.onReceive(receiveEvent);
    Wire.onRequest(sendData);
}

void loop() {}

void receiveEvent(int howMany) {
 while(Wire.available()){
      if(count < 32){
        byteArray[count] = Wire.read();
        count++;
      }
      else{
        count = 0;
        byteArray[count] = Wire.read();
      }
    }
    count = 0;        // print the integer
    apply_frame_command(String(byteArray));
}

void sendData() {
    Wire.write(byteArray);
}
