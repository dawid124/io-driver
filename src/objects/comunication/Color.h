//
// Created by Dawid on 06.03.2022.
//

#ifndef IO_DRIVER_COLOR_H
#define IO_DRIVER_COLOR_H

#include "Arduino.h"

class Color {


public:
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t w;

    Color(uint8_t r, uint8_t g, uint8_t b);

    Color(uint8_t r, uint8_t g, uint8_t b, uint8_t w);

    Color();

    void setBrightness(uint8_t v);
};


#endif //IO_DRIVER_COLOR_H
