//
// Created by Dawid on 06.03.2022.
//

#include "RgbwChange.h"

RgbwChange::RgbwChange(const Color &color, int steps, RgbwLight *light, int delay)
        : color(color), steps(steps), light(light), delay(delay) {
    rStep = ((float) (color.r - light->R)) / (float) steps;
    gStep = ((float) (color.g - light->G)) / (float) steps;
    bStep = ((float) (color.b - light->B)) / (float) steps;
    wStep = ((float) (color.w - light->W)) / (float) steps;
    wwStep = ((float) (color.ww - light->Ww)) / (float) steps;

    rVal = (float) light->R;
    gVal = (float) light->G;
    bVal = (float) light->B;
    wVal = (float) light->W;
    wwVal = (float) light->Ww;
}

bool RgbwChange::nextStep() {
    if (steps == 0) {
        light->color(color.r, color.g, color.b, color.w, color.ww);
        light->task = (uintptr_t)(NULL);
        return false;
    }

    rVal = rVal + rStep;
    gVal = gVal + gStep;
    bVal = bVal + bStep;
    wVal = wVal + wStep;
    wwVal = wwVal + wwStep;

//    Serial.println("RgbwChange: id:" + String(light->id)
//    + " r: " + String(rVal)
//    + " g: " + String(gVal)
//    + " b: " + String(bVal)
//    + " w: " + String(wVal));

    light->color(rVal, gVal, bVal, wVal, wwVal);
    steps--;
    return true;
}
