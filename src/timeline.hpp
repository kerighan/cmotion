#ifndef TIMELINE_H_
#define TIMELINE_H_
#include <vector>
#include <unordered_map>


using Attribute = std::tuple<int, float, float>;
using Attributes = std::unordered_map<std::string, float>;
using AttributesVector = std::unordered_map<std::string, std::vector<Attribute>>;


class Timeline
{
public:
    Timeline(std::vector<float> keyframes, float delay);

    void at(Attributes& attributes);
    Attributes get(float t);

    float get_duration();
    float get_start();
    float get_end();

    void animate(int type, std::string attr, float start, float end);
    void constant(std::string attr, float start);
    void linear(std::string attr, float start, float end);
    void cubic_in(std::string attr, float start, float end);
    void cubic_out(std::string attr, float start, float end);
    void cubic_in_out(std::string attr, float start, float end);

private:
    float delay;
    float start;
    float end;
    size_t n_keyframes;
    std::vector<float> keyframes;
    AttributesVector attributes;
};

#endif