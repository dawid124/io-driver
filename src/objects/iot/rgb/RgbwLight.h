//
// Created by Dawid on 01.03.2022.
//

#ifndef IO_DRIVER_RGBWLIGHT_H
#define IO_DRIVER_RGBWLIGHT_H

#include "RgbLight.h"

class RgbwLight: public RgbLight {
private:
    uint8_t pinW;
    uint8_t pinWw;
    bool rgbww;
public:
    int W;
    int Ww;
    uintptr_t task;
    RgbwLight(const String& id, SX1509 &io, uint8_t pinR, uint8_t pinG, uint8_t pinB, uint8_t pinW);
    RgbwLight(const String& id, SX1509 &io, uint8_t pinR, uint8_t pinG, uint8_t pinB, uint8_t pinW, uint8_t pinWw);
    void on(uint8_t r, uint8_t g, uint8_t b, uint8_t w, uint8_t ww);
    void color(uint8_t r, uint8_t g, uint8_t b, uint8_t w, uint8_t ww);
};


#endif //IO_DRIVER_RGBWLIGHT_H
