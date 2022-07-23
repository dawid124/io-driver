//
// Created by Dawid on 20.07.2022.
//

#include "Blind.h"

void Blind::up() {
    if (io != nullptr) {
        io->digitalWrite(upPin, LOW);
        io->digitalWrite(downPin, HIGH);
    } else {
        digitalWrite(upPin, LOW);
        digitalWrite(downPin, HIGH);
    }
}

void Blind::down() {
    if (io != nullptr) {
        io->digitalWrite(upPin, HIGH);
        io->digitalWrite(downPin, LOW);
    } else {
        digitalWrite(upPin, HIGH);
        digitalWrite(downPin, LOW);
    }
}

void Blind::clear() {
    if (io != nullptr) {
        io->digitalWrite(upPin, HIGH);
        io->digitalWrite(downPin, HIGH);
    } else {
        digitalWrite(upPin, HIGH);
        digitalWrite(downPin, HIGH);
    }
}

Blind::Blind(const String& id, uint8_t upPin, uint8_t downPin, SX1509 *io) : IThings(id), upPin(upPin), downPin(downPin), io(io) {
    if (io != nullptr) {
        io->pinMode(upPin, OUTPUT);
        io->digitalWrite(upPin, HIGH);
        io->pinMode(downPin, OUTPUT);
        io->digitalWrite(downPin, HIGH);
    } else {
        pinMode(upPin, OUTPUT);
        digitalWrite(upPin, HIGH);
        pinMode(downPin, OUTPUT);
        digitalWrite(downPin, HIGH);
    }
}
