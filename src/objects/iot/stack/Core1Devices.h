//
// Created by Dawid on 03.04.2022.
//

#ifndef IO_DRIVER_CORE1DEVICES_H
#define IO_DRIVER_CORE1DEVICES_H

#include <Arduino.h>
#include <WS2812FX.h>
#include <objects/comunication/Action.h>
#include "objects/iot/neo/NeoLight.h"
#include "module/logger.h"

#define STATUS_LED_INTERVAL   2000

class Core1Devices {
private:
    unsigned long change_color_timeout;
    bool state;
public:
    Core1Devices();

    std::vector<NeoLight*> neo;

    void setupDevices();

    void printDevices();

    void service();

    void process(const Action &action);

    void picoLight();

    NeoLight* getNeoById(String id);
};


#endif //IO_DRIVER_CORE1DEVICES_H
