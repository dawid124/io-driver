//
// Created by Dawid on 06.03.2022.
//

#ifndef IO_DRIVER_RGBWCHANGE_H
#define IO_DRIVER_RGBWCHANGE_H


#include <objects/iot/rgb/RgbwLight.h>
#include <objects/comunication/Color.h>

class RgbwChange {
private:
    RgbwLight *light;
    Color color;
    int steps;

    float rStep;
    float gStep;
    float bStep;
    float wStep;
    float wwStep;

    float rVal;
    float gVal;
    float bVal;
    float wVal;
    float wwVal;
public:
    int delay;
    RgbwChange(const Color &color, int steps, RgbwLight *light, int delay);
    bool nextStep();
};


#endif //IO_DRIVER_RGBWCHANGE_H
