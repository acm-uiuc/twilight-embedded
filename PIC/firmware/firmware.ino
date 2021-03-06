#include "constants/constants.hpp"
#include "led_driver/led_driver.hpp"
#include "node_slave/node_slave.hpp"
#include "networking/networking.hpp"
void setup() {
    //Setup LED Thread
    setup_frame();

    //Setup Network Thread
    setup_networking();

    //DeviceI2C
    setup_i2c();

    // Serial.println("Starting");
    frame.SetColor(255,165,0);
    frame.UpdateFrame();
}

void loop() {
    run_node_slave();
    handle_network_msgs();
}



