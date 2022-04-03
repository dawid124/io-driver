//
// Created by Dawid on 06.03.2022.
//

#include "NeoLight.h"

NeoLight::NeoLight(const String& id, uint8_t pin, uint16_t ledCount, neoPixelType type)
        : IThings(id), pin(pin), ledCount(ledCount), type(type), ws2812fx(WS2812FX(ledCount, pin, type)) {

    ws2812fx.begin();
    ws2812fx.setBrightness(0);
    ws2812fx.setSpeed(1000);
    ws2812fx.setColor(0,0,0,255);
    ws2812fx.setMode(FX_MODE_STATIC);
//    ws2812fx.start();
}

void NeoLight::service() {
    ws2812fx.service();
}
