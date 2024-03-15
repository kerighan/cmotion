#include <vector>
#include "element.hpp"
#include "utils.hpp"

class Scene
{
public:
    Scene(int width, int height, std::string color);
    ~Scene(); // Destructor to clean up dynamically allocated resources

    // Rule of Five: Copy constructor and copy assignment operator
    Scene(const Scene& other); // Copy constructor
    Scene& operator=(const Scene& other); // Copy assignment operator

    // Move constructor and move assignment operator for efficient transfers
    Scene(Scene&& other) noexcept; // Move constructor
    Scene& operator=(Scene&& other) noexcept; // Move assignment operator

    void add(Element* element);
    void render(std::string filename, int fps, int quality, int antialias);
    void at(cairo_t *cr, float t);
    void save(std::string filename, float t);
    void to_svg(std::string filename, float t);
    void resize(int width, int height);
    float get_end();
    size_t get_number_of_layers();

private:
    std::vector<Element*> layers; // Elements in the scene
    Color color; // Background color
    int width, height; // Dimensions of the scene
    bool background; // Indicates whether a background should be drawn
};
