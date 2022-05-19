//
// Created by Dawid on 03.04.2022.
//

#include "Core1Devices.h"

void Core1Devices::setupDevices() {
//    111
    neo.push_back(new NeoLight("neo-kitchen", 12, 98, NEO_RGBW + NEO_KHZ800, false));
    neo.push_back(new NeoLight("neo-celling", 13, 225, NEO_RGBW + NEO_KHZ800, false));
    neo.push_back(new NeoLight("neo-tv", 11, 133, NEO_RGBW + NEO_KHZ800, true));

//    2222
//    neo.push_back(new NeoLight("neo-wall-1", 15, 121, NEO_GRB + NEO_KHZ800));
//    neo.push_back(new NeoLight("neo-wall-2", 12, 121, NEO_GRB + NEO_KHZ800));
//    neo.push_back(new NeoLight("neo-wall-3", 10, 121, NEO_GRB + NEO_KHZ800));
//    neo.push_back(new NeoLight("neo-wall-4", 7, 121, NEO_GRB + NEO_KHZ800));
//    neo.push_back(new NeoLight("neo-wall-5", 14, 121, NEO_GRB + NEO_KHZ800));
    printDevices();
}

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
//
//    Logger::printlnCore1("process: " + String(neoItem->id));
//    Logger::printlnCore1("process: ");

    WS2812FX &fx = neoItem->ws2812fx;
    if (action->action == "ON") {
        fx.start();
    } else if (action->action == "OFF") {
        fx.stop();
    } else if (action->action == "CHANGE") {
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
