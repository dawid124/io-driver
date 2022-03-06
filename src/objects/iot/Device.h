//
// Created by Dawid on 02.03.2022.
//

#ifndef IO_DRIVER_DEVICE_H
#define IO_DRIVER_DEVICE_H

#include "Arduino.h"
#include "IThings.h"

class Device {
public:
    String id;
    IThings* iot;

    Device(String id, IThings* iot);
};


#endif //IO_DRIVER_DEVICE_H
