#include <iostream>
#include <math.h>
#include "shape.hpp"
#include "utils.hpp"

// ============================================================================
// Circle class
// ============================================================================

Circle::Circle(float x, float y, float radius, const std::string color, float opacity, bool responsive, char *align, int z_index, float stroke_width, float angle, float phase)
{
    this->x = x;
    this->y = y;
    this->radius = radius;
    this->color = hex_to_rgb(color);
    this->opacity = opacity;
    this->z_index = z_index;
    this->align = parse_alignment(align);
    this->responsive = responsive;
    this->stroke_width = stroke_width;
    this->angle = angle;
    this->phase = phase;
}

Circle::Circle(const Circle &element)
{
    this->copy(element);
    this->radius = element.radius;
    this->color = element.color;
    this->stroke_width = element.stroke_width;
    this->angle = element.angle;
    this->phase = element.phase;
}

void Circle::draw(cairo_t *cr, float t)
{
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
    float angle = this->angle * M_PI / 180;
    float phase = this->phase * M_PI / 180;

    // return conditions
    if (radius <= 0 || opacity == 0)
        return;

    // draw circle
    float r = std::get<0>(this->color);
    float g = std::get<1>(this->color);
    float b = std::get<2>(this->color);
    cairo_set_source_rgba(cr, r, g, b, opacity);

    cairo_move_to(cr, x + radius * cos(phase), y + radius * sin(phase));
    cairo_arc(cr, x, y, radius, phase, angle + phase);
    if (stroke_width == 0)
    {
        cairo_fill(cr);
    }
    else
    {
        float sw = this->get_x(stroke_width);
        cairo_set_line_width(cr, sw);
        cairo_stroke(cr);
    }
}

// ============================================================================
// Hexagon class
// ============================================================================

Hexagon::Hexagon(float x, float y, float radius, const std::string color, float opacity, bool responsive, char *align, int z_index, float stroke_width)
{
    this->x = x;
    this->y = y;
    this->radius = radius;
    this->color = hex_to_rgb(color);
    this->opacity = opacity;
    this->z_index = z_index;
    this->align = parse_alignment(align);
    this->responsive = responsive;
    this->stroke_width = stroke_width;
}

Hexagon::Hexagon(const Hexagon &element)
{
    this->copy(element);
    this->radius = element.radius;
    this->color = element.color;
    this->stroke_width = element.stroke_width;
}

void Hexagon::draw(cairo_t *cr, float t)
{
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

    // draw Rectangle
    float r = std::get<0>(this->color);
    float g = std::get<1>(this->color);
    float b = std::get<2>(this->color);
    cairo_set_source_rgba(cr, r, g, b, opacity);

    float angle = M_PI / 6;
    for (size_t i = 0; i < 6; i++)
    {
        float x_ = x + radius * cos(angle);
        float y_ = y + radius * sin(angle);
        if (i == 0)
            cairo_move_to(cr, x_, y_);
        else
            cairo_line_to(cr, x_, y_);
        angle += M_PI / 3;
    }
    cairo_close_path(cr);

    if (stroke_width == 0)
    {
        cairo_fill(cr);
    }
    else
    {
        float sw = this->get_x(stroke_width);
        cairo_set_line_width(cr, sw);
        cairo_stroke(cr);
    }
}

// ============================================================================
// Rectangle class
// ============================================================================

Rectangle::Rectangle(float x, float y, float width, float height, const std::string color, float opacity, float border_radius, bool responsive, char *align, int z_index, float stroke_width)
{
    this->x = x;
    this->y = y;
    this->width = width;
    this->height = height;
    this->color = hex_to_rgb(color);
    this->opacity = opacity;
    this->border_radius = border_radius;
    this->z_index = z_index;
    this->align = parse_alignment(align);
    this->responsive = responsive;
    this->stroke_width = stroke_width;
}

Rectangle::Rectangle(const Rectangle &element)
{
    this->copy(element);
    this->width = element.width;
    this->height = element.height;
    this->border_radius = element.border_radius;
    this->color = element.color;
    this->stroke_width = element.stroke_width;
}

void Rectangle::draw(cairo_t *cr, float t)
{
    // compute default attributes
    Attributes attributes;
    attributes["x"] = this->x;
    attributes["y"] = this->y;
    attributes["width"] = this->width;
    attributes["height"] = this->height;
    attributes["opacity"] = this->opacity;
    attributes["border_radius"] = this->border_radius;
    this->at(attributes, t);

    // get main attributes
    float x = this->get_x(attributes["x"]);
    float y = this->get_y(attributes["y"]);
    float width = this->get_x(attributes["width"]);
    float height = this->get_y(attributes["height"]);
    float radius = attributes["border_radius"];
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
    if (radius == 0)
    {
        cairo_rectangle(cr, x, y, width, height);
    }
    else
    {
        float deg = M_PI / 180.0;
        cairo_move_to(cr, x, y);
        cairo_arc(cr,
                  x + width - radius,
                  y + radius,
                  radius,
                  -90 * deg, 0 * deg);
        cairo_arc(cr,
                  x + width - radius,
                  y + height - radius,
                  radius,
                  0 * deg, 90 * deg);
        cairo_arc(cr,
                  x + radius,
                  y + height - radius,
                  radius,
                  90 * deg,
                  180 * deg);
        cairo_arc(cr,
                  x + radius,
                  y + radius, radius,
                  180 * deg,
                  270 * deg);
        cairo_close_path(cr);
    }

    if (stroke_width == 0)
    {
        cairo_fill(cr);
    }
    else
    {
        float sw = this->get_x(stroke_width);
        cairo_set_line_width(cr, sw);
        cairo_stroke(cr);
    }
}

// ============================================================================
// Line class
// ============================================================================

Line::Line(
    std::vector<float> &x,
    std::vector<float> &y,
    float stroke_width,
    int cap,
    const std::string color,
    float opacity,
    double dotted,
    bool responsive,
    char *align,
    int z_index)
{
    this->x_pos = x;
    this->y_pos = y;
    this->stroke_width = stroke_width;
    this->color = hex_to_rgb(color);
    this->opacity = opacity;
    this->z_index = z_index;
    this->align = parse_alignment(align);
    this->responsive = responsive;
    this->dotted = dotted;

    switch (cap)
    {
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

Line::Line(const Line &element)
{
    this->copy(element);
    this->x_pos = element.x_pos;
    this->y_pos = element.y_pos;
    this->stroke_width = element.stroke_width;
    this->color = element.color;
    this->cap = element.cap;
    this->dotted = element.dotted;
}

void Line::draw(cairo_t *cr, float t)
{
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

    // dotted pattern
    if (this->dotted > 0)
    {
        double dash[1] = {this->dotted * stroke_width};
        cairo_set_dash(cr, dash, 1, 0);
    }

    cairo_move_to(cr, this->get_x(x_pos[0]), this->get_y(y_pos[0]));
    for (size_t i = 1; i < n_points; i++)
    {
        cairo_line_to(cr, this->get_x(x_pos[i]), this->get_y(y_pos[i]));
    }
    cairo_stroke(cr);

    // clear pattern
    if (this->dotted > 0)
    {
        cairo_set_dash(cr, {}, 0, 0);
    }
}

// ============================================================================
// Curve class
// ============================================================================

Curve::Curve(
    std::vector<float> &x,
    std::vector<float> &y,
    float stroke_width,
    int cap,
    const std::string color,
    float opacity,
    double dotted,
    bool responsive,
    char *align,
    int z_index,
    float curvature)
{
    this->x_pos = x;
    this->y_pos = y;
    this->stroke_width = stroke_width;
    this->color = hex_to_rgb(color);
    this->opacity = opacity;
    this->z_index = z_index;
    this->align = parse_alignment(align);
    this->responsive = responsive;
    this->dotted = dotted;
    this->curvature = curvature;

    switch (cap)
    {
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

Curve::Curve(const Curve &element)
{
    this->copy(element);
    this->x_pos = element.x_pos;
    this->y_pos = element.y_pos;
    this->stroke_width = element.stroke_width;
    this->color = element.color;
    this->cap = element.cap;
    this->dotted = element.dotted;
    this->curvature = element.curvature;
}

void Curve::draw(cairo_t *cr, float t)
{
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

    // dotted pattern
    if (this->dotted > 0)
    {
        double dash[1] = {this->dotted * stroke_width};
        cairo_set_dash(cr, dash, 1, 0);
    }

    float x0 = x_pos[0];
    float y0 = y_pos[0];
    float x1 = x_pos[1];
    float y1 = y_pos[1];
    float curvature = this->curvature;

    float delta_x = x1 - x0;
    float delta_y = y1 - y0;

    float xm0 = x0 + delta_x / 3 + curvature * delta_y;
    float ym0 = y0 + delta_y / 3 - curvature * delta_x;

    float xm1 = x0 + 2 * delta_x / 3 + curvature * delta_y;
    float ym1 = y0 + 2 * delta_y / 3 - curvature * delta_x;

    cairo_move_to(cr, this->get_x(x0), this->get_y(y0));
    cairo_curve_to(
        cr, this->get_x(xm0), this->get_y(ym0),
        this->get_x(xm1), this->get_y(ym1),
        this->get_x(x1), this->get_y(y1));

    cairo_stroke(cr);
}

// ============================================================================
// Polygon class
// ============================================================================

Polygon::Polygon(
    std::vector<float> &x,
    std::vector<float> &y,
    float stroke_width,
    int cap,
    const std::string color,
    float opacity,
    double dotted,
    bool responsive,
    char *align,
    int z_index)
{
    this->x_pos = x;
    this->y_pos = y;
    this->stroke_width = stroke_width;
    this->color = hex_to_rgb(color);
    this->opacity = opacity;
    this->z_index = z_index;
    this->align = parse_alignment(align);
    this->responsive = responsive;
    this->dotted = dotted;

    switch (cap)
    {
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

Polygon::Polygon(const Polygon &element)
{
    this->copy(element);
    this->x_pos = element.x_pos;
    this->y_pos = element.y_pos;
    this->stroke_width = element.stroke_width;
    this->color = element.color;
    this->cap = element.cap;
    this->dotted = element.dotted;
}

void Polygon::draw(cairo_t *cr, float t)
{
    // compute default attributes
    Attributes attributes;
    attributes["stroke_width"] = this->stroke_width;
    attributes["opacity"] = this->opacity;
    this->at(attributes, t);

    // get main attributes
    float stroke_width = this->get_x(attributes["stroke_width"]);
    float opacity = attributes["opacity"];

    // return conditions
    if (opacity == 0)
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

    // dotted pattern
    if (this->dotted > 0)
    {
        double dash[1] = {this->dotted * stroke_width};
        cairo_set_dash(cr, dash, 1, 0);
    }

    cairo_move_to(cr, this->get_x(x_pos[0]), this->get_y(y_pos[0]));
    for (size_t i = 1; i < n_points; i++)
    {
        cairo_line_to(cr, this->get_x(x_pos[i]), this->get_y(y_pos[i]));
    }
    cairo_close_path(cr);

    if (stroke_width > 0)
    {
        cairo_stroke(cr);
        // clear pattern
        if (this->dotted > 0)
        {
            cairo_set_dash(cr, {}, 0, 0);
        }
    }
    else
    {
        cairo_fill(cr);
    }
}
