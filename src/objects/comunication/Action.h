//
// Created by Dawid on 26.02.2022.
//

#ifndef IO_DRIVER_ACTION_H
#define IO_DRIVER_ACTION_H

#include "Arduino.h"
#include "Color.h"

class Action {
public:
    String ioId;
    String ioType;
    String action;

    Color color;
    uint8_t brightness;

    int animationId;
    int staticSubMode;
    int speed;
    int time;
    int delay;
    int stepTime;

    Action(String ioId);

    Action(String ioId, String ioType, String action, Color color, uint8_t brightness, int animationId,
           int staticSubMode, int speed, int time, int stepTime, int delay);

    Action(String ioId, String action, Color color, uint8_t brightness, int animationId, int staticSubMode,
           int speed, int time, int stepTime, int delay);
};


#endif //IO_DRIVER_ACTION_H
