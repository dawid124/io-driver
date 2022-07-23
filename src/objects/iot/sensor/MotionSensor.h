//
// Created by Dawid on 27.03.2022.
//

#ifndef IO_DRIVER_MOTIONSENSOR_H
#define IO_DRIVER_MOTIONSENSOR_H

#include <objects/iot/IThings.h>
#include "Arduino.h"

enum MotionSensorStatus {
    NOT_CHANGED,
    ACTIVE,
    DE_ACTIVE
};

class MotionSensor: public IThings {
private:
    uint8_t pin;
    bool prevState;
    bool reverse;
public:
    bool currentState;
    MotionSensor(const String &id, uint8_t pin, bool reverse);

    MotionSensorStatus check();
};


#endif //IO_DRIVER_MOTIONSENSOR_H
