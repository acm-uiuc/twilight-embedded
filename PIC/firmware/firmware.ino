#include <ThreadController.h>
#include <StaticThreadController.h>
#include <Thread.h>
#include "networking/networking.hpp"
#include "led_driver/led_driver.hpp"
#include "node_slave/node_slave.hpp"

Thread LEDThread = Thread();
Thread NetworkThread = Thread();
Thread NodeSlaveThread = Thread();
ThreadController ThreadCtrlr = ThreadController();

void setup() {
    
    //Setup LED Thread 
    setup_frame();
    LEDThread.enabled = true;
    LEDThread.setInterval(10);
    LEDThread.onRun(update_frame);

    //Setup Network Thread
    setup_networking();
    NetworkThread.enabled = true;
    NetworkThread.setInterval(10);
    NetworkThread.onRun(handle_network_msgs);
    
    //DeviceI2C
    setup_i2c();
    NodeSlaveThread.enabled = true;
    NodeSlaveThread.setInterval(10);
    NodeSlaveThread.onRun(run_node_slave);

    ThreadCtrlr.add(&LEDThread);
    ThreadCtrlr.add(&NetworkThread);
    ThreadCtrlr.add(&NodeSlaveThread);

    uint16_t num = 10000;
    rainbow(get_strip(), num); //Boot Complete
}

void loop() {
    ThreadCtrlr.run();
}






