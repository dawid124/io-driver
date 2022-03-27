//
// Created by Dawid on 27.03.2022.
//

#include "MotionSensor.h"

MotionSensor::MotionSensor(const String &id, uint8_t pin) : IThings(id), pin(pin) {
    this->currentState = false;
    this->prevState = false;
    pinMode(pin, INPUT);
    digitalWrite(pin, LOW);
}

MotionSensorStatus MotionSensor::check() {
    PinStatus read = digitalRead(this->pin);
    sleep_ms(1);
    PinStatus read2 = digitalRead(this->pin);

    if (read != read2) {
        return NOT_CHANGED;
    }
    this->currentState = read;

    if (this->currentState != this->prevState) {
        this->prevState = this->currentState;
        if (this->currentState) {
            return ACTIVE;
        } else {
            return DE_ACTIVE;
        }
    }

    return NOT_CHANGED;
}