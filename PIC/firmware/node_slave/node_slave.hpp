#ifndef NODE_SLAVE
#define NODE_SLAVE

#include "../networking/networking.hpp"
#include "../constants/directions.hpp"
#include "Arduino.h" 
#include <Wire.h>

///////////////////////// MAIN CTRLR THREAD //////////////////////////
// The Node Slave instance controls execution of operations on the 
// controller. Assigns the box color and sends messages. 
//////////////////////////////////////////////////////////////////////

void run_node_slave();

class NodeSlave {
public:
    void GetIncommingNetworkMsgs();
    void GetIncommingNodeMsgs();
    void SendMsgsToNode();
    void SendMsgsToNetwork();
    void ApplyNodeLEDCmd();
    void SortMsgsFromNode();
    
private:
    std::vector<String> node_inbox;          // QUEUE OF MESSAGES FROM THE NODE
    std::vector<String> node_outbox;         // QUEUE OF MESSAGES TO GO TO THE NODE
    std::vector<String> network_outbox;      // QUEUE OF MESSAGES TO GO OUT TO THE NETWORK 
    String led_cmd;                         // QUEUE OF COMMANDS TO RUN ON THE LEDS (RIGHT NOW ITS JUST A SINGLE CMD)
}

int count = 0;

//Get incomming messages from the Network
void NodeSlave::GetIncommingNetworkMsgs() {
    this->node_outbox.push_back(recv_msgs());
    return;
}

void NodeSlave::GetIncommingNodeMsgs() {
    //I2C Magic
    char buf[32];
    while(Wire.available()){
        if(count < 32){
            byteArray[count] = Wire.read();
            count++;
        } else {
            count = 0;
            byteArray[count] = Wire.read();
        }
    }     
    count = 0;
    this->node_inbox.push_back(String(buf));
}

void NodeSlave::SendMsgsToNetwork() {
    send_msgs(this->network_outbox);
}

void NodeSlave::SendMsgsToNode() {
    //I2C Magic
    for (int i = 0; i < this->node_outbox.size(); i++) {
        Wire.write(this->node_outbox[i].size());
        Wire.write(this->node_outbox[i])
    }
    this->node_outbox.clear(led_cmd);
}

void NodeSlave::ApplyNodeLEDCmd() {
    // SHOULD IT JUST BE THE LATEST COMMAND?
    apply_frame_command()
}

void NodeSlave::SortMsgsFromNode() {
    for (int i = 0; i < this->node_inbox.size(); i++) {
        if (this->node_inbox[i].startsWith("LED")) {
            this->led_cmd = this->node_inbox[i];
        } else if (this->node_inbox[i].startsWith("LOC")) {
            this->network_outbox.push_back(this->node_inbox[i]);
        } else if (this->node_inbox[i].startsWith("DATA")) {
            this->network_outbox.push_back(this->node_inbox[i]);
        } else {
            //Unsupported Msg Type 
            continue;
        }
    }
    this->node_inbox.clear();
}

NodeSlave node_ctrlr = NodeSlave();

void run_node_slave() {
    node_ctrlr.GetIncommingNodeMsgs();
    node_ctrlr.SortMsgsFromNode();
    node_ctrlr.GetIncommingNetworkMsgs();
    node_ctrlr.SendMsgsToNetwork();
    node_ctrlr.ApplyNodeLEDCmd();
    node_ctrlr.SendMsgsToNode();
}

#endif //NODE_SLAVE