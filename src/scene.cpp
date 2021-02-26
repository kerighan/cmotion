#include <vector>
#include <iostream>
#include <algorithm>
#include <cstring>
#include <cairo.h>
#include <math.h>
#include "cairo_jpg.hpp"
#include "threading.hpp"
#include "scene.hpp"


// ============================================================================
// Scene
// ============================================================================
Scene::Scene(int width, int height, const std::string color){
    this->width = width;
    this->height = height;
    this->color = hex_to_rgb(color);
}


void Scene::add(Element* element){
    element->set_screen(this->width, this->height);
    element->on_resize();
    this->layers.push_back(element);
}


void Scene::resize(int width, int height){
    this->width = width;
    this->height = height;
    for (auto& layer : this->layers){
        layer->set_screen(width, height);
        layer->on_resize();
    }
}


float Scene::get_end(){
    float max = 0;
    for (auto& layer : this->layers){
        float tmp = layer->get_end();
        if (tmp > max)
            max = tmp;
    }
    return max;
}


void Scene::render(std::string filename, int fps, int quality, int antialias){
    // extension
    const std::string ext = std::string(".jpg");
    filename += "/";

    // choose the right antialias
    _cairo_antialias antialias_value;
    switch (antialias){
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

    // get background color
    float r = std::get<0>(this->color);
    float g = std::get<1>(this->color);
    float b = std::get<2>(this->color);

    // parallel rendering
    PARALLEL_FOR_BEGIN(n_frames){
        // get time from frame
        float t = (float)i / fps;

        // create master surface to draw onto
        cairo_surface_t *surface;
        cairo_t *cr;
        surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, this->width, this->height);
        cr = cairo_create(surface);
        cairo_set_antialias(cr, antialias_value);

        // add background
        cairo_rectangle(cr, 0, 0, this->width, this->height);
        cairo_set_source_rgb(cr, r, g, b);
        cairo_fill(cr);

        // render frame
        for (size_t j = 0; j < this->layers.size(); j++){
            this->layers[j]->draw(cr, t);
        }

        // render frame
        std::string framename = get_filename(filename, i, ext);
        char* framename_char = const_cast<char*>(framename.c_str());
        cairo_image_surface_write_to_jpeg(surface, framename_char, quality);

        // free surface and context
        cairo_destroy(cr);
        cairo_surface_destroy(surface);
    }PARALLEL_FOR_END();
}
