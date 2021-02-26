#include <iostream>
#include <sstream>
#include "utils.hpp"


bool z_sort(Element* a, Element* b){
    return a->z_index < b->z_index;
}


std::string get_filename(std::string const& filename, int i, std::string const& ext){
    std::string index = std::to_string(i);
    index = std::string(6 - index.length(), '0').append(index);
    std::string fn = filename + index + ext;
    return fn;
}

Color hex_to_rgb(std::string hex){
    hex.erase(0, 1);
    int num = stoi(hex, 0, 16);
    float r = num / 0x10000;
    float g = (num / 0x100) % 0x100;
    float b = num % 0x100;
    r /= 255;
    g /= 255;
    b /= 255;
    Color color = std::make_tuple(r, g, b);
    return color;
}
