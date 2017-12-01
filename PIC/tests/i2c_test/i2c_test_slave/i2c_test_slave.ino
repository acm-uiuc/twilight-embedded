#include <Wire.h>

#define PIN 9
String data = "";
int count = 0;
int byteArray[4];

void setup() {
    Wire.begin(0x12);
    Wire.onReceive(receiveData);
    Wire.onRequest(sendData);
    pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {}

void receiveData(int numByte){
       digitalWrite(9, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);                       // wait for a second
  digitalWrite(9, LOW);    // turn the LED off by making the voltage LOW
  delay(1000);

    while(Wire.available()){
      if(count < 4){
        byteArray[count] = Wire.read();
        count++;
      }
      else{
        count = 0;
        byteArray[count] = Wire.read();
      }
    }
     digitalWrite(9, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);                       // wait for a second
  digitalWrite(9, LOW);    // turn the LED off by making the voltage LOW
  delay(1000);                       // wait for a second
}  

void sendData() {
    Wire.write(data.c_str());
}



