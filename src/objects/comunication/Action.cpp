//
// Created by Dawid on 26.02.2022.
//

#include "Action.h"

Action::Action(String ioId, String ioType, String action, Color color, uint8_t brightness,
               int animationId, int speed, int time, int stepTime) : ioId(std::move(ioId)), ioType(std::move(ioType)), action(std::move(action)), color(color),
                                                       brightness(brightness), animationId(animationId), speed(speed),
                                                       time(time), stepTime(stepTime) {}

Action::Action(Action *pAction) {
    ioId = pAction->ioId;
    ioType = pAction->ioType;
    action = pAction->action;
//    color = pAction->color;
    brightness = pAction->brightness;
    animationId = pAction->animationId;
    speed = pAction->speed;
    time = pAction->time;
    stepTime = pAction->stepTime;
}
