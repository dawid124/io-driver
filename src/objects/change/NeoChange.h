//
// Created by Dawid on 22.03.2022.
//

#ifndef IO_DRIVER_NEOCHANGE_H
#define IO_DRIVER_NEOCHANGE_H


#include <objects/iot/neo/NeoLight.h>
#include <objects/comunication/Color.h>

class NeoChange {
private:
    Color color;
    int steps;
    NeoLight *neo;

    float rStep;
    float gStep;
    float bStep;
    float wStep;
    float brightnessStep;

    float rVal;
    float gVal;
    float bVal;
    float wVal;
    float brightnessVal;
public:
    NeoChange(const Color &color, int brightness, int steps, NeoLight *neo);
    bool nextStep();
};


#endif //IO_DRIVER_NEOCHANGE_H
