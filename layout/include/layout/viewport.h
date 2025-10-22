#pragma once

#include "types.h"
#include <memory>

namespace layout {

// Viewport for layout calculations
class Viewport {
public:
    Viewport();
    explicit Viewport(const Rect& rect);
    ~Viewport();

    // Get viewport rectangle
    const Rect& rect() const { return rect_; }
    void setRect(const Rect& rect) { rect_ = rect; }

    // Get viewport size
    const Size& size() const { return rect_.size(); }
    void setSize(const Size& size) { rect_ = Rect(rect_.x, rect_.y, size.width, size.height); }

    // Get viewport width
    double width() const { return rect_.width; }
    void setWidth(double width) { rect_.width = width; }

    // Get viewport height
    double height() const { return rect_.height; }
    void setHeight(double height) { rect_.height = height; }

    // Get viewport position
    const Point& position() const { return rect_.origin(); }
    void setPosition(const Point& position) { rect_ = Rect(position, rect_.size()); }

    // Get viewport x
    double x() const { return rect_.x; }
    void setX(double x) { rect_.x = x; }

    // Get viewport y
    double y() const { return rect_.y; }
    void setY(double y) { rect_.y = y; }

    // Check if point is in viewport
    bool contains(const Point& point) const { return rect_.contains(point); }

    // Check if rectangle intersects with viewport
    bool intersects(const Rect& rect) const { return rect_.intersects(rect); }

    // Get intersection with viewport
    Rect intersection(const Rect& rect) const { return rect_.intersection(rect); }

    // Get union with viewport
    Rect unionRect(const Rect& rect) const { return rect_.unionRect(rect); }

    // Transform viewport
    void transform(const Transform& transform) { rect_ = transform.transform(rect_); }

    // Reset viewport
    void reset() { rect_ = Rect(0, 0, 0, 0); }

    // Copy constructor and assignment
    Viewport(const Viewport& other);
    Viewport& operator=(const Viewport& other);

    // Move constructor and assignment
    Viewport(Viewport&& other) noexcept;
    Viewport& operator=(Viewport&& other) noexcept;

private:
    Rect rect_;
};

} // namespace layout
