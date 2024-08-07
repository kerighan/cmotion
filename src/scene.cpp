#include <vector>
#include <iostream>
#include <algorithm>
#include <cstring>
#include <cairo.h>
#include <math.h>
#include "cairo_jpg.hpp"
#include "threading.hpp"
#include "scene.hpp"
#include <cairo-svg.h>

// ============================================================================
// Scene
// ============================================================================
Scene::Scene(int width, int height, const std::string color)
{
    this->width = width;
    this->height = height;
    if (color.length() == 0)
    {
        this->color = hex_to_rgb("#FFFFFF");
        this->background = false;
    }
    else
    {
        this->color = hex_to_rgb(color);
        this->background = true;
    }
}

Scene::~Scene() {
    for (auto elem : layers) {
        delete elem;
    }
}

Scene::Scene(const Scene& other) : width(other.width), height(other.height), color(other.color), background(other.background) {
    layers.reserve(other.layers.size());
    for (Element* elem : other.layers) {
        layers.push_back(elem->clone());
    }
}

Scene& Scene::operator=(const Scene& other) {
    if (this != &other) {
        for (Element* elem : layers) {
            delete elem;
        }
        layers.clear();
        layers.reserve(other.layers.size());
        for (Element* elem : other.layers) {
            layers.push_back(elem->clone());
        }
        width = other.width;
        height = other.height;
        color = other.color;
        background = other.background;
    }
    return *this;
}

void Scene::add(Element *element)
{
    Element *elem = element->clone();
    elem->set_screen(this->width, this->height);
    this->layers.push_back(elem);
}

void Scene::resize(int width, int height)
{
    this->width = width;
    this->height = height;
    for (auto &layer : this->layers)
    {
        layer->set_screen(width, height);
    }
}

float Scene::get_end()
{
    float max = 0;
    for (auto &layer : this->layers)
    {
        float tmp = layer->get_end();
        if (tmp > max)
            max = tmp;
    }
    return max;
}

void Scene::at(cairo_t *cr, float t)
{
    // sort layers
    std::sort(this->layers.begin(), this->layers.end(), z_sort);

    // get background color
    float r = std::get<0>(this->color);
    float g = std::get<1>(this->color);
    float b = std::get<2>(this->color);

    // add background
    if (this->background)
    {
        cairo_rectangle(cr, 0, 0, this->width, this->height);
        cairo_set_source_rgb(cr, r, g, b);
        cairo_fill(cr);
    }

    // render frame
    for (size_t j = 0; j < this->layers.size(); j++)
    {
        this->layers[j]->draw(cr, t);
    }
}

void Scene::save(std::string filename, float t)
{
    // create surface and context
    cairo_surface_t *surface;
    cairo_t *cr;
    surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, this->width, this->height);
    cr = cairo_create(surface);
    cairo_set_antialias(cr, CAIRO_ANTIALIAS_BEST);

    // fill surface with content
    this->at(cr, t);

    // render frame
    // if (extension == "")
    std::string ext = filename.substr(filename.find_last_of(".") + 1);

    if (ext == "jpg" || ext == "jpeg")
        cairo_image_surface_write_to_jpeg(surface, filename.c_str(), 100);
    else if (ext == "png")
        cairo_surface_write_to_png(surface, filename.c_str());

    // free surface and context
    cairo_destroy(cr);
    cairo_surface_destroy(surface);
}

void Scene::to_svg(std::string filename, float t)
{
    // create surface and context
    cairo_surface_t *surface;
    cairo_t *cr;
    // surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, this->width, this->height);
    surface = cairo_svg_surface_create(filename.c_str(), this->width, this->height);
    cr = cairo_create(surface);
    cairo_set_antialias(cr, CAIRO_ANTIALIAS_BEST);

    // fill surface with content
    this->at(cr, t);

    // free surface and context
    cairo_destroy(cr);
    cairo_surface_destroy(surface);
}

void Scene::render(std::string filename, int fps, int quality, int antialias)
{
    // extension
    const std::string ext = std::string(".jpg");
    filename += "/";

    // choose the right antialias
    _cairo_antialias antialias_value;
    switch (antialias)
    {
    case 0:
        antialias_value = CAIRO_ANTIALIAS_NONE;
        break;
    case 1:
        antialias_value = CAIRO_ANTIALIAS_GRAY;
        break;
    case 2:
        antialias_value = CAIRO_ANTIALIAS_FAST;
        break;
    case 3:
        antialias_value = CAIRO_ANTIALIAS_GOOD;
        break;
    case 4:
        antialias_value = CAIRO_ANTIALIAS_SUBPIXEL;
        break;
    default:
        antialias_value = CAIRO_ANTIALIAS_BEST;
        break;
    }

    // sort layers
    std::sort(this->layers.begin(), this->layers.end(), z_sort);

    // number of frames
    int n_frames = fps * this->get_end();
    std::cout << n_frames << " frames" << std::endl;

    // parallel rendering
    PARALLEL_FOR_BEGIN(n_frames)
    {
        // get time from frame
        float t = (float)i / fps;

        // create master surface to draw onto
        cairo_surface_t *surface;
        cairo_t *cr;
        surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, this->width, this->height);
        cr = cairo_create(surface);
        cairo_set_antialias(cr, antialias_value);

        // fill surface with content
        this->at(cr, t);

        // render frame
        std::string framename = get_filename(filename, i, ext);
        char *framename_char = const_cast<char *>(framename.c_str());
        cairo_image_surface_write_to_jpeg(surface, framename_char, quality);

        // free surface and context
        cairo_destroy(cr);
        cairo_surface_destroy(surface);
    }
    PARALLEL_FOR_END();
}

size_t Scene::get_number_of_layers()
{
    return this->layers.size();
}
