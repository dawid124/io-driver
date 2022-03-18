//
// Created by Dawid on 01.03.2022.
//

#ifndef IO_DRIVER_IOTDEVICES_H
#define IO_DRIVER_IOTDEVICES_H

#include <Arduino.h>
#include <WS2812FX.h>
#include <SparkFunSX1509.h>
#include "Device.h"
#include "rgb/RgbLight.h"
#include "rgb/RgbwLight.h"
#include "neo/NeoLight.h"

class IotDevices {
public:
    std::vector<RgbLight*> rgb;
    std::vector<RgbwLight*> rgbw;
    std::vector<NeoLight*> neo;

    SX1509 ioExpanderA;
    SX1509 ioExpanderB;
    SX1509 ioExpanderC;
    SX1509 ioExpanderD;

    void init();

    void setupDevices();

    void initializeExpander(SX1509 &expander, byte address);

    void printDevices();

    RgbLight* getRgbById(String id);
    RgbwLight* getRgbwById(String id);
    NeoLight* getNeoById(String id);
};


#endif //IO_DRIVER_IOTDEVICES_H
