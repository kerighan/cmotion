#include <iostream>
#include <math.h>
#include "shape.hpp"
#include "utils.hpp"

// ============================================================================
// Circle class
// ============================================================================
Circle::Circle(float x, float y, float radius, const std::string color, float opacity, bool responsive, char* align, int z_index){
    this->x = x;
    this->y = y;
    this->radius = radius;
    this->color = hex_to_rgb(color);
    this->opacity = opacity;
    this->z_index = z_index;
    this->align = parse_alignment(align);
    this->responsive = responsive;
}


void Circle::draw(cairo_t* cr, float t){
    // compute default attributes
    Attributes attributes;
    attributes["x"] = this->x;
    attributes["y"] = this->y;
    attributes["radius"] = this->radius;
    attributes["opacity"] = this->opacity;
    this->at(attributes, t);

    // get main attributes
    float x = this->get_x(attributes["x"]);
    float y = this->get_y(attributes["y"]);
    float radius = this->get_x(attributes["radius"]);
    float opacity = attributes["opacity"];

    // return conditions
    if (radius <= 0 || opacity == 0)
        return;

    // draw circle
    float r = std::get<0>(this->color);
    float g = std::get<1>(this->color);
    float b = std::get<2>(this->color);
    cairo_set_source_rgba(cr, r, g, b, opacity);
    cairo_arc(cr, x, y, radius, 0, 2 * M_PI);
    cairo_fill(cr);
}

// ============================================================================
// Rectangle class
// ============================================================================
Rectangle::Rectangle(float x, float y, float width, float height, const std::string color, float opacity, bool responsive, char* align, int z_index){
    this->x = x;
    this->y = y;
    this->width = width;
    this->height = height;
    this->color = hex_to_rgb(color);
    this->opacity = opacity;
    this->z_index = z_index;
    this->align = parse_alignment(align);
    this->responsive = responsive;
}


void Rectangle::draw(cairo_t* cr, float t){
    // compute default attributes
    Attributes attributes;
    attributes["x"] = this->x;
    attributes["y"] = this->y;
    attributes["width"] = this->width;
    attributes["height"] = this->height;
    attributes["opacity"] = this->opacity;
    this->at(attributes, t);

    // get main attributes
    float x = this->get_x(attributes["x"]);
    float y = this->get_y(attributes["y"]);
    float width = this->get_x(attributes["width"]);
    float height = this->get_y(attributes["height"]);
    float opacity = attributes["opacity"];

    // return conditions
    if (width <= 0 || height <= 0 || opacity == 0)
        return;
    
    // adapt to alignment
    adapt_to_alignment(this->align, &x, &y, &width, &height);

    // draw Rectangle
    float r = std::get<0>(this->color);
    float g = std::get<1>(this->color);
    float b = std::get<2>(this->color);
    cairo_set_source_rgba(cr, r, g, b, opacity);
    cairo_rectangle(cr, x, y, width, height);
    cairo_fill(cr);
}