#ifndef NODE_SLAVE_HPP
#define NODE_SLAVE_HPP

#include "../networking/networking.hpp"
#include "../constants/constants.hpp"
#include "Arduino.h"
#include <Wire.h>

///////////////////////// MAIN CTRLR THREAD //////////////////////////
// The Node Slave instance controls execution of operations on the
// controller. Assigns the box color and sends messages.
//////////////////////////////////////////////////////////////////////

class NodeSlave {
public:
    void GetIncommingNetworkMsgs();
    void GetIncommingNodeMsgs();
    void SendMsgsToNode();
    void SendMsgsToNetwork();
    void ApplyNodeLEDCmd();
    void SortMsgsFromNode();

    uint8_t i2c_buffer_pos = 0;
    uint8_t i2c_buffer_ready = 0;
    char i2c_buffer[BUFSIZE];
    std::vector<String> node_inbox;          // QUEUE OF MESSAGES FROM THE NODE

private:

    std::vector<String> node_outbox;         // QUEUE OF MESSAGES TO GO TO THE NODE
    std::vector<String> network_outbox;      // QUEUE OF MESSAGES TO GO OUT TO THE NETWORK
    String led_cmd;                          // QUEUE OF COMMANDS TO RUN ON THE LEDS (RIGHT NOW ITS JUST A SINGLE CMD)
    uint8_t led_cmd_ready = 0;
};



//Get incomming messages from the Network
void NodeSlave::GetIncommingNetworkMsgs() {
    std::vector<String> msgs = recv_msgs();
    node_outbox.insert(node_outbox.end(), msgs.begin(), msgs.end());
}

void NodeSlave::GetIncommingNodeMsgs() {
    // Serial.println("GetIncommingNodeMsgs");
    // current implementation will drop a message if the next one
    // is sent before the thread consumes the original message

    while (Wire.available()) {
        int readByte = Wire.read();

        if (readByte == STX) {
            i2c_buffer_pos = 0;
            i2c_buffer_ready = 0;
        } else if (readByte == ETX) {
            i2c_buffer[i2c_buffer_pos] = 0;
            i2c_buffer_ready = 1;
        } else {
            i2c_buffer[i2c_buffer_pos] = readByte;
            i2c_buffer_pos++;
        }

        if (i2c_buffer_pos == BUFSIZE) {
            memset(i2c_buffer, 0, BUFSIZE);
            i2c_buffer_pos = 0;
        }
    }
}

void NodeSlave::SendMsgsToNetwork() {
    send_msgs(this->network_outbox);
    this->network_outbox.clear();
}

void NodeSlave::SendMsgsToNode() {
    //I2C Magic
    if (this->node_outbox.size() == 0) {
        Wire.write("EMPTY");
        return;
    }

    Wire.write(this->node_outbox[0].c_str());
    this->node_outbox.erase(this->node_outbox.begin());
}

void NodeSlave::ApplyNodeLEDCmd() {
    if (this->led_cmd_ready) {
        apply_frame_command(this->led_cmd);
        led_cmd_ready = 0;
    }
}

void NodeSlave::SortMsgsFromNode() {

    for (int i = 0; i < this->node_inbox.size(); i++) {
        if (this->node_inbox[i].startsWith("LED")) {
            this->led_cmd = this->node_inbox[i];
            this->led_cmd_ready = 1;
        } else if (this->node_inbox[i].startsWith("LOC")) {
            this->network_outbox.push_back(this->node_inbox[i]);
        } else if (this->node_inbox[i].startsWith("COM")) {
            this->network_outbox.push_back(this->node_inbox[i]);
        } else {
            //Unsupported Msg Type
            continue;
        }
    }
    this->node_inbox.clear();
}

NodeSlave node_ctrlr = NodeSlave();

void receive_msgs_from_node(int numBytes) {
    // Serial.println("receive_msgs_from_node");
    (void)numBytes;
    node_ctrlr.GetIncommingNodeMsgs();
}

void send_msgs_to_node() {
    node_ctrlr.SendMsgsToNode();
}

void setup_i2c() {
    // Serial.println("setup_i2c");
    Wire.begin(SLAVE_ADDR);
    Wire.onReceive(receive_msgs_from_node);
    Wire.onRequest(send_msgs_to_node);
}

void run_node_slave() {
    if (node_ctrlr.i2c_buffer_ready) {
        node_ctrlr.node_inbox.push_back(node_ctrlr.i2c_buffer);
        node_ctrlr.i2c_buffer_ready = 0;
    }

    node_ctrlr.SortMsgsFromNode();
    node_ctrlr.GetIncommingNetworkMsgs();
    node_ctrlr.SendMsgsToNetwork();
    node_ctrlr.ApplyNodeLEDCmd();
}

#endif //NODE_SLAVE_HPP