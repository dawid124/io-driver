//
// Created by Dawid on 06.03.2022.
//

#include "Color.h"

Color::Color(uint8_t r, uint8_t g, uint8_t b) : r(r), g(g), b(b) {
    w = 0;
}

Color::Color(uint8_t r, uint8_t g, uint8_t b, uint8_t w) : r(r), g(g), b(b), w(w) {}

Color::Color() {

}
