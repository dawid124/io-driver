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
        Logger::printlnCore0("Successful initialize ioExpander on address: " + String(address));
    } else {
        Logger::printlnCore0("Not initialize ioExpander on address: " + String(address));
    }
}

void Core0Devices::printDevices() {
    int i = 0;
    Logger::printlnCore0("Initialized devices RGB: ");
    for (auto const& device: rgb) {
        Logger::printlnCore0(String(i) + ": " + device->id);
        i++;
    }
    i = 0;
    Logger::printlnCore0("Initialized devices RGBW: ");
    for (auto const& device: rgbw) {
        Logger::printlnCore0(String(i) + ": " + device->id);
        i++;
    }
    Logger::printlnCore0("Initialized Motion Sensors: ");
    for (auto const& device: motionSensors) {
        Logger::printlnCore0(String(i) + ": " + device->id);
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


void Core0Devices::process(Action *action) {
    if (action->ioType == "RGBW") {
        RgbwLight *light = getRgbwById(action->ioId);
        Logger::printlnCore0("process: " + String(light->id));
        processRgbwAction(action, *light);
    } else if (action->ioType == "RGB") {

    } else if (action->ioType == "BLIND") {

    } else if (action->ioType == "HEATING") {

    }
}

void Core0Devices::processRgbwAction(Action *action, RgbwLight &light) {

    if (light.task != (uintptr_t)(NULL)) {
        timer.cancel(light.task);
    }

    Color color = action->color;

    color.setBrightness(action->brightness);
    int stepTime = CHANGE_MILLIS;
    if (action->stepTime > 0) {
        stepTime = action->stepTime;
    }
    int steps = (int) action->time / stepTime;

    if (steps <= 1) {
        RgbwChange(color, steps, &light).nextStep();
        return;
    }

    auto *rgbwChangePtr = new RgbwChange(color, steps, &light);
    light.task = timer.every(stepTime, rgbwChange, rgbwChangePtr);
}

bool Core0Devices::rgbwChange(RgbwChange *change) {
    bool hasNext = change->nextStep();
    return hasNext;
}

void Core0Devices::service() {
    timer.tick();
}
