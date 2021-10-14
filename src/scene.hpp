#include <vector>
#include "element.hpp"
#include "utils.hpp"

class Scene
{
public:
    Scene(int width, int height, std::string color);
    // ~Scene();

    void add(Element *element);
    void render(std::string filename, int fps, int quality, int antialias);
    void at(cairo_t *cr, float t);
    void save(std::string filename, float t);
    void to_svg(std::string filename, float t);
    void resize(int width, int height);
    float get_end();
    size_t get_number_of_layers();

private:
    std::vector<Element *> layers;
    Color color;
    int width;
    int height;
};
