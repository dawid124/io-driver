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

    int pinR;
    int pinG;
    int pinB;
protected:
    SX1509* io;
public:
    int R;
    int G;
    int B;

    RgbLight(const String& id, SX1509 *io, int pinR, int pinG, int pinB);

    void on(uint8_t r, uint8_t g, uint8_t b);
    void color(uint8_t r, uint8_t g, uint8_t b);
    void off();



};


#endif //IO_DRIVER_RGBLIGHT_H
