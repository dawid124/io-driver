//
// Created by Dawid on 27.03.2022.
//

#include "MotionSensor.h"

MotionSensor::MotionSensor(const String &id, uint8_t pin, bool reverse) : IThings(id), pin(pin), reverse(reverse) {
    this->currentState = false;
    this->prevState = false;
    if (reverse) {
        pinMode(pin, INPUT_PULLUP);
    } else {
        pinMode(pin, INPUT);
        digitalWrite(pin, LOW);
    }
}

MotionSensorStatus MotionSensor::check() {
    bool read;
    bool read2;
    if (reverse) {
        read = !digitalRead(this->pin);
        sleep_ms(1);
        read2 = !digitalRead(this->pin);
    } else {
        read = digitalRead(this->pin);
        sleep_ms(1);
        read2 = digitalRead(this->pin);
    }


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