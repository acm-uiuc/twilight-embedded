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
    void ParseNodeMessageIfNeeded();

    uint8_t i2c_buffer_pos = 0;
    uint8_t i2c_buffer_ready = 0;
    char i2c_buffer[BUFSIZE];

private:

    std::vector<String> node_outbox;         // QUEUE OF MESSAGES TO GO TO THE NODE
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

void NodeSlave::SendMsgsToNode() {
    //I2C Magic
    if (this->node_outbox.size() == 0) {
        Wire.write("EMPTY");
        return;
    }

    Wire.write(this->node_outbox[0].c_str());
    this->node_outbox.erase(this->node_outbox.begin());
}

NodeSlave node_ctrlr = NodeSlave();

void receive_msgs_from_node(int numBytes) {
    (void)numBytes;
    node_ctrlr.GetIncommingNodeMsgs();
}

void send_msgs_to_node() {
    node_ctrlr.SendMsgsToNode();
}

void setup_i2c() {
    Wire.begin(SLAVE_ADDR);
    Wire.onReceive(receive_msgs_from_node);
    Wire.onRequest(send_msgs_to_node);
}


void NodeSlave::ParseNodeMessageIfNeeded() {
    if (!node_ctrlr.i2c_buffer_ready) {
        return;
    }

    node_ctrlr.i2c_buffer_ready = 0;

    String msg = node_ctrlr.i2c_buffer;

    if (msg.startsWith("LED")) {
        apply_frame_command(msg);
    } else if (msg.startsWith("LOC")) {
        multicast(msg);
    } else if (msg.startsWith("COM")) {
        multicast(msg);
    } else {
        //Unsupported Msg Type
        return;
    }
}

void run_node_slave() {
    node_ctrlr.GetIncommingNetworkMsgs();
    node_ctrlr.ParseNodeMessageIfNeeded();
}

#endif //NODE_SLAVE_HPP