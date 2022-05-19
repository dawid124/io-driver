//
// Created by Dawid on 18.05.2022.
//

#include "logger.h"

void Logger::print(const String &s) {
#ifdef APP_DEBUG
    Serial.print(s);
#endif
}

void Logger::println(const String &s) {
#ifdef APP_DEBUG
    Serial.print("\n" + s);
#endif
}

void Logger::printCore0(const String &s) {
#ifdef APP_DEBUG
    print("[Core0] " + s);
#endif
}

void Logger::printlnCore0(const String &s) {
#ifdef APP_DEBUG
    print("\n[Core0] " + s);
#endif
}

void Logger::printCore1(const String &s) {
#ifdef APP_DEBUG
    print("[Core1] " + s);
#endif
}

void Logger::printlnCore1(const String &s) {
#ifdef APP_DEBUG
    print("\n[Core1] " + s);
#endif
}

void Logger::printCore0(const Printable & x) {
#ifdef APP_DEBUG
    print("[Core0] ");
    Serial.print(x);
#endif
}

void Logger::printlnCore0(const Printable& x) {
#ifdef APP_DEBUG
    print("\n[Core0] ");
    Serial.println(x);
#endif
}
