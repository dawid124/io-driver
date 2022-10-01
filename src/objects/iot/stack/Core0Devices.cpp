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
    Logger::printlnCore0("Initialized Blinds: ");
    for (auto const& device: blinds) {
        Logger::printlnCore0(String(i) + ": " + device->id);
        i++;
    }
    Logger::printlnCore0("Initialized Switches: ");
    for (auto const& device: switches) {
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

Blind *Core0Devices::getBlindById(String id) {
    for (auto io: blinds) {
        if (io->id == id) {
            return io;
        }
    }
    return nullptr;
}

Switch *Core0Devices::getSwitchById(String id) {
    for (auto io: switches) {
        if (io->id == id) {
            return io;
        }
    }
    return nullptr;
}

void Core0Devices::process(Action *action) {
    String ioType = this->idTypeMap[action->ioId];
    if (ioType == "RGBW") {
        RgbwLight *light = getRgbwById(action->ioId);
        Logger::printlnCore0("process: " + String(light->id));
        processRgbwAction(action, *light);
    } else if (ioType == "BLIND") {
        Blind *blind = getBlindById(action->ioId);
        Logger::printlnCore0("process: " + String(blind->id));
        processBlindAction(action, *blind);
    } else if (ioType == "SWITCH") {
        Switch *io = getSwitchById(action->ioId);
        Logger::printlnCore0("process: " + String(io->id));
        processSwitchAction(action, *io);
    } else if (ioType == "HEATING") {

    }
}

void Core0Devices::processRgbwAction(Action *action, RgbwLight &light) {

    if (light.task != (uintptr_t)(NULL)) {
        timer.cancel(light.task);
    }

    Color color = action->color;;
    color.setBrightness(action->brightness);

//    light.color(color.r,color.g,color.b,color.w,color.ww);
    int stepTime = CHANGE_MILLIS;
    if (action->stepTime > 0) {
        stepTime = action->stepTime;
    }
    int steps = (int) action->time / stepTime;

    if (steps <= 1) {
//        light.color(color.r, color.g, color.b, color.w, color.ww);
        RgbwChange(color, steps, &light, 0).nextStep();
        return;
    }

    auto *rgbwChangePtr = new RgbwChange(color, steps, &light, action->delay);
    light.task = timer.every(stepTime, rgbwChange, rgbwChangePtr);
}

void Core0Devices::processBlindAction(Action *action, Blind &blind) {

    if (blind.task != (uintptr_t)(NULL)) {
        blindTimer.cancel(blind.task);
    }

    auto *blindCleanPtr = new BlindClean(&blind);
    if (action->action == "UP") {
        blind.up();
        blind.task = blindTimer.in(action->time, blindClear, blindCleanPtr);
//        blind.task = blindTimer.in(action->time, blindClear, &blind);
    } else if (action->action == "DOWN") {
        blind.down();
        blind.task = blindTimer.in(action->time, blindClear, blindCleanPtr);
//        blind.task = blindTimer.in(action->time, blindClear, &blind);
    }
}

void Core0Devices::processSwitchAction(Action *action, Switch &switchh) {
    if (action->action == "ON") {
        switchh.on();
    } else if (action->action == "OFF") {
        switchh.off();
    } else if (action->action == "TEMP_ON") {
        auto *switchOffPtr = new SwitchOff(&switchh);
        switchh.on();
        switchTimer.in(action->time, switchOff, switchOffPtr);
    }
}

bool Core0Devices::rgbwChange(RgbwChange *change) {
    if (change->delay > 0) {
        if (millis() < change->delay) {
            return true;
        } else {
            change->delay = 0;
        }
    }
    bool hasNext = change->nextStep();
    return hasNext;
}

bool Core0Devices::blindClear(BlindClean *clean) {
    return clean->clean();
}

bool Core0Devices::switchOff(SwitchOff *switchh) {
    return switchh->off();
}

void Core0Devices::service() {
    blindTimer.tick();
    switchTimer.tick();
    timer.tick();
}
