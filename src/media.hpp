#include <cairo.h>
#include "element.hpp"
#include "utils.hpp"

class Image : public Element
{
public:
    // Constructor to initialize an Image with various parameters
    Image(char *src, float x, float y, float width, float height, float opacity, bool responsive, bool hide_overflow, bool circle_mask, char *fit, char *align, int z_index);
    // Copy constructor for deep copying resources or proper reference management
    Image(const Image &element);
    // Destructor to release any allocated resources
    virtual ~Image();

    // Rule of Five: If you're using C++11 or newer, consider explicitly defining or deleting these as well
    Image(Image&& other) = delete; // Move constructor
    Image& operator=(const Image& other) = delete; // Copy assignment operator
    Image& operator=(Image&& other) = delete; // Move assignment operator

    Image *clone() override { return new Image(*this); }
    void draw(cairo_t *cr, float t) override;

private:
    float width;
    float height;
    bool hide_overflow;
    bool circle_mask;
    cairo_surface_t *image; // Resource that needs managed destruction
    layout::fit fit;
};
