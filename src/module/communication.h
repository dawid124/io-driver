//
// Created by Dawid on 26.02.2022.
//

#ifndef IO_DRIVER_COMMUNICATION_H
#define IO_DRIVER_COMMUNICATION_H
#include <Arduino.h>
#include "./defines.h"
#include <PubSubClient.h>
#include <aJSON.h>


    extern const char* mqttServer;
    extern const IPAddress IP;
    extern const IPAddress DNS;
    extern byte MAC[];

    void setupMqtt();

    void reconnectMqtt();

    void actionTopicCallback(char* topic, byte* payload, unsigned int length);

    void mqttService();


#endif //IO_DRIVER_COMMUNICATION_H
