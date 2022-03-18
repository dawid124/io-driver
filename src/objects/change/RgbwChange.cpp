//
// Created by Dawid on 06.03.2022.
//

#include "RgbwChange.h"

RgbwChange::RgbwChange(const Color &color, int steps, RgbwLight *light) : color(color), steps(steps), light(light) {
    rStep = ((float) (color.r - light->R)) / (float) steps;
    gStep = ((float) (color.g - light->G)) / (float) steps;
    bStep = ((float) (color.b - light->B)) / (float) steps;
    wStep = ((float) (color.w - light->W)) / (float) steps;

//    Serial.println("bStep: " + String(bStep));

    rVal = (float) light->R;
    gVal = (float) light->G;
    bVal = (float) light->B;
    wVal = (float) light->W;

//    Serial.println("steps: " + String(steps));
}

bool RgbwChange::nextStep() {
    if (steps == 0) {
        light->color(color.r, color.g, color.b, color.w);
        return false;
    }

    rVal = rVal + rStep;
    gVal = gVal + gStep;
    bVal = bVal + bStep;
    wVal = wVal + wStep;

//    Serial.println(String(steps) + " - wVal: " + Stri255g(wVal));

    light->color(rVal, gVal, bVal, wVal);

    steps--;
    return true;
}
