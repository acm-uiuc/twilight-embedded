#ifndef LED_DRIVER_HPP
#define LED_DRIVER_HPP

#include "Arduino.h"
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

#define LED_PIN 9
#define STRIP_LENGTH 35

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
    void ApplyCommand(String cmd);
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
        strip.show();
    }

    this->lastColor = this->color;
}

void LEDFrame::SetColor(uint8_t r, uint8_t g, uint8_t b) {
    this->color.r = r;
    this->color.g = g;
    this->color.b = b;
}

void LEDFrame::ApplyCommand(String cmd) {
    // Expects string of form "LED:255,255,255"

    // check length
    if (cmd.length() <= 15) {
        return;
    }

    // copy into a c string
    char cmdBuf[16];
    cmd.toCharArray(cmdBuf, 16);

    // extract channels
    int colorTuple1, colorTuple2, colorTuple3;
    int n = sscanf(cmdBuf, "LED:%d,%d,%d", &colorTuple1, &colorTuple2, &colorTuple3);

    // ensure all 3 channels were extracted
    if (n != 3) {
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
    frame.ApplyCommand(cmd);
}

//---------------------------- STANDARD ANIMATIONS -------------------------------//

// Fill the dots one after the other with a color
void colorWipe(Adafruit_NeoPixel strip, uint32_t c, uint8_t wait) {
    for(uint16_t i=0; i<strip.numPixels(); i++) {
        strip.setPixelColor(i, c);
        strip.show();
        delay(wait);
    }
}

void rainbow(Adafruit_NeoPixel strip, uint8_t wait) {
    uint16_t i, j;
    for(j=0; j<256; j++) {
        for(i=0; i<strip.numPixels(); i++) {
            strip.setPixelColor(i, Wheel(strip, (i+j) & 255));
        }
        strip.show();
        delay(wait);
    }
}

// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(Adafruit_NeoPixel strip, uint8_t wait) {
    uint16_t i, j;

    for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
        for(i=0; i< strip.numPixels(); i++) {
            strip.setPixelColor(i, Wheel(strip, ((i * 256 / strip.numPixels()) + j) & 255));
        }
        strip.show();
        delay(wait);
      }
  }

//Theatre-style crawling lights.
void theaterChase(Adafruit_NeoPixel strip, uint32_t c, uint8_t wait) {
    for (int j=0; j<10; j++) {  //do 10 cycles of chasing
        for (int q=0; q < 3; q++) {
            for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
                strip.setPixelColor(i+q, c);    //turn every third pixel on
            }
            strip.show();
            delay(wait);
            for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
                strip.setPixelColor(i+q, 0);        //turn every third pixel off
            }
        }
    }
}

//Theatre-style crawling lights with rainbow effect
void theaterChaseRainbow(Adafruit_NeoPixel strip, uint8_t wait) {
    for (int j=0; j < 256; j++) {     // cycle all 256 colors in the wheel
        for (int q=0; q < 3; q++) {
            for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
                strip.setPixelColor(i+q, Wheel(strip, (i+j) % 255));    //turn every third pixel on
            }
            strip.show();

            delay(wait);

            for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
                strip.setPixelColor(i+q, 0);        //turn every third pixel off
            }
        }
    }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(Adafruit_NeoPixel strip, byte WheelPos) {
    WheelPos = 255 - WheelPos;
    if(WheelPos < 85) {
        return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
    }
    if(WheelPos < 170) {
        WheelPos -= 85;
        return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
    }
    WheelPos -= 170;
    return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}

#endif //LED_DRIVER_HPP
