#include <cairo.h>
#include "element.hpp"
#include "utils.hpp"

class Circle: public Element {
public:
    Circle(float x, float y, float radius, std::string color, float opacity, bool responsive, char* align, int z_index);
    void draw(cairo_t * cr, float t) override;
private:
    float radius;
    Color color;
};

class Rectangle: public Element {
public:
    Rectangle(float x, float y, float width, float height, std::string color, float opacity, bool responsive, char* align, int z_index);
    void draw(cairo_t * cr, float t) override;
private:
    float width;
    float height;
    Color color;
};


class Line: public Element {
public:
    Line(std::vector<float>& x, std::vector<float>& y, float stroke_width, std::string color, float opacity, bool responsive, char* align, int z_index);
    void draw(cairo_t * cr, float t) override;
private:
    std::vector<float> x_pos;
    std::vector<float> y_pos;
    float stroke_width;
    Color color;
};