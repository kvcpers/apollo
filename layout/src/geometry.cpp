#include "layout/geometry.h"

namespace layout {

// Point implementation
Point::Point() : x(0), y(0) {}

Point::Point(float x, float y) : x(x), y(y) {}

Point::~Point() {}

float Point::get_x() const {
    return x;
}

float Point::get_y() const {
    return y;
}

void Point::set_x(float x) {
    this->x = x;
}

void Point::set_y(float y) {
    this->y = y;
}

// Size implementation
Size::Size() : width(0), height(0) {}

Size::Size(float width, float height) : width(width), height(height) {}

Size::~Size() {}

float Size::get_width() const {
    return width;
}

float Size::get_height() const {
    return height;
}

void Size::set_width(float width) {
    this->width = width;
}

void Size::set_height(float height) {
    this->height = height;
}

// Rect implementation
Rect::Rect() : x(0), y(0), width(0), height(0) {}

Rect::Rect(float x, float y, float width, float height) 
    : x(x), y(y), width(width), height(height) {}

Rect::~Rect() {}

float Rect::get_x() const {
    return x;
}

float Rect::get_y() const {
    return y;
}

float Rect::get_width() const {
    return width;
}

float Rect::get_height() const {
    return height;
}

void Rect::set_x(float x) {
    this->x = x;
}

void Rect::set_y(float y) {
    this->y = y;
}

void Rect::set_width(float width) {
    this->width = width;
}

void Rect::set_height(float height) {
    this->height = height;
}

bool Rect::contains(const Point& point) const {
    return point.get_x() >= x && point.get_x() < x + width &&
           point.get_y() >= y && point.get_y() < y + height;
}

bool Rect::intersects(const Rect& other) const {
    return !(x + width <= other.x || other.x + other.width <= x ||
             y + height <= other.y || other.y + other.height <= y);
}

} // namespace layout
