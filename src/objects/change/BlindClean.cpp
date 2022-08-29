//
// Created by Dawid on 02.08.2022.
//

#include "BlindClean.h"

BlindClean::BlindClean(Blind *blind) : blind(blind) {}

bool BlindClean::clean() {
    this->blind->clear();
    return false;
}
