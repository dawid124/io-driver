//
// Created by Dawid on 03.04.2022.
//

#include "Core1Devices.h"

void Core1Devices::printDevices() {
    int i = 0;
    Logger::printlnCore1("Initialized devices NEO: ");
    for (auto const& device: neo) {
        Logger::printlnCore1(String(i) + ": " + device->id);
        i++;
    }
}

NeoLight *Core1Devices::getNeoById(String id) {
    for (auto io: neo) {
        if (io->id == id) {
            return io;
        }
    }
    return nullptr;
}

void Core1Devices::process(Action *action) {
    NeoLight *neoItem = getNeoById(action->ioId);

    WS2812FX &fx = neoItem->ws2812fx;
//    if (action->brightness == 0) {
//        fx.stop();
//    } else {
        if (fx.getMode() != action->animationId) {
            fx.setMode(action->animationId);
        }

        if (action->animationId == 0 && action->staticSubMode > 0) {
            int stepTime = 30;
            if (action->stepTime > 0) {
                stepTime = action->stepTime;
            }
            int steps = (int) action->time / stepTime;
            int stepValue = (int) 255 / steps;
            int speed = action->time / steps;

            fx.setStaticSubMode(0, action->staticSubMode);
            fx.setStaticSubModeStepValue(0, stepValue);
            fx.setSpeed(speed);
            setNeoColor(action, fx, 1);
        } else {
            fx.setColor(1, 0, 0, 0, 0);
            setNeoColor(action, fx, 0);
            if (fx.getSpeed() != action->speed) {
                fx.setSpeed(action->speed);
            }
        }

        if (!fx.isRunning()) {
            fx.start();
        }
//    }
}

void Core1Devices::setNeoColor(const Action *action, WS2812FX &fx, uint8_t colorIndex) const {
    float percent = (float) action->brightness / (float) 255;
    uint8_t r = (int) ((float) action->color.r * percent);
    uint8_t g = (int) ((float) action->color.g * percent);
    uint8_t b = (int) ((float) action->color.b * percent);
    uint8_t w = (int) ((float) action->color.w * percent);
    fx.setColor(colorIndex, r, g, b, w);
}

void Core1Devices::test(Action *action) {
    for (auto *neoItem: neo) {
        neoItem->ws2812fx.setBrightness(255);
        neoItem->ws2812fx.setColor(0, 0, 0, 255);
        neoItem->ws2812fx.setMode(0);
        neoItem->ws2812fx.setSpeed(0);
        if (!neoItem->ws2812fx.isRunning()) {
            neoItem->ws2812fx.start();
        }
    }
}

void Core1Devices::picoLight() {
    if ((millis() > change_color_timeout) || (change_color_timeout == 0))
    {
        state = !state;
        digitalWrite(LED_BUILTIN, state);
        change_color_timeout = millis() + STATUS_LED_INTERVAL;
    }
}

Core1Devices::Core1Devices() {
    change_color_timeout = 0;
    state = false;
}

void Core1Devices::service() {
    picoLight();
    for (auto *neoItem: neo) {
        neoItem->service();
    }
}
