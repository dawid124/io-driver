//
// Created by Dawid on 06.03.2022.
//

#include "Color.h"

Color::Color(uint8_t r, uint8_t g, uint8_t b) : r(r), g(g), b(b) {
    w = 0;
}

Color::Color(uint8_t r, uint8_t g, uint8_t b, uint8_t w) : r(r), g(g), b(b), w(w) {}


Color::Color(uint8_t r, uint8_t g, uint8_t b, uint8_t w, uint8_t ww) : r(r), g(g), b(b), w(w), ww(ww) {}

Color::Color() {

}

void Color::setBrightness(uint8_t v) {
    float percent = (float) v / (float) 255;
    r = (int) ((float) r * percent);
    Serial.println("newR: " + String(r));
    g = (int) ((float) g * percent);
    Serial.println("newG: " + String(g));
    b = (int) ((float) b * percent);
    Serial.println("newB: " + String(b));
    w = (int) ((float) w * percent);
    Serial.println("newW: " + String(w));
    ww = (int) ((float) ww * percent);
    Serial.println("newWw: " + String(ww));
}

