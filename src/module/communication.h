//
// Created by Dawid on 26.02.2022.
//

#ifndef IO_DRIVER_COMMUNICATION_H
#define IO_DRIVER_COMMUNICATION_H

#include "./defines.h"
#include "./module/logger.h"
#include <malloc.h>

typedef Action* ActionPtr;

struct Queue {
    int front, rear, capacity;
    ActionPtr* queue;
    Queue(int c)
    {
        front = rear = 0;
        capacity = c;
        queue = new ActionPtr[128];
    }

    ~Queue() { delete[] queue; }

    void queueEnqueue(Action* data)
    {
        if (capacity == rear) {
            printf("\nQueue is full\n");
        } else {
            queue[rear] = data;
            rear++;
        }
    }

    ActionPtr queueDequeue()
    {
        if (front == rear) {
            printf("\nQueue is  empty\n");
            return nullptr;
        } else {
            Action* el = queue[0];
            for (int i = 0; i < rear - 1; i++) {
                queue[i] = queue[i + 1];
            }

            rear--;

            return el;
        }
    }
};

extern const char *ACTION_TOPIC;
extern mutex_t actionCore1MsgMutex;
extern Queue actionCore0Msgs;
extern Queue actionCore1Msgs;

void setupMqtt();

void reconnectMqtt();

void actionTopicCallback(char *topic, byte *payload, unsigned int length);

void mqttService();

void addToCore1Mutex(Action *action);

void resetENC();

void publishSensor(const String& id, bool state);

void parseBytes(const char *str, char sep, byte *bytes, int maxBytes, int base);

#endif //IO_DRIVER_COMMUNICATION_H
