#ifndef NODE_SLAVE
#define NODE_SLAVE

#include "networking/networking.hpp"
#include "../constants/directions.hpp"
#include "Arduino.h" 

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
    std::vector<String> led_cmds;            // QUEUE OF COMMANDS TO RUN ON THE LEDS
}

void NodeSlave::GetIncommingNetworkMsgs() {
    this->node_outbox.push_back(recv_msgs());
    return;
}

void NodeSlave::GetIncommingNodeMsgs() {
    //I2C Magic

    this->node_inbox.push_back();
}

void NodeSlave::SendMsgsToNetwork() {
    send_msgs(this->network_outbox);
}

void NodeSlave::SendMsgsToNode() {
    //I2C Magic
    this->node_outbox.clear();
}

void NodeSlave::ApplyNodeLEDCmd() {
    // SHOULD IT JUST BE THE LATEST COMMAND? 
}

void NodeSlave::SortMsgsFromNode() {
    for (int i = 0; i < this->node_inbox.size(); i++) {
        if (this->node_inbox[i].startsWith("LED")) {
            this->led_cmds.push_back(this->node_inbox[i]);
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