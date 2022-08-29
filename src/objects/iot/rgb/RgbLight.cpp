//
// Created by Dawid on 01.03.2022.
//

#include "RgbLight.h"

RgbLight::RgbLight(const String& id, SX1509 *io, uint8_t pinR, uint8_t pinG, uint8_t pinB)
        : IThings(id), io(io), pinR(pinR), pinG(pinG), pinB(pinB) {
    R = 0;
    G = 0;
    B = 0;

    if (io != nullptr) {
        if (pinR>= 0) {
            io->analogWrite(pinR, 0);
            io->pinMode(pinR, ANALOG_OUTPUT);
        }
        if (pinG >= 0) {
            io->analogWrite(pinG, 0);
            io->pinMode(pinG, ANALOG_OUTPUT);
        }
        if (pinB >= 0) {
            io->analogWrite(pinB, 0);
            io->pinMode(pinB, ANALOG_OUTPUT);
        }
    } else {
        if (pinR >= 0) {
            analogWrite(pinR, 255);
            pinMode(pinR, ANALOG_OUTPUT);
        }
        if (pinG >= 0) {
            analogWrite(pinG, 255);
            pinMode(pinG, ANALOG_OUTPUT);
        }
        if (pinB >= 0) {
            analogWrite(pinB, 255);
            pinMode(pinB, ANALOG_OUTPUT);
        }
    }
}

void RgbLight::on(uint8_t r, uint8_t g, uint8_t b) {
    color(r, g, b);
}

void RgbLight::color(uint8_t r, uint8_t g, uint8_t b) {
    R = r;
    G = g;
    B = b;

    if (io != nullptr) {
        if (pinR >= 0) {
            io->analogWrite(pinR, R);
        }
        if (pinG >= 0) {
            io->analogWrite(pinG, G);
        }
        if (pinB >= 0) {
            io->analogWrite(pinB, B);
        }
    } else {
        if (pinR >= 0) {
            analogWrite(pinR, 255 - R);
        }
        if (pinG >= 0) {
            analogWrite(pinG, 255 - G);
        }
        if (pinB >= 0) {
            analogWrite(pinB, 255 - B);
        }
    }
}

void RgbLight::off() {
    color(0, 0, 0);
}
