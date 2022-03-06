//
// Created by Dawid on 01.03.2022.
//

#ifndef IO_DRIVER_RGBLIGHT_H
#define IO_DRIVER_RGBLIGHT_H

#include <SparkFunSX1509.h>
#include <objects/iot/IThings.h>
#include "Arduino.h"

class RgbLight: public IThings {
private:

    uint8_t pinR;
    uint8_t pinG;
    uint8_t pinB;

    uint8_t R;
    uint8_t G;
    uint8_t B;
protected:
    SX1509 io;
public:


    RgbLight(const String& id, SX1509 &io, uint8_t pinR, uint8_t pinG, uint8_t pinB);

    void on(uint8_t r, uint8_t g, uint8_t b);
    void color(uint8_t r, uint8_t g, uint8_t b);
    void off();



};


#endif //IO_DRIVER_RGBLIGHT_H
