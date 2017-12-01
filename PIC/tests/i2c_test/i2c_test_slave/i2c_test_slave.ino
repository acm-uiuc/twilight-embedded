#include <Wire.h>

static const char messageInterrupt = A0;
static const char numberInterrupt = A1;

int flag = 0;

void setup() {
    pinMode(numberInterrupt, OUTPUT);
    pinMode(messageInterrupt, OUTPUT);

    Serial.begin(9600);

    Wire.begin(0x12);
    Wire.onReceive(receiveData);
    Wire.onRequest(sendData);
    digitalWrite(numberInterrupt, LOW);
    digitalWrite(messageInterrupt, LOW);

}

void loop() {}

void receiveData(int bytes) {}

void sendData() {
    if(flag == 2) {
      Wire.write("Hello foo");
      delay(1000);
    }

    if(flag == 1) {
        Wire.write("Hello bar");
        delay(1000);  //if no sync : add more delay.
    }

    flag = 0;
}
