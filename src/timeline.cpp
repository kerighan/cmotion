#include <iostream>
#include "timeline.hpp"


Timeline::Timeline(std::vector<float> keyframes, float delay){
    this->n_keyframes = keyframes.size();
    for (size_t i = 0; i < this->n_keyframes; i++){
        float keyframe = keyframes[i];
        if (i == 0){
            this->start = keyframe;
        } else if (i == this->n_keyframes - 1){
            this->end = keyframe;
        }
        this->keyframes.push_back(keyframe);
    }
    this->delay = delay;
}

float Timeline::get_duration(){
    return this->end - this->start;
}

float Timeline::get_start(){
    return this->start + this->delay;
}

float Timeline::get_end(){
    return this->end + this->delay;
}

void Timeline::at(Attributes& attributes){
    // offset time by internal delay and keep
    float t = attributes["t"];
    t -= delay;
    if (t < this->start)
        t = this->start;
    else if (t > this->end)
        t = this->end;

    // get appropriate keyframe index
    size_t index = 0;
    float start = 0;
    float end = 0;
    for (size_t i = 0; i < this->n_keyframes - 1; i++){
        start = this->keyframes[i];
        end = this->keyframes[i + 1];
        if (start <= t && t <= end){
            index = i;
            break;
        }
    }

    float interp = (t - start) / (end - start);
    for(auto& it: this->attributes) {
        std::string key = it.first;
        Attribute attr = it.second[index];

        int interpType = std::get<0>(attr);
        float startVal = std::get<1>(attr);
        float endVal = std::get<2>(attr);
        float deltaVal = endVal - startVal;
        float attrVal;
        float tmp;

        switch (interpType){
            case 0:  // constant value
                attrVal = 0;
                break;
            case 1:  // linear interpolation
                attrVal = interp;
                break;
            case 2:  // cubic in
                attrVal = interp * interp * interp;
                break;
            case 3:  // cubic out
                tmp = (interp - 1);
                attrVal = tmp * tmp * tmp + 1;
                break;
            case 4:  // cubic in out
                if (interp < .5){
                    attrVal = 4 * interp * interp * interp;
                } else {
                    tmp = 2 * interp - 2;
                    attrVal = .5 * tmp * tmp * tmp + 1;
                }
                break;
            default:
                attrVal = 0;
                break;
        }
        attributes[key] = startVal + attrVal * deltaVal;
    }
}

Attributes Timeline::get(float t){
    Attributes attributes;
    attributes["t"] = t;
    this->at(attributes);
    return attributes;
}

// ============================================================================
// Easing functions
// ============================================================================

void Timeline::animate(int type, std::string attr, float start, float end){
    this->attributes[attr].push_back(std::make_tuple(type, start, end));
}

void Timeline::constant(std::string attr, float start){
    this->animate(0, attr, start, start);
}

void Timeline::linear(std::string attr, float start, float end){
    this->animate(1, attr, start, end);
}

// Cubic functions

void Timeline::cubic_in(std::string attr, float start, float end){
    this->animate(2, attr, start, end);
}

void Timeline::cubic_out(std::string attr, float start, float end){
    this->animate(3, attr, start, end);
}

void Timeline::cubic_in_out(std::string attr, float start, float end){
    this->animate(4, attr, start, end);
}