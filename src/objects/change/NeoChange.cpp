//
// Created by Dawid on 22.03.2022.
//

#include "NeoChange.h"


NeoChange::NeoChange(const Color &color,int brightness, int steps, NeoLight *neo) {
    uint32_t currentColor = neo->ws2812fx.getColor();
    uint8_t currentBrightness = neo->ws2812fx.getBrightness();
    uint8_t b = (uint8_t) ((currentColor) & 0xFF);
    uint8_t g = (uint8_t) ((currentColor >> 8) & 0xFF);
    uint8_t r = (uint8_t) ((currentColor >> 16) & 0xFF);
    uint8_t w = (uint8_t) ((currentColor >> 24) & 0xFF);

    rStep = ((float) (color.r - r)) / (float) steps;
    gStep = ((float) (color.g - g)) / (float) steps;
    bStep = ((float) (color.b - b)) / (float) steps;
    wStep = ((float) (color.w - w)) / (float) steps;
    brightnessStep = ((float) (brightness - currentBrightness)) / (float) steps;


    rVal = (float) r;
    gVal = (float) g;
    bVal = (float) b;
    wVal = (float) w;
    brightnessVal = (float) currentBrightness;
}

bool NeoChange::nextStep() {
    Serial.println("steps: " + String(steps));
    if (steps == 0) {
        neo->ws2812fx.setColor(color.r, color.g, color.b, color.w);
        return false;
    }

    rVal = rVal + rStep;
    gVal = gVal + gStep;
    bVal = bVal + bStep;
    wVal = wVal + wStep;
    brightnessVal = brightnessVal + brightnessStep;

    neo->ws2812fx.setColor(rVal, gVal, bVal, wVal);
    neo->ws2812fx.setBrightness(brightnessVal);

    steps--;
    return true;
}
