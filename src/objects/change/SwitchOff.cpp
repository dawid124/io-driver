//
// Created by Dawid on 24.08.2022.
//

#include "SwitchOff.h"

SwitchOff::SwitchOff(Switch *switchh) : switchh(switchh) {}

bool SwitchOff::off() {
    this->switchh->off();
    return false;
}
