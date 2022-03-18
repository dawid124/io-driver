//
// Created by Dawid on 01.03.2022.
//

#ifndef IO_DRIVER_RGBWLIGHT_H
#define IO_DRIVER_RGBWLIGHT_H


#include "RgbLight.h"

class RgbwLight: public RgbLight {
private:
    uint8_t pinW;
public:
    int W;
    RgbwLight(const String& id, SX1509 &io, uint8_t pinR, uint8_t pinG, uint8_t pinB, uint8_t pinW);
    void on(uint8_t r, uint8_t g, uint8_t b, uint8_t w);
    void color(uint8_t r, uint8_t g, uint8_t b, uint8_t w);
};


#endif //IO_DRIVER_RGBWLIGHT_H
