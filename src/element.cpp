#include "element.hpp"
#include <string.h>


Element::Element(){
    this->screen_width = -1;
    this->screen_height = -1;
    this->x = 50;
    this->y = 50;
    this->opacity = 1;
    this->z_index = 0;
    this->align = layout::position::center;
    this->responsive = true;
}

Element::~Element() {
    // Currently, there's nothing to clean up because this class
    // doesn't manage any dynamic resources directly.
    // This destructor is here for completeness and future-proofing.
}

Element::Element(const Element& element){
    this->screen_width = element.screen_width;
    this->screen_height = element.screen_height;
    this->x = element.x;
    this->y = element.y;
    this->opacity = element.opacity;
    this->z_index = element.z_index;
    this->align = element.align;
    this->responsive = element.responsive;
    this->timelines = element.timelines;
}


void Element::copy(const Element& element){
    this->screen_width = element.screen_width;
    this->screen_height = element.screen_height;
    this->x = element.x;
    this->y = element.y;
    this->opacity = element.opacity;
    this->z_index = element.z_index;
    this->align = element.align;
    this->responsive = element.responsive;
    this->timelines = element.timelines;
}


void Element::add(Timeline& timeline){
    this->timelines.push_back(timeline);
}


void Element::at(Attributes& attributes, float t){
    attributes["t"] = t;
    for (auto& timeline: this->timelines){
        timeline.at(attributes);
    }
}

// ============================================================================
// Positioning functions
// ============================================================================
void Element::set_screen(int w, int h){
    this->screen_height = h;
    this->screen_width = w;
    this->on_resize();
}


void Element::set_position(float x, float y){
    this->x = x;
    this->y = y;
}


float Element::get_x(float x){
    if (!this->responsive)
        return x;
    
    return x * this->screen_width / 100.;
}


float Element::get_y(float y){
    if (!this->responsive)
        return y;
    
    return y * this->screen_height / 100.;
}


float Element::get_end(){
    float end = 0;
    for (auto& timeline : this->timelines){
        float tmp = timeline.get_end();
        if (end < tmp)
            end = tmp;
    }
    return end;
}

void Element::get_attributes(Attributes& attributes, float t){
    attributes["t"] = t;
}


void Element::on_resize(){}


// ============================================================================
// Positioning utils
// ============================================================================
layout::position parse_alignment(char* alignment){
    if (!strcmp(alignment, "center")) return layout::position::center;
    if (!strcmp(alignment, "top")) return layout::position::top;
    if (!strcmp(alignment, "top_right")) return layout::position::top_right;
    if (!strcmp(alignment, "right")) return layout::position::right;
    if (!strcmp(alignment, "bottom_right")) return layout::position::bottom_right;
    if (!strcmp(alignment, "bottom")) return layout::position::bottom;
    if (!strcmp(alignment, "bottom_left")) return layout::position::bottom_left;
    if (!strcmp(alignment, "left")) return layout::position::left;
    if (!strcmp(alignment, "top_left")) return layout::position::top_left;
    return layout::position::center;
}


layout::fit parse_fit(char* fit){
    if (!strcmp(fit, "cover")) return layout::fit::cover;
    if (!strcmp(fit, "contain")) return layout::fit::contain;
    return layout::fit::cover;
}


void adapt_to_alignment(layout::position& align, float* x, float* y, float* width, float* height){
    switch (align){
        case layout::position::center:
            *x -= *width / 2;
            *y -= *height / 2;
            break;
        case layout::position::top:
            *x -= *width / 2;
            break;
        case layout::position::top_right:
            *x -= *width;
            break;
        case layout::position::right:
            *x -= *width;
            *y -= *height / 2;
            break;
        case layout::position::bottom_right:
            *x -= *width;
            *y -= *height;
            break;
        case layout::position::bottom:
            *x -= *width / 2;
            *y -= *height;
            break;
        case layout::position::bottom_left:
            *y -= *height;
            break;
        case layout::position::left:
            *y -= *height / 2;
            break;
        case layout::position::top_left:
            break;
    }
    return;
}