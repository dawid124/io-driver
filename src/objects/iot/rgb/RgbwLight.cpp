//
// Created by Dawid on 01.03.2022.
//

#include "RgbwLight.h"

RgbwLight::RgbwLight(const String& id, SX1509* io, int pinR, int pinG, int pinB, int pinW)
        : RgbLight(id, io, pinR, pinG, pinB), pinW(pinW) {
    W = 0;
    Ww = 0;
    pinWw = -1;

    if (io != nullptr) {
        if (pinW >= 0) {
            io->analogWrite(pinW, 0);
            io->pinMode(pinW, ANALOG_OUTPUT);
        }
    } else {
        if (pinW >= 0) {
            analogWrite(pinW, 0);
            pinMode(pinW, ANALOG_OUTPUT);
        }
    }
}

RgbwLight::RgbwLight(const String &id, SX1509 *io, int pinR, int pinG, int pinB, int pinW,
                     int pinWw): RgbLight(id, io, pinR, pinG, pinB), pinW(pinW), pinWw(pinWw)  {
    W = 0;
    Ww = 0;
    if (io != nullptr) {
        if (pinW >= 0) {
            io->pinMode(pinW, ANALOG_OUTPUT);
            io->analogWrite(pinW, 0);
        }
        if (pinWw >= 0) {
            io->pinMode(pinWw, ANALOG_OUTPUT);
            io->analogWrite(pinWw, 0);
        }
    } else {
        if (pinW >= 0) {
            pinMode(pinW, ANALOG_OUTPUT);
            analogWrite(pinW, 255);
        }
        if (pinWw >= 0) {
            io->pinMode(pinWw, ANALOG_OUTPUT);
            io->analogWrite(pinWw, 255);
        }
    }
}

void RgbwLight::on(uint8_t r, uint8_t g, uint8_t b, uint8_t w, uint8_t ww) {
    color(r, g, b, w, ww);
}

void RgbwLight::color(uint8_t r, uint8_t g, uint8_t b, uint8_t w, uint8_t ww) {
    RgbLight::color(r, g, b);

    W = w;
    Ww = ww;

    if (io != nullptr) {
        if (pinW >= 0) {
            io->analogWrite(pinW, W);
        }
        if (pinWw >= 0) {
            io->analogWrite(pinWw, Ww);
        }
    } else {
        if (pinW >= 0) {
            analogWrite(pinW, 255 - W);
        }
        if (pinWw >= 0) {
            analogWrite(pinWw, 255 - Ww);
        }
    }
}
