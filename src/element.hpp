#ifndef ELEMENT_H_
#define ELEMENT_H_
#include <vector>
#include <cairo.h>
#include <iostream>
#include "timeline.hpp"


// ============================================================================
// layout enumerations
// ============================================================================

namespace layout
{
    enum position {
        center = 0,
        top = 1,
        top_right = 2,
        right = 3,
        bottom_right = 4,
        bottom = 5,
        bottom_left = 6,
        left = 7,
        top_left = 8
    };

    enum fit {
        cover = 0,
        contain = 1
    };
}

layout::position parse_alignment(char* align);
layout::fit parse_fit(char* fit);
void adapt_to_alignment(layout::position& align, float* x, float* y, float* width, float* height);


// ============================================================================
// Base Element class
// ============================================================================

class Element
{
public:
    Element();
    Element(const Element& elem);
    virtual Element* clone(){ return new Element(*this); };
    void copy(const Element& element);

    void add(Timeline& timeline);
    void set_screen(int w, int h);
    void set_position(float x, float y);
    float get_x(float x);
    float get_y(float y);
    void at(Attributes& attributes, float t);

    virtual float get_end();
    virtual void draw(cairo_t*, float){};
    virtual void on_resize();
    virtual void get_attributes(Attributes& attributes, float t);

    int z_index;

protected:
    std::vector<Timeline> timelines;
    float x;
    float y;
    float opacity;
    int screen_width;
    int screen_height;
    layout::position align;
    bool responsive;
};

#endif