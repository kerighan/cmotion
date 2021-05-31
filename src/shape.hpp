#include <cairo.h>
#include "element.hpp"
#include "utils.hpp"

class Circle: public Element {
public:
    Circle(float x, float y, float radius, std::string color, float opacity, bool responsive, char* align, int z_index);
    Circle(const Circle& circle);
    Circle* clone() override{ return new Circle(*this); }

    void draw(cairo_t * cr, float t) override;
private:
    float radius;
    Color color;
};

class Rectangle: public Element {
public:
    Rectangle(float x, float y, float width, float height, std::string color, float opacity, bool responsive, char* align, int z_index);
    Rectangle(const Rectangle& rect);
    Rectangle* clone() override{ return new Rectangle(*this); }

    void draw(cairo_t * cr, float t) override;
private:
    float width;
    float height;
    Color color;
};


class Line: public Element {
public:
    Line(std::vector<float>& x, std::vector<float>& y, float stroke_width, int cap, std::string color, float opacity, double dotted, bool responsive, char* align, int z_index);
    Line(const Line& line);
    Line* clone() override{ return new Line(*this); }

    void draw(cairo_t * cr, float t) override;
private:
    std::vector<float> x_pos;
    std::vector<float> y_pos;
    float stroke_width;
    double dotted;
    Color color;
    cairo_line_cap_t cap;
};