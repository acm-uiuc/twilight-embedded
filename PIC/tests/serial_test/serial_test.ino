#include <ThreadController.h>
#include <StaticThreadController.h>
#include <Thread.h>
#include "networking/networking.hpp"
#include "led_driver/led_driver.hpp"


Thread LEDs = Thread();
Thread Network = Thread();
Thread DeviceI2C = Thread();
ThreadController ctrlr = ThreadController();


void setup() {

    //Setup LED Thread
    setup_frame();
    LEDs.enabled = true;
    LEDs.setInterval(10);
    LEDs.onRun(update_frame);

    //Setup Network Thread
    setup_networking();
    Network.enabled = true;
    Network.setInterval(10);
    Network.onRun(handle_network_msgs);

    ctrlr.add(&LEDs);
    ctrlr.add(&Network);
    ctrlr.add(&DeviceI2C);

    frame.SetColor(0,255,0);
    Serial.begin(BAUD_RATE);
    Serial.println("STARTING");

}

void loop() {
    ctrlr.run();
    send_msg("BLUE");
    std::vector<String> incomming = recv_msgs();
    for (int i = 0; i < incomming.size(); i++) {
        Serial.println(incomming[i]);
        if (incomming[i].startsWith(String("BLUE"))) {
            frame.SetColor(0,0,255);
        }
    }
}





