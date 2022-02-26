//
// Created by Dawid on 26.02.2022.
//

#ifndef IO_DRIVER_ACTION_H
#define IO_DRIVER_ACTION_H

#include "Arduino.h"

class Action {
public:
    String ioId;
    String type;

    Action(const String &ioId, const String &type);
};


#endif //IO_DRIVER_ACTION_H
