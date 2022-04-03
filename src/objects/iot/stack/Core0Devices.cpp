//
// Created by Dawid on 01.03.2022.
//

#include "Core0Devices.h"

void Core0Devices::init() {
    initializeExpander(ioExpanderA, 0x3E);
    initializeExpander(ioExpanderB, 0x3F);
    initializeExpander(ioExpanderC, 0x70);
    initializeExpander(ioExpanderD, 0x71);
}

void Core0Devices::initializeExpander(SX1509 &expander, byte address) {
    if (expander.begin(address)) {
        expander.clock(INTERNAL_CLOCK_2MHZ, 5); // 6, 5, 4
        Serial.println("Successful initialize ioExpander on address: " + String(address));
    } else {
        Serial.println("Not initialize ioExpander on address: " + String(address));
    }
}

void Core0Devices::setupDevices() {
    rgbw.push_back(new RgbwLight("rgbw-tv", ioExpanderA, 0, 1, 2, 3));
    rgbw.push_back(new RgbwLight("rgbw-dinner", ioExpanderA, 4, 5, 6, 7));
    rgbw.push_back(new RgbwLight("rgbw-office", ioExpanderA, 8, 9, 10, 11));
    rgbw.push_back(new RgbwLight("rgbw-kitchen", ioExpanderA, 12, 13, 14, 15));

    rgbw.push_back(new RgbwLight("rgbw-lobby", ioExpanderB, 8, 9, 10, 11));

    motionSensors.push_back(new MotionSensor("pir-office", 7));
    motionSensors.push_back(new MotionSensor("pir-lobby", 8));
    motionSensors.push_back(new MotionSensor("pir-wc", 9));
    motionSensors.push_back(new MotionSensor("pir-kitchen", 10));

    printDevices();
}

void Core0Devices::printDevices() {
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
}

RgbLight* Core0Devices::getRgbById(String id) {
    for (auto io: rgb) {
        if (io->id == id) {
            return io;
        }
    }
    return nullptr;
}

RgbwLight *Core0Devices::getRgbwById(String id) {
    for (auto io: rgbw) {
        if (io->id == id) {
            return io;
        }
    }
    return nullptr;
}

void Core0Devices::process(const Action &action) {
    if (action.ioType == "RGBW") {
        RgbwLight *light = getRgbwById(action.ioId);
        processRgbwAction(action, *light);
    } else if (action.ioType == "RGB") {

    } else if (action.ioType == "BLIND") {

    } else if (action.ioType == "HEATING") {

    }
}

void Core0Devices::processRgbwAction(const Action &action, RgbwLight &light) {

    if (light.task != (uintptr_t)(NULL)) {
        timer.cancel(light.task);
    }

    Color color = action.color;

    color.setBrightness(action.brightness);
    int stepTime = CHANGE_MILLIS;
    if (action.stepTime > 0) {
        stepTime = action.stepTime;
    }
    int steps = (int) action.time / stepTime;

    light.task = timer.every(stepTime, rgbwChange, new RgbwChange(color, steps, &light));
}

bool Core0Devices::rgbwChange(RgbwChange *change) {
    return change->nextStep();
}

void Core0Devices::service() {
    timer.tick();
}
