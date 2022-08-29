//
// Created by Dawid on 24.08.2022.
//

#include "Switch.h"

Switch::Switch(const String &id, uint8_t pin, const String &type, SX1509 *io) : IThings(id), pin(pin), type(type),
                                                                                io(io) {
    off();
}

void Switch::on() {
    if (type == "INPUT_OFF_LOW_ON") {
        outputLow();
    } else if (type == "LOW_ON") {
        outputLow();
    } else if (type == "HIGH_ON") {
        outputHigh();
    }
}

void Switch::off() {
    if (type == "INPUT_OFF_LOW_ON") {
        input();
    } else if (type == "LOW_ON") {
        outputHigh();
    } else if (type == "HIGH_ON") {
        outputLow();
    }
}


void Switch::outputHigh() {
    if (io != nullptr) {
        io->pinMode(pin, OUTPUT);
        io->writePin(pin, HIGH);
    } else {
        pinMode(pin, OUTPUT);
        digitalWrite(pin, HIGH);
    }
}

void Switch::input() {
    if (io != nullptr) {
        io->pinMode(pin, INPUT);
    } else {
        pinMode(pin, INPUT);
    }
}

void Switch::outputLow() {
    if (io != nullptr) {
        io->pinMode(pin, OUTPUT);
        io->writePin(pin, LOW);
    } else {
        pinMode(pin, OUTPUT);
        digitalWrite(pin, LOW);
    }
}
