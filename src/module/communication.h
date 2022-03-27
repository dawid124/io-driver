//
// Created by Dawid on 26.02.2022.
//

#ifndef IO_DRIVER_COMMUNICATION_H
#define IO_DRIVER_COMMUNICATION_H

#include "./defines.h"


extern const char *ACTION_TOPIC;
extern mutex_t actionCore1MsgMutex;
extern std::vector<Action> actionCore0Msgs;
extern std::vector<Action> actionCore1Msgs;

void setupMqtt();

void reconnectMqtt();

void actionTopicCallback(char *topic, byte *payload, unsigned int length);

void mqttService();

void addToCore1Mutex(const Action& action);

void resetENC();

void publishSensor(const String& id, bool state);

void parseBytes(const char *str, char sep, byte *bytes, int maxBytes, int base);


#endif //IO_DRIVER_COMMUNICATION_H
