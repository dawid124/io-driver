//
// Created by Dawid on 01.03.2022.
//

#include <objects/iot/rgb/RgbLight.h>
#include <objects/iot/rgb/RgbwLight.h>
#include <objects/iot/neo/NeoLight.h>
#include "IotDevices.h"

void IotDevices::init() {
    initializeExpander(ioExpanderA, 0x3E);
    initializeExpander(ioExpanderB, 0x3F);
    initializeExpander(ioExpanderC, 0x70);
    initializeExpander(ioExpanderD, 0x71);
}

void IotDevices::initializeExpander(SX1509 &expander, byte address) {
    if (expander.begin(address)) {
        Serial.println("Successful initialize ioExpander on address: " + String(address));
    } else {
        Serial.println("Not initialize ioExpander on address: " + String(address));
    }
}

void IotDevices::setupDevices() {
    rgbw.push_back(new RgbwLight("rgbw-tv", ioExpanderA, 0, 1, 2, 3));
    rgbw.push_back(new RgbwLight("rgbw-dinner", ioExpanderA, 4, 5, 6, 7));
    rgbw.push_back(new RgbwLight("rgbw-office", ioExpanderA, 8, 9, 10, 11));
    rgbw.push_back(new RgbwLight("rgbw-kitchen", ioExpanderA, 12, 13, 14, 15));

    neo.push_back(new NeoLight("neo-kitchen", 15, 98, NEO_RGBW + NEO_KHZ800));
    neo.push_back(new NeoLight("neo-celling", 14, 220, NEO_RGBW + NEO_KHZ800));
    neo.push_back(new NeoLight("neo-test", 13, 300, NEO_RGBW + NEO_KHZ800));

    motionSensors.push_back(new MotionSensor("pir-office", 7));
    motionSensors.push_back(new MotionSensor("pir-lobby", 8));
    motionSensors.push_back(new MotionSensor("pir-wc", 9));
    motionSensors.push_back(new MotionSensor("pir-kitchen", 10));
}

void IotDevices::printDevices() {
    int i = 0;
    Serial.println("Initialized devices RGB: ");
    for (auto const& device: rgb) {
        Serial.println(String(i) + ": " + device->id);
        i++;
    }
    i = 0;
    Serial.println("Initialized devices RGBW: ");
    for (auto const& device: rgbw) {
        Serial.println(String(i) + ": " + device->id);
        i++;
    }
    i = 0;
    Serial.println("Initialized devices NEO: ");
    for (auto const& device: neo) {
        Serial.println(String(i) + ": " + device->id);
        i++;
    }
}

RgbLight* IotDevices::getRgbById(String id) {
    for (auto io: rgb) {
        if (io->id == id) {
            return io;
        }
    }
    return nullptr;
}

RgbwLight *IotDevices::getRgbwById(String id) {
    for (auto io: rgbw) {
        if (io->id == id) {
            return io;
        }
    }
    return nullptr;
}

NeoLight *IotDevices::getNeoById(String id) {
    for (auto io: neo) {
        if (io->id == id) {
            return io;
        }
    }
    return nullptr;
}
