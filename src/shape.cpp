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


Circle::Circle(const Circle& element){
    this->copy(element);
    this->radius = element.radius;
    this->color = element.color;
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


Rectangle::Rectangle(const Rectangle& element){
    this->copy(element);
    this->width = element.width;
    this->height = element.height;
    this->color = element.color;
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

// ============================================================================
// Rectangle class
// ============================================================================

Line::Line(
    std::vector<float>& x,
    std::vector<float>& y,
    float stroke_width,
    int cap,
    const std::string color,
    float opacity,
    bool responsive,
    char* align,
    int z_index
){
    this->x_pos = x;
    this->y_pos = y;
    this->stroke_width = stroke_width;
    this->color = hex_to_rgb(color);
    this->opacity = opacity;
    this->z_index = z_index;
    this->align = parse_alignment(align);
    this->responsive = responsive;

    switch (cap){
        case 1:
            this->cap = CAIRO_LINE_CAP_ROUND;
            break;
        case 2:
            this->cap = CAIRO_LINE_CAP_SQUARE;
            break;
        default:
            this->cap = CAIRO_LINE_CAP_BUTT;
            break;
    }
}


Line::Line(const Line& element){
    this->copy(element);
    this->x_pos = element.x_pos;
    this->y_pos = element.y_pos;
    this->stroke_width = element.stroke_width;
    this->color = element.color;
    this->cap = element.cap;
}


void Line::draw(cairo_t* cr, float t){
    // compute default attributes
    Attributes attributes;
    attributes["stroke_width"] = this->stroke_width;
    attributes["opacity"] = this->opacity;
    this->at(attributes, t);

    // get main attributes
    float stroke_width = this->get_x(attributes["stroke_width"]);
    float opacity = attributes["opacity"];

    // return conditions
    if (stroke_width <= 0 || opacity == 0)
        return;

    // number of points
    size_t n_points = this->x_pos.size();
    if (n_points <= 1)
        return;

    // draw lines
    float r = std::get<0>(this->color);
    float g = std::get<1>(this->color);
    float b = std::get<2>(this->color);
    cairo_set_source_rgba(cr, r, g, b, opacity);
    cairo_set_line_cap(cr, this->cap);
    cairo_set_line_join(cr, CAIRO_LINE_JOIN_ROUND);
    cairo_set_line_width(cr, stroke_width);
    
    cairo_move_to(cr, this->get_x(x_pos[0]), this->get_y(y_pos[0]));
    for (size_t i = 1; i < n_points; i++){
        cairo_line_to(cr, this->get_x(x_pos[i]), this->get_y(y_pos[i]));
    }
    cairo_stroke(cr);
}