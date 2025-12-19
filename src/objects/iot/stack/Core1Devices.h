//
// Created by Dawid on 03.04.2022.
//

#ifndef IO_DRIVER_CORE1DEVICES_H
#define IO_DRIVER_CORE1DEVICES_H

#include <Arduino.h>
#include <vector>
#include <WS2812FX.h>
#include <objects/comunication/Action.h>
#include "objects/iot/neo/NeoLight.h"
#include "module/logger.h"
#include <map>

#define STATUS_LED_INTERVAL   2000

class Core1Devices {
private:
    unsigned long change_color_timeout;
    bool state;
public:
    std::map<String,String> idTypeMap;

    Core1Devices();

    std::vector<NeoLight*> neo;

    void printDevices();

    void service();

    void process(Action *action);
    void test(Action *action);

    void picoLight();

    NeoLight* getNeoById(String id);

    void setNeoColor(const Action *action, WS2812FX &fx, uint8_t colorIndex) const;
};


#endif //IO_DRIVER_CORE1DEVICES_H
