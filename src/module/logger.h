//
// Created by Dawid on 18.05.2022.
//

#ifndef IO_DRIVER_LOGGER_H
#define IO_DRIVER_LOGGER_H

#include "Arduino.h"
#include "defines.h"

class Logger {
public:
    static void print(const String &s);
    static void println(const String &s);

    static void printCore0(const String &s);
    static void printlnCore0(const String &s);

    static void printCore1(const String &s);
    static void printlnCore1(const String &s);

    static void printCore0(const Printable&);
    static void printlnCore0(const Printable&);

};



#endif //IO_DRIVER_LOGGER_H
