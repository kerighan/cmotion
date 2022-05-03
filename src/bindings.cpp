#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "scene.hpp"
#include "element.hpp"
#include "shape.hpp"
#include "media.hpp"
#include "text.hpp"
#include "timeline.hpp"

namespace py = pybind11;

PYBIND11_MODULE(cmotion, m)
{
    py::module_ m_text = m.def_submodule("text", "Text submodule");
    py::module_ m_shape = m.def_submodule("shape", "Shape submodule");
    py::module_ m_media = m.def_submodule("media", "Media submodule");
    py::module_ m_anim = m.def_submodule("animation", "Animation submodule");

    py::class_<Scene>(m, "Scene")
        .def(py::init<int, int, std::string>(),
             py::arg("width") = 1000,
             py::arg("height") = 1000,
             py::arg("color") = "")
        .def("add", &Scene::add)
        .def("get_end", &Scene::get_end)
        .def("resize", &Scene::resize)
        .def("save", &Scene::save,
             py::arg("filename"),
             py::arg("t") = 0)
        .def("to_svg", &Scene::to_svg,
             py::arg("filename"),
             py::arg("t") = 0)
        .def("render", &Scene::render,
             py::arg("filename"),
             py::arg("fps") = 25,
             py::arg("quality") = 90,
             py::arg("antialias") = 5)
        .def("get_number_of_layers", &Scene::get_number_of_layers);
    py::class_<Element>(m, "Element")
        .def(py::init<>())
        .def("add", &Element::add)
        .def("set_position", &Element::set_position)
        .def("set_screen", &Element::set_screen)
        .def("on_resize", &Element::on_resize)
        .def("get_end", &Element::get_end);

    // text submodule
    py::class_<Font>(m_text, "Font")
        .def(py::init<char *>(), py::arg("font_family"));

    py::class_<Text, Element>(m_text, "Text")
        .def(py::init<std::string &, float, float, float, float, float, float, char *, std::string, float, bool, char *, int, Font *>(),
             py::arg("txt"),
             py::arg("x") = 50,
             py::arg("y") = 50,
             py::arg("width") = 100,
             py::arg("size") = 5,
             py::arg("line_height") = .5,
             py::arg("space_size") = .5,
             py::arg("font_family") = "OpenSans-Light.ttf",
             py::arg("color") = "#FFFFFF",
             py::arg("opacity") = 1,
             py::arg("responsive") = true,
             py::arg("align") = "center",
             py::arg("z_index") = 0,
             py::arg("font") = nullptr)
        .def("animate_words", &Text::animate_words)
        .def("animate_lines", &Text::animate_lines)
        .def("get_lines_bbox", &Text::get_lines_bbox);

    // shape submodule
    py::class_<Circle, Element>(m_shape, "Circle")
        .def(py::init<float, float, float, std::string, float, bool, char *, int, float>(),
             py::arg("x") = 50,
             py::arg("y") = 50,
             py::arg("radius") = 1,
             py::arg("color") = "#FFFFFF",
             py::arg("opacity") = 1,
             py::arg("responsive") = true,
             py::arg("align") = "center",
             py::arg("z_index") = 0,
             py::arg("stroke_width") = 0);
    py::class_<Rectangle, Element>(m_shape, "Rectangle")
        .def(py::init<float, float, float, float, std::string, float, float, bool, char *, int, float>(),
             py::arg("x") = 50,
             py::arg("y") = 50,
             py::arg("width") = 50,
             py::arg("height") = 50,
             py::arg("color") = "#FFFFFF",
             py::arg("opacity") = 1,
             py::arg("border_radius") = 0,
             py::arg("responsive") = true,
             py::arg("align") = "center",
             py::arg("z_index") = 0,
             py::arg("stroke_width") = 0);

    py::class_<Line, Element>(m_shape, "Line")
        .def(py::init<std::vector<float> &, std::vector<float> &, float, int, std::string, float, double, bool, char *, int>(),
             py::arg("x"),
             py::arg("y"),
             py::arg("stroke_width") = .5,
             py::arg("cap") = 0,
             py::arg("color") = "#FFFFFF",
             py::arg("opacity") = 1,
             py::arg("dotted") = 0,
             py::arg("responsive") = true,
             py::arg("align") = "center",
             py::arg("z_index") = 0);

    py::class_<Curve, Element>(m_shape, "Curve")
        .def(py::init<std::vector<float> &, std::vector<float> &, float, int, std::string, float, double, bool, char *, int, float>(),
             py::arg("x"),
             py::arg("y"),
             py::arg("stroke_width") = .5,
             py::arg("cap") = 0,
             py::arg("color") = "#FFFFFF",
             py::arg("opacity") = 1,
             py::arg("dotted") = 0,
             py::arg("responsive") = true,
             py::arg("align") = "center",
             py::arg("z_index") = 0,
             py::arg("curvature") = 0.5);

    // media submodule
    py::class_<Image, Element>(m_media, "Image")
        .def(py::init<char *, float, float, float, float, float, bool, bool, bool, char *, char *, int>(),
             py::arg("src"),
             py::arg("x") = 50,
             py::arg("y") = 50,
             py::arg("width") = 100,
             py::arg("height") = 100,
             py::arg("opacity") = 1,
             py::arg("responsive") = true,
             py::arg("hide_overflow") = true,
             py::arg("circle_mask") = false,
             py::arg("fit") = "contain",
             py::arg("align") = "center",
             py::arg("z_index") = 0);

    // animation submodule
    py::class_<Timeline>(m_anim, "Timeline")
        .def(py::init<std::vector<float>, float>(), py::arg("keyframes"), py::arg("delay") = 0)
        .def("get", &Timeline::get)
        .def("get_duration", &Timeline::get_duration)
        .def("get_start", &Timeline::get_start)
        .def("get_end", &Timeline::get_end)
        .def("linear", &Timeline::linear)
        .def("constant", &Timeline::constant)
        .def("cubic_in", &Timeline::cubic_in)
        .def("cubic_out", &Timeline::cubic_out)
        .def("cubic_in_out", &Timeline::cubic_in_out);
}