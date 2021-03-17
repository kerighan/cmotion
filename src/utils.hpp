#ifndef UTILS_H_
#define UTILS_H_
#include "element.hpp"
#include <iostream>
#include <memory>
#include <sstream>

using Color = std::tuple<float, float, float>;

bool z_sort(Element* a, Element* b);

std::string get_filename(std::string const& filename, int i, std::string const& ext);

Color hex_to_rgb(std::string hex);

#endif