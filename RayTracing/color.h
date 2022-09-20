#ifndef COLOR_H
#define COLOR_H

#include "vec3.h"

#include <iostream>

void write_color(std::fstream& out, color pixel_color) {
    auto r = pixel_color.x();
    auto g = pixel_color.y();
    auto b = pixel_color.z();

    // Write the translated [0,255] value of each color component.
    out << static_cast<int>(256 * r) << ' '
        << static_cast<int>(256 * g) << ' '
        << static_cast<int>(256 * b) << '\n';
}

#endif