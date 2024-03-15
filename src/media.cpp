#include <cairo.h>
#include <iostream>
#include <cstring>
#include "media.hpp"
#include "cairo_jpg.hpp"
#include <math.h>

const char *get_file_extension(const char *filename)
{
    int ext = '.';
    const char *extension = NULL;
    extension = strrchr(filename, ext);
    return extension;
}

Image::Image(char *src, float x, float y, float width, float height, float opacity, bool responsive, bool hide_overflow, bool circle_mask, char *fit, char *align, int z_index)
{
    this->x = x;
    this->y = y;
    this->width = width;
    this->height = height;
    this->opacity = opacity;
    this->z_index = z_index;
    this->align = parse_alignment(align);
    this->fit = parse_fit(fit);
    this->responsive = responsive;
    this->hide_overflow = hide_overflow;
    this->circle_mask = circle_mask;

    const char *ext = get_file_extension(src);
    if (strcmp(ext, ".png") == 0)
    {
        this->image = cairo_image_surface_create_from_png(src);
    }
    else if (strcmp(ext, ".jpg") == 0 || strcmp(ext, ".jpeg") == 0)
    {
        this->image = cairo_image_surface_create_from_jpeg(src);
    }
}

Image::~Image()
{
    if (this->image != nullptr)
    {
        cairo_surface_destroy(this->image);
    }
}

Image::Image(const Image &element)
{
    this->copy(element);
    this->width = element.width;
    this->height = element.height;
    this->fit = element.fit;
    this->hide_overflow = element.hide_overflow;
    this->circle_mask = element.circle_mask;
    this->image = element.image;
    // if (element.image != nullptr) {
    //     this->image = cairo_surface_reference(element.image);
    // } else {
    //     this->image = nullptr;
    // }
}

void Image::draw(cairo_t *cr, float t)
{
    // compute default attributes
    Attributes attributes;
    attributes["scale"] = 1.;
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
    float scale = attributes["scale"];
    float x_copy = x;
    float y_copy = y;

    // return conditions
    if (width <= 0 || height <= 0 || opacity == 0)
        return;

    // fit image to frame
    float w = cairo_image_surface_get_width(this->image);
    float h = cairo_image_surface_get_height(this->image);
    float w_ratio = width / w;
    float h_ratio = height / h;
    float ratio = scale;
    switch (this->fit)
    {
    case layout::fit::cover:
        ratio *= std::max(w_ratio, h_ratio);
        break;
    case layout::fit::contain:
        ratio *= std::min(w_ratio, h_ratio);
        break;
    }
    w *= ratio;
    h *= ratio;
    adapt_to_alignment(this->align, &x, &y, &w, &h);

    cairo_translate(cr, x, y);
    cairo_scale(cr, ratio, ratio);

    // draw
    cairo_set_source_surface(cr, this->image, 0, 0);

    // scale back
    cairo_scale(cr, 1. / ratio, 1. / ratio);
    cairo_translate(cr, -x, -y);

    // clip if overflow is hidden
    if (this->hide_overflow)
    {
        adapt_to_alignment(this->align, &x_copy, &y_copy, &width, &height);
        cairo_rectangle(cr, x_copy, y_copy, width, height);
        cairo_clip(cr);
    }

    if (this->circle_mask)
    {
        cairo_arc(
            cr, x_copy + width / 2, y_copy + height / 2, width / 2, 0,
            2 * M_PI);
        cairo_clip(cr);
    }

    // paint with opacity if needed
    if (opacity < 1)
    {
        cairo_paint_with_alpha(cr, opacity);
    }
    else
    {
        cairo_paint(cr);
    }
    cairo_reset_clip(cr);
}
