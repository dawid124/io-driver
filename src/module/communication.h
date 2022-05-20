//
// Created by Dawid on 26.02.2022.
//

#ifndef IO_DRIVER_COMMUNICATION_H
#define IO_DRIVER_COMMUNICATION_H

#include "./defines.h"
#include "./module/logger.h"
#include <malloc.h>
#include <objects/iot/stack/Core0Devices.h>
#include <objects/iot/stack/Core1Devices.h>
#include "msgQueue.h"
#include <stdio.h>

enum DriverState { READY_FOR_CONFIG, READY_FOR_INITIAL_STATE, WORKING };

extern mutex_t actionCore1MsgMutex;
extern Queue actionCore0Msgs;
extern Queue actionCore1Msgs;

extern Core0Devices core0Devices;
extern Core1Devices core1Devices;

extern DriverState driverState;

void setupMqtt();
void initDevices();

void reconnectMqtt();

void actionTopicCallback(char *topic, byte *payload, unsigned int length);

void mqttService();

void addToCore1Mutex(Action *action);

void resetENC();

void publishSensor(const String& id, bool state);

void parseBytes(const char *str, char sep, byte *bytes, int maxBytes, int base);

void processActions(const JsonArray &actions);

void processConfig(const JsonArray &actions);

#endif //IO_DRIVER_COMMUNICATION_H
