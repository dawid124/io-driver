//
// Created by Dawid on 06.03.2022.
//

#include "NeoLight.h"

NeoLight::NeoLight(const String& id, uint8_t pin, uint16_t ledCount, neoPixelType type)
        : IThings(id), pin(pin), ledCount(ledCount), type(type), ws2812fx(WS2812FX(ledCount, pin, type)) {

    ws2812fx.begin();
    ws2812fx.setBrightness(255);
    ws2812fx.setSpeed(100);
    ws2812fx.setColor(0,255,0,0);
    ws2812fx.setMode(FX_MODE_RAINBOW_CYCLE);
    ws2812fx.start();
    service();
    Serial.println(ws2812fx.getMode());
}

void NeoLight::service() {
    ws2812fx.service();
}
