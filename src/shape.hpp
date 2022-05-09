#include <cairo.h>
#include "element.hpp"
#include "utils.hpp"

class Circle : public Element
{
public:
    Circle(float x, float y, float radius, std::string color, float opacity, bool responsive, char *align, int z_index, float stroke_width, float angle, float phase);
    Circle(const Circle &circle);
    Circle *clone() override { return new Circle(*this); }

    void draw(cairo_t *cr, float t) override;

private:
    float radius;
    Color color;
    float stroke_width;
    float angle;
    float phase;
};

class Hexagon : public Element
{
public:
    Hexagon(float x, float y, float radius, std::string color, float opacity, bool responsive, char *align, int z_index, float stroke_width);
    Hexagon(const Hexagon &element);
    Hexagon *clone() override { return new Hexagon(*this); }
    void draw(cairo_t *cr, float t) override;

private:
    float radius;
    float stroke_width;
    Color color;
};

class Rectangle : public Element
{
public:
    Rectangle(float x, float y, float width, float height, std::string color, float opacity, float border_radius, bool responsive, char *align, int z_index, float stroke_width);
    Rectangle(const Rectangle &rect);
    Rectangle *clone() override { return new Rectangle(*this); }

    void draw(cairo_t *cr, float t) override;

private:
    float width;
    float height;
    float border_radius;
    float stroke_width;
    Color color;
};

class Line : public Element
{
public:
    Line(std::vector<float> &x, std::vector<float> &y, float stroke_width, int cap, std::string color, float opacity, double dotted, bool responsive, char *align, int z_index);
    Line(const Line &line);
    Line *clone() override { return new Line(*this); }

    void draw(cairo_t *cr, float t) override;

private:
    std::vector<float> x_pos;
    std::vector<float> y_pos;
    float stroke_width;
    double dotted;
    Color color;
    cairo_line_cap_t cap;
};

class Curve : public Element
{
public:
    Curve(
        std::vector<float> &x, std::vector<float> &y, float stroke_width, int cap, std::string color, float opacity, double dotted, bool responsive, char *align, int z_index, float curvature);
    Curve(const Curve &Curve);
    Curve *clone() override { return new Curve(*this); }

    void draw(cairo_t *cr, float t) override;

private:
    std::vector<float> x_pos;
    std::vector<float> y_pos;
    float stroke_width;
    double dotted;
    Color color;
    cairo_line_cap_t cap;
    float curvature;
};

class Polygon : public Element
{
public:
    Polygon(std::vector<float> &x, std::vector<float> &y, float stroke_width, int cap, std::string color, float opacity, double dotted, bool responsive, char *align, int z_index);
    Polygon(const Polygon &element);
    Polygon *clone() override { return new Polygon(*this); }

    void draw(cairo_t *cr, float t) override;

private:
    std::vector<float> x_pos;
    std::vector<float> y_pos;
    float stroke_width;
    double dotted;
    Color color;
    cairo_line_cap_t cap;
};
