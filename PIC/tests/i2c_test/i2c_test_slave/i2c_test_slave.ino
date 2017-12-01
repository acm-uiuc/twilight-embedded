#include <Wire.h>

#define PIN 9
String data = "";
int count = 0;
char byteArray[4];

void setup() {
    Wire.begin(0x12);
    Wire.onReceive(receiveEvent);
    Wire.onRequest(sendData);
    pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {}

void receiveEvent(int howMany) {
 while(Wire.available()){
      if(count < 4){
        byteArray[count] = Wire.read();
        count++;
      }
      else{
        count = 0;
        byteArray[count] = Wire.read();
      }
    }        // print the integer
}

void sendData() {
    Wire.write(byteArray);
}



