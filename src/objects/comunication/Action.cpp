//
// Created by Dawid on 26.02.2022.
//

#include "Action.h"

Action::Action(const String &ioId, const String &ioType, String action, const Color &color, uint8_t brightness,
               int animationId, int speed, int time, int stepTime) : ioId(ioId), ioType(ioType), action(action), color(color),
                                                       brightness(brightness), animationId(animationId), speed(speed),
                                                       time(time), stepTime(stepTime) {}
