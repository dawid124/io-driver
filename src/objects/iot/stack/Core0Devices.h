//
// Created by Dawid on 01.03.2022.
//

#ifndef IO_DRIVER_CORE0DEVICES_H
#define IO_DRIVER_CORE0DEVICES_H

#include <Arduino.h>
#include <SparkFunSX1509.h>
#include <objects/iot/sensor/MotionSensor.h>
#include <objects/comunication/Action.h>
#include <objects/change/RgbwChange.h>
#include "objects/iot/Device.h"
#include <arduino-timer.h>
#include "objects/iot/rgb/RgbLight.h"
#include "objects/iot/rgb/RgbwLight.h"

#define CHANGE_MILLIS 30

class Core0Devices {
public:
    std::vector<RgbLight*> rgb;
    std::vector<RgbwLight*> rgbw;
    std::vector<MotionSensor*> motionSensors;
    Timer<20, millis, RgbwChange*> timer;

    SX1509 ioExpanderA;
    SX1509 ioExpanderB;
    SX1509 ioExpanderC;
    SX1509 ioExpanderD;

    void init();

    void setupDevices();

    void initializeExpander(SX1509 &expander, byte address);

    void printDevices();

    void process(const Action &action);
    void service();

    RgbLight* getRgbById(String id);
    RgbwLight* getRgbwById(String id);

private:
    void processRgbwAction(const Action &action, RgbwLight& light);
    static bool rgbwChange(RgbwChange *change);
};


#endif //IO_DRIVER_CORE0DEVICES_H
