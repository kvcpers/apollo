#include "layout/viewport.h"

namespace layout {

Viewport::Viewport() : width(0), height(0), scale(1.0f) {}

Viewport::~Viewport() {}

void Viewport::set_size(float w, float h) {
    width = w;
    height = h;
}

float Viewport::get_width() const {
    return width;
}

float Viewport::get_height() const {
    return height;
}

void Viewport::set_scale(float s) {
    scale = s;
}

float Viewport::get_scale() const {
    return scale;
}

void Viewport::set_viewport_units(float vw, float vh, float vmin, float vmax) {
    viewport_width = vw;
    viewport_height = vh;
    viewport_min = vmin;
    viewport_max = vmax;
}

float Viewport::get_viewport_width() const {
    return viewport_width;
}

float Viewport::get_viewport_height() const {
    return viewport_height;
}

float Viewport::get_viewport_min() const {
    return viewport_min;
}

float Viewport::get_viewport_max() const {
    return viewport_max;
}

} // namespace layout
