#include <Wire.h>

String data = "";

void setup() {
    Wire.begin(0x12);
    Wire.onReceive(receiveData);
    Wire.onRequest(sendData);
}

void loop() {}

void receiveData(int bytes) {
      data = "";
  while( Wire.available()){
    data += (char)Wire.read();
  }
}

void sendData() {
    Wire.write(data.c_str());
}


