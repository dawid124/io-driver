//
// Created by Dawid on 01.03.2022.
//

#include "RgbLight.h"

RgbLight::RgbLight(const String& id, SX1509 &io, uint8_t pinR, uint8_t pinG, uint8_t pinB)
        : IThings(id), io(io), pinR(pinR), pinG(pinG), pinB(pinB) {
    R = 0;
    G = 0;
    B = 0;

    io.analogWrite(pinR, 0);
    io.analogWrite(pinG, 0);
    io.analogWrite(pinB, 0);

    io.pinMode(pinR, ANALOG_OUTPUT);
    io.pinMode(pinG, ANALOG_OUTPUT);
    io.pinMode(pinB, ANALOG_OUTPUT);
}

void RgbLight::on(uint8_t r, uint8_t g, uint8_t b) {
    color(r, g, b);
}

void RgbLight::color(uint8_t r, uint8_t g, uint8_t b) {
    R = r;
    G = g;
    B = b;

    io.analogWrite(pinR, R);
    io.analogWrite(pinG, G);
    io.analogWrite(pinB, B);
}

void RgbLight::off() {
    color(0, 0, 0);
}
