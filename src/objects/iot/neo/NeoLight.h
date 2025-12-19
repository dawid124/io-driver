//
// Created by Dawid on 06.03.2022.
//

#ifndef IO_DRIVER_NEOLIGHT_H
#define IO_DRIVER_NEOLIGHT_H

#include "Arduino.h"
#include <vector>
#include <objects/iot/IThings.h>
#include <WS2812FX.h>

class NeoLight: public IThings {
private:
    uint16_t ledCount;
    neoPixelType type;
    uint8_t pin;
public:
    WS2812FX ws2812fx;
    std::vector<WS2812FX> collection;
    NeoLight(const String& id, uint8_t pin, uint16_t ledCount, neoPixelType type, bool reverse);

//    NeoLight(const String& id, uint8_t pin, uint16_t ledCount, neoPixelType type,
//             const std::vector<WS2812FX> &collection);

    void service();
};


#endif //IO_DRIVER_NEOLIGHT_H
