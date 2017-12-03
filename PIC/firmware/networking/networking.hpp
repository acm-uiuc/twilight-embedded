#ifndef NETWORKING_HPP
#define NETWORKING_HPP

#include "Arduino.h"
#include <ArduinoSTL.h>
#include "../constants/constants.hpp"

typedef struct NetworkExchange_struct {
    std::vector<String> inbox;
    std::vector<String> outbox;
} NetworkExchange;

NetworkExchange interconnect = NetworkExchange();

////////////////////// INTERFACE ///////////////////////
void setup_networking() {
    Serial.begin(BAUDRATE);  //WESTBOUND
    Serial1.begin(BAUDRATE); //EASTBOUND
    Serial2.begin(BAUDRATE); //NORTHBOUND
    Serial3.begin(BAUDRATE); //SOUTHBOUND
}

void send_msg(String msg) {
    interconnect.outbox.push_back(msg);
}

void send_msgs(std::vector<String> msgs) {
    interconnect.outbox.insert(interconnect.outbox.end(), msgs.begin(), msgs.end());
}

std::vector<String> recv_msgs() {
    std::vector<String> msgs = interconnect.inbox;
    interconnect.inbox.clear();
    return msgs;
}
////////////////////////////////////////////////////////

void setup_networking();
void handle_network_msgs();
void multicast(String msg);



void multicast(String msg) {
    Serial.print(msg);
    Serial1.print(msg);
    Serial2.print(msg);
    Serial3.print(msg);
}

void handle_network_msgs() {
    // Get new messages from other nodes
    if (Serial.available()) {
        String msg = Serial.readString();
        interconnect.inbox.push_back(String(msg + ';' + String(WEST)));
    }

    if (Serial1.available()) {
        String msg = Serial1.readString();
        interconnect.inbox.push_back(String(msg + ';' + String(EAST)));
    }

    if (Serial2.available()) {
        String msg = Serial2.readString();
        interconnect.inbox.push_back(String(msg + ';' + String(NORTH)));
    }

    if (Serial3.available()) {
        String msg = Serial3.readString();
        interconnect.inbox.push_back(String(msg + ';' + String(SOUTH)));
    }

    // Send messages in outbox
    for (auto &msg: interconnect.outbox) {
        multicast(msg);
    }
    interconnect.outbox.clear();
}

#endif //NETWORKING_H