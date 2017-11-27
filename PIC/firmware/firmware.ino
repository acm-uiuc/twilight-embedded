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
    NodeSlaveThread.enable = true;
    NodeSlaveThread.setInterval(10);
    NodeSlaveThread.onRun(handle_i2c_msgs);

    ThreadCtrlr.add(&LEDs);
    ThreadCtrlr.add(&Network);
    ThreadCtrlr.add(&DeviceI2C);

    rainbow(get_strip(), 10000) //Boot Complete
    Serial.begin(9600); //DEBUGGING

}

void loop() {
    ThreadCtrlr.run();
}




