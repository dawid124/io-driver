//
// Created by Dawid on 24.08.2022.
//

#ifndef IO_DRIVER_SWITCHOFF_H
#define IO_DRIVER_SWITCHOFF_H


#include <objects/iot/switch/Switch.h>

class SwitchOff {
private:
    Switch* switchh;
public:
    SwitchOff(Switch *switchh);

    bool off();
};


#endif //IO_DRIVER_SWITCHOFF_H
