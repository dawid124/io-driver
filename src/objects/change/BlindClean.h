//
// Created by Dawid on 02.08.2022.
//

#ifndef IO_DRIVER_BLINDCLEAN_H
#define IO_DRIVER_BLINDCLEAN_H


#include <objects/iot/blind/Blind.h>

class BlindClean {
private:
    Blind *blind;
public:
    BlindClean(Blind *blind);
    bool clean();
};


#endif //IO_DRIVER_BLINDCLEAN_H
