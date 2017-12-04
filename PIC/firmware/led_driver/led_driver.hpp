#ifndef LED_DRIVER_HPP
#define LED_DRIVER_HPP

#include "Arduino.h"
#include <Adafruit_NeoPixel.h>
#include <Wire.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)

void colorWipe(uint32_t c, uint8_t wait) ;
uint32_t Wheel(Adafruit_NeoPixel, byte WheelPos);
void rainbowCycle(uint8_t wait);

typedef struct color_struct {
    uint8_t r;
    uint8_t g;
    uint8_t b;
} Color;

inline bool operator == (const Color& lhs, const Color& rhs) {
    return lhs.r == rhs.r && lhs.g == rhs.g && lhs.b == rhs.b;
}

class LEDFrame {
public:
    LEDFrame();
    void setup();
    void UpdateFrame();
    void SetColor(uint8_t r, uint8_t g, uint8_t b);
    void ApplyCommand(const char* cmd);
    Adafruit_NeoPixel GetStrip();
private:
    Adafruit_NeoPixel strip;
    Color color;
    Color lastColor;
};

LEDFrame::LEDFrame() {
   this->strip = Adafruit_NeoPixel(STRIP_LENGTH, LED_PIN, NEO_GRB + NEO_KHZ800);
   this->color = Color{255,255,255};
}

Adafruit_NeoPixel LEDFrame::GetStrip() {
    return this->strip;
}

void LEDFrame::setup() {
    this->strip.begin();
    this->strip.show();
}

void LEDFrame::UpdateFrame() {
    if (this->color == this->lastColor) {
        return;
    }

    for(uint16_t i=0; i < this->strip.numPixels(); i++) {
        strip.setPixelColor(i, this->strip.Color(this->color.r, this->color.b, this->color.g));
    }

    Wire.end();
    strip.show();
    Wire.begin();

    this->lastColor = this->color;
}

void LEDFrame::SetColor(uint8_t r, uint8_t g, uint8_t b) {
    this->color.r = r;
    this->color.g = g;
    this->color.b = b;
}

void LEDFrame::ApplyCommand(const char* cmd) {
    // Expects string of form "LED:255,255,255"

    // check length
    if (strlen(cmd) > 15) {
        SetColor(255, 0, 0);
        return;
    }

    // extract channels
    int colorTuple1, colorTuple2, colorTuple3;
    int n = sscanf(cmd, "LED:%d,%d,%d", &colorTuple1, &colorTuple2, &colorTuple3);

    // ensure all 3 channels were extracted
    if (n != 3) {
        SetColor(0, 0, 255);
        return;
    }

    // set color
    this->SetColor(colorTuple1, colorTuple2, colorTuple3);
}


LEDFrame frame = LEDFrame();

void setup_frame() {
    frame.setup();
}

// Thread for LED frame
void update_frame() {
    frame.UpdateFrame();
}

Adafruit_NeoPixel get_strip() {
    return frame.GetStrip();
}

void apply_frame_command(String cmd) {
    frame.ApplyCommand(cmd.c_str());
    frame.UpdateFrame();
}

#endif //LED_DRIVER_HPP
