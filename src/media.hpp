#include <cairo.h>
#include "element.hpp"
#include "utils.hpp"

class Image : public Element
{
public:
    Image(char *src, float x, float y, float width, float height, float opacity, bool responsive, bool hide_overflow, bool circle_mask, char *fit, char *align, int z_index);
    Image(const Image &element);
    Image *clone() override { return new Image(*this); }

    void draw(cairo_t *cr, float t) override;

private:
    float width;
    float height;
    bool hide_overflow;
    bool circle_mask;
    cairo_surface_t *image;
    layout::fit fit;
};
