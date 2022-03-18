//
// Created by Dawid on 01.03.2022.
//

#include "RgbwLight.h"

RgbwLight::RgbwLight(const String& id, SX1509 &io, uint8_t pinR, uint8_t pinG, uint8_t pinB, uint8_t pinW)
        : RgbLight(id, io, pinR, pinG, pinB), pinW(pinW) {
    W = 0;

    io.analogWrite(pinW, 0);
    io.pinMode(pinW, ANALOG_OUTPUT);
}

void RgbwLight::on(uint8_t r, uint8_t g, uint8_t b, uint8_t w) {
    color(r, g, b, w);
}

void RgbwLight::color(uint8_t r, uint8_t g, uint8_t b, uint8_t w) {
    RgbLight::color(r, g, b);

    W = w;
    io.analogWrite(pinW, w);
}
