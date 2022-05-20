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
    if (action->brightness == 0) {
        fx.stop();
    } else {
        if (fx.getBrightness() != action->brightness) {
            fx.setBrightness(action->brightness);
        }
        fx.setColor(action->color.r, action->color.g, action->color.b, action->color.w);
        if (fx.getMode() != action->animationId) {
            fx.setMode(action->animationId);
        }

        fx.setSpeed(action->speed);
        if (!fx.isRunning()) {
            fx.start();
        }
    }
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
