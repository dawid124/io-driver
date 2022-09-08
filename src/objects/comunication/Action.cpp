//
// Created by Dawid on 26.02.2022.
//

#include "Action.h"

Action::Action(String ioId) : ioId(std::move(ioId)) {
    animationId = -1;
    staticSubMode = 0;
    stepTime = 0;
    i = 0;
    speed = -1;
    time = 0;
    delay = 0;
    stepTime = 0;
}

