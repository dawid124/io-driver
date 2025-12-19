//
// Created by Dawid on 01.03.2022.
//

#ifndef IO_DRIVER_CORE0DEVICES_H
#define IO_DRIVER_CORE0DEVICES_H

#include <Arduino.h>
#include <vector>

#include <Wire.h>           // Include the I2C library (required)
#include <SparkFunSX1509.h>
#include <objects/iot/sensor/MotionSensor.h>
#include <objects/comunication/Action.h>
#include <objects/change/RgbwChange.h>
#include "objects/iot/Device.h"
#include <arduino-timer.h>
#include "objects/iot/rgb/RgbLight.h"
#include "objects/iot/rgb/RgbwLight.h"
#include "defines.h"
#include "module/logger.h"
#include <map>
#include <objects/iot/blind/Blind.h>
#include <objects/change/BlindClean.h>
#include <objects/iot/switch/Switch.h>
#include <objects/change/SwitchOff.h>

#define CHANGE_MILLIS 30

class Core0Devices {
public:
    std::vector<RgbLight*> rgb;
    std::vector<RgbwLight*> rgbw;
    std::vector<MotionSensor*> motionSensors;
    std::vector<Blind*> blinds;
    std::vector<Switch*> switches;
    Timer<32, millis, BlindClean*> blindTimer;
    Timer<16, millis, SwitchOff*> switchTimer;
    Timer<128, millis, RgbwChange*> timer;

//    TwoWire Wire;

    std::map<String,String> idTypeMap;

    SX1509 ioExpanderA;
    SX1509 ioExpanderB;
    SX1509 ioExpanderC;
    SX1509 ioExpanderD;

    void init();

    void initializeExpander(SX1509 &expander, byte address);

    void printDevices();

    void process(Action *action);
    void service();

    RgbLight* getRgbById(String id);
    RgbwLight* getRgbwById(String id);
    Blind* getBlindById(String id);
    Switch* getSwitchById(String id);

private:
    void processRgbwAction(Action *action, RgbwLight& light);
    void processBlindAction(Action *action, Blind& blind);
    void processSwitchAction(Action *action, Switch& switchh);

    static bool rgbwChange(RgbwChange *change);
    static bool blindClear(BlindClean *clean);
    static bool switchOff(SwitchOff *clean);
};


#endif //IO_DRIVER_CORE0DEVICES_H
