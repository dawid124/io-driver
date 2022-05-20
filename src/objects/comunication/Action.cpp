//
// Created by Dawid on 26.02.2022.
//

#include "Action.h"

Action::Action(String ioId, String ioType, String action, Color color, uint8_t brightness,
               int animationId, int speed, int time, int stepTime) : ioId(std::move(ioId)), ioType(std::move(ioType)), action(std::move(action)), color(color),
                                                       brightness(brightness), animationId(animationId), speed(speed),
                                                       time(time), stepTime(stepTime) {}

Action::Action(String ioId, String action, Color color, uint8_t brightness,
               int animationId, int speed, int time, int stepTime) : ioId(std::move(ioId)), action(std::move(action)), color(color),
                                                                     brightness(brightness), animationId(animationId), speed(speed),
                                                                     time(time), stepTime(stepTime) {}

Action::Action(String ioId) : ioId(std::move(ioId)) {
    stepTime = 0;
}

