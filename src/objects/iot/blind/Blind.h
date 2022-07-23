//
// Created by Dawid on 20.07.2022.
//

#ifndef IO_DRIVER_BLIND_H
#define IO_DRIVER_BLIND_H

#include <SparkFunSX1509.h>
#include <objects/iot/IThings.h>
#include "Arduino.h"

class Blind : public IThings {
public:

    uint8_t upPin;
    uint8_t downPin;

    uintptr_t task;

    void up();
    void down();
    void clear();

    Blind(const String& id, uint8_t upPin, uint8_t downPin, SX1509 *io);

protected:
    SX1509* io;
};


#endif //IO_DRIVER_BLIND_H
