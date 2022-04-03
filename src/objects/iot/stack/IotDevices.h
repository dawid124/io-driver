//
// Created by Dawid on 01.03.2022.
//

#ifndef IO_DRIVER_IOTDEVICES_H
#define IO_DRIVER_IOTDEVICES_H

#include <Arduino.h>
#include <WS2812FX.h>
#include <SparkFunSX1509.h>
#include <objects/iot/sensor/MotionSensor.h>
#include "objects/iot/Device.h"
#include "objects/iot/rgb/RgbLight.h"
#include "objects/iot/rgb/RgbwLight.h"
#include "objects/iot/neo/NeoLight.h"

class IotDevices {
public:
    std::vector<RgbLight*> rgb;
    std::vector<RgbwLight*> rgbw;
    std::vector<NeoLight*> neo;
    std::vector<MotionSensor*> motionSensors;

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
