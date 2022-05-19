//
// Created by Dawid on 26.02.2022.
//

#ifndef IO_DRIVER_ACTION_H
#define IO_DRIVER_ACTION_H

#include "Arduino.h"
#include "Color.h"

class Action {
public:
    Action(Action *pAction);

    String ioId;
    String ioType;
    String action;

    Color color;
    uint8_t brightness;

    int animationId;
    int speed;
    int time;
    int stepTime;

    Action(const String &ioId, const String &ioType, String action, const Color &color, uint8_t brightness,
           int animationId, int speed, int time, int stepTime);
};


#endif //IO_DRIVER_ACTION_H
