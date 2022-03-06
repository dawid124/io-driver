//
// Created by Dawid on 02.03.2022.
//

#ifndef IO_DRIVER_ITHINGS_H
#define IO_DRIVER_ITHINGS_H

#include "Arduino.h"

class IThings {
public:
    String id;
    explicit IThings(const String &id);
};


#endif //IO_DRIVER_ITHINGS_H
