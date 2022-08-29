//
// Created by Dawid on 24.08.2022.
//

#ifndef IO_DRIVER_SWITCH_H
#define IO_DRIVER_SWITCH_H

#include "Arduino.h"
#include <SparkFunSX1509.h>
#include <objects/iot/IThings.h>

class Switch : public IThings {
public:
    uint8_t pin;
    String type;

    void on();
    void off();

    Switch(const String &id, uint8_t pin, const String &type, SX1509 *io);

protected:
    SX1509* io;

    void outputLow();

    void input();

    void outputHigh();
};


#endif //IO_DRIVER_SWITCH_H
