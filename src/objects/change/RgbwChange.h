//
// Created by Dawid on 06.03.2022.
//

#ifndef IO_DRIVER_RGBWCHANGE_H
#define IO_DRIVER_RGBWCHANGE_H


#include <objects/iot/rgb/RgbwLight.h>
#include <objects/comunication/Color.h>

class RgbwChange {
private:
    Color color;
    int steps;
    RgbwLight *light;

    float rStep;
    float gStep;
    float bStep;
    float wStep;

    float rVal;
    float gVal;
    float bVal;
    float wVal;
public:
    RgbwChange(const Color &color, int steps, RgbwLight *light);
    bool nextStep();
};


#endif //IO_DRIVER_RGBWCHANGE_H
