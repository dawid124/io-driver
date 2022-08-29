//
// Created by Dawid on 20.07.2022.
//

#include "Blind.h"

void Blind::up() {
    if (io != nullptr) {
        io->pinMode(upPin, OUTPUT);
        io->writePin(upPin, LOW);
        io->pinMode(downPin, INPUT);
    } else {
        pinMode(upPin, OUTPUT);
        digitalWrite(upPin, LOW);
        pinMode(downPin, INPUT);
    }
}

void Blind::down() {
    if (io != nullptr) {
        io->pinMode(downPin, OUTPUT);
        io->writePin(downPin, LOW);
        io->pinMode(upPin, INPUT);
    } else {
        pinMode(downPin, OUTPUT);
        digitalWrite(downPin, LOW);
        pinMode(upPin, INPUT);
    }
}

void Blind::clear() {
    if (io != nullptr) {
        io->pinMode(upPin, INPUT);
        io->pinMode(downPin, INPUT);
    } else {
        pinMode(upPin, INPUT);
        pinMode(downPin, INPUT);
    }
}

Blind::Blind(const String& id, uint8_t upPin, uint8_t downPin, SX1509 *io) : IThings(id), upPin(upPin), downPin(downPin), io(io) {

    if (io != nullptr) {
        io->pinMode(upPin, INPUT);
        io->pinMode(downPin, INPUT);
    } else {
        pinMode(upPin, INPUT);
        pinMode(downPin, INPUT);
    }
}
