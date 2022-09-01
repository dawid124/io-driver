//
// Created by Dawid on 01.03.2022.
//

#include "RgbLight.h"

RgbLight::RgbLight(const String& id, SX1509 *io, int pinR, int pinG, int pinB)
        : IThings(id), io(io), pinR(pinR), pinG(pinG), pinB(pinB) {
    R = 0;
    G = 0;
    B = 0;

    if (io != nullptr) {
        if (pinR>= 0) {
            io->pinMode(pinR, ANALOG_OUTPUT);
            io->analogWrite(pinR, 0);
        }
        if (pinG >= 0) {
            io->pinMode(pinG, ANALOG_OUTPUT);
            io->analogWrite(pinG, 0);
        }
        if (pinB >= 0) {
            io->pinMode(pinB, ANALOG_OUTPUT);
            io->analogWrite(pinB, 0);
        }
    } else {
        if (pinR >= 0) {
            pinMode(pinR, ANALOG_OUTPUT);
            analogWrite(pinR, 255);
        }
        if (pinG >= 0) {
            pinMode(pinG, ANALOG_OUTPUT);
            analogWrite(pinG, 255);
        }
        if (pinB >= 0) {
            pinMode(pinB, ANALOG_OUTPUT);
            analogWrite(pinB, 255);
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
