#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include <memory>

namespace layout {

// Basic types
using Length = double;
using Color = uint32_t;

// Point and rectangle structures
struct Point {
    double x, y;
    
    Point() : x(0), y(0) {}
    Point(double x, double y) : x(x), y(y) {}
    
    Point operator+(const Point& other) const { return Point(x + other.x, y + other.y); }
    Point operator-(const Point& other) const { return Point(x - other.x, y - other.y); }
    Point operator*(double factor) const { return Point(x * factor, y * factor); }
    Point operator/(double factor) const { return Point(x / factor, y / factor); }
    
    bool operator==(const Point& other) const { return x == other.x && y == other.y; }
    bool operator!=(const Point& other) const { return !(*this == other); }
};

struct Size {
    double width, height;
    
    Size() : width(0), height(0) {}
    Size(double width, double height) : width(width), height(height) {}
    
    Size operator+(const Size& other) const { return Size(width + other.width, height + other.height); }
    Size operator-(const Size& other) const { return Size(width - other.width, height - other.height); }
    Size operator*(double factor) const { return Size(width * factor, height * factor); }
    Size operator/(double factor) const { return Size(width / factor, height / factor); }
    
    bool operator==(const Size& other) const { return width == other.width && height == other.height; }
    bool operator!=(const Size& other) const { return !(*this == other); }
    
    bool isEmpty() const { return width <= 0 || height <= 0; }
    double area() const { return width * height; }
};

struct Rect {
    double x, y, width, height;
    
    Rect() : x(0), y(0), width(0), height(0) {}
    Rect(double x, double y, double width, double height) : x(x), y(y), width(width), height(height) {}
    Rect(const Point& origin, const Size& size) : x(origin.x), y(origin.y), width(size.width), height(size.height) {}
    
    Point origin() const { return Point(x, y); }
    Size size() const { return Size(width, height); }
    
    Point topLeft() const { return Point(x, y); }
    Point topRight() const { return Point(x + width, y); }
    Point bottomLeft() const { return Point(x, y + height); }
    Point bottomRight() const { return Point(x + width, y + height); }
    
    Point center() const { return Point(x + width / 2, y + height / 2); }
    
    double left() const { return x; }
    double right() const { return x + width; }
    double top() const { return y; }
    double bottom() const { return y + height; }
    
    bool contains(const Point& point) const {
        return point.x >= x && point.x <= x + width && point.y >= y && point.y <= y + height;
    }
    
    bool contains(const Rect& other) const {
        return x <= other.x && y <= other.y && 
               x + width >= other.x + other.width && 
               y + height >= other.y + other.height;
    }
    
    bool intersects(const Rect& other) const {
        return x < other.x + other.width && x + width > other.x &&
               y < other.y + other.height && y + height > other.y;
    }
    
    Rect intersection(const Rect& other) const {
        double left = std::max(x, other.x);
        double top = std::max(y, other.y);
        double right = std::min(x + width, other.x + other.width);
        double bottom = std::min(y + height, other.y + other.height);
        
        if (left < right && top < bottom) {
            return Rect(left, top, right - left, bottom - top);
        }
        return Rect();
    }
    
    Rect unionRect(const Rect& other) const {
        if (isEmpty()) return other;
        if (other.isEmpty()) return *this;
        
        double left = std::min(x, other.x);
        double top = std::min(y, other.y);
        double right = std::max(x + width, other.x + other.width);
        double bottom = std::max(y + height, other.y + other.height);
        
        return Rect(left, top, right - left, bottom - top);
    }
    
    bool isEmpty() const { return width <= 0 || height <= 0; }
    
    Rect operator+(const Point& offset) const { return Rect(x + offset.x, y + offset.y, width, height); }
    Rect operator-(const Point& offset) const { return Rect(x - offset.x, y - offset.y, width, height); }
    
    bool operator==(const Rect& other) const {
        return x == other.x && y == other.y && width == other.width && height == other.height;
    }
    bool operator!=(const Rect& other) const { return !(*this == other); }
};

// Edge insets for margins, padding, borders
struct EdgeInsets {
    double top, right, bottom, left;
    
    EdgeInsets() : top(0), right(0), bottom(0), left(0) {}
    EdgeInsets(double all) : top(all), right(all), bottom(all), left(all) {}
    EdgeInsets(double vertical, double horizontal) : top(vertical), right(horizontal), bottom(vertical), left(horizontal) {}
    EdgeInsets(double top, double right, double bottom, double left) : top(top), right(right), bottom(bottom), left(left) {}
    
    EdgeInsets operator+(const EdgeInsets& other) const {
        return EdgeInsets(top + other.top, right + other.right, bottom + other.bottom, left + other.left);
    }
    
    EdgeInsets operator-(const EdgeInsets& other) const {
        return EdgeInsets(top - other.top, right - other.right, bottom - other.bottom, left - other.left);
    }
    
    EdgeInsets operator*(double factor) const {
        return EdgeInsets(top * factor, right * factor, bottom * factor, left * factor);
    }
    
    double horizontal() const { return left + right; }
    double vertical() const { return top + bottom; }
    
    Size size() const { return Size(horizontal(), vertical()); }
    
    bool operator==(const EdgeInsets& other) const {
        return top == other.top && right == other.right && bottom == other.bottom && left == other.left;
    }
    bool operator!=(const EdgeInsets& other) const { return !(*this == other); }
};

// Transform matrix for 2D transformations
struct Transform {
    double m11, m12, m21, m22, dx, dy;
    
    Transform() : m11(1), m12(0), m21(0), m22(1), dx(0), dy(0) {}
    Transform(double m11, double m12, double m21, double m22, double dx, double dy)
        : m11(m11), m12(m12), m21(m21), m22(m22), dx(dx), dy(dy) {}
    
    static Transform identity() { return Transform(); }
    
    static Transform translation(double dx, double dy) {
        return Transform(1, 0, 0, 1, dx, dy);
    }
    
    static Transform scale(double sx, double sy) {
        return Transform(sx, 0, 0, sy, 0, 0);
    }
    
    static Transform rotation(double angle) {
        double cos_a = std::cos(angle);
        double sin_a = std::sin(angle);
        return Transform(cos_a, -sin_a, sin_a, cos_a, 0, 0);
    }
    
    Transform operator*(const Transform& other) const {
        return Transform(
            m11 * other.m11 + m12 * other.m21,
            m11 * other.m12 + m12 * other.m22,
            m21 * other.m11 + m22 * other.m21,
            m21 * other.m12 + m22 * other.m22,
            dx * other.m11 + dy * other.m21 + other.dx,
            dx * other.m12 + dy * other.m22 + other.dy
        );
    }
    
    Point transform(const Point& point) const {
        return Point(
            m11 * point.x + m21 * point.y + dx,
            m12 * point.x + m22 * point.y + dy
        );
    }
    
    Rect transform(const Rect& rect) const {
        Point p1 = transform(rect.topLeft());
        Point p2 = transform(rect.topRight());
        Point p3 = transform(rect.bottomLeft());
        Point p4 = transform(rect.bottomRight());
        
        double left = std::min({p1.x, p2.x, p3.x, p4.x});
        double top = std::min({p1.y, p2.y, p3.y, p4.y});
        double right = std::max({p1.x, p2.x, p3.x, p4.x});
        double bottom = std::max({p1.y, p2.y, p3.y, p4.y});
        
        return Rect(left, top, right - left, bottom - top);
    }
    
    bool isIdentity() const {
        return m11 == 1 && m12 == 0 && m21 == 0 && m22 == 1 && dx == 0 && dy == 0;
    }
    
    bool operator==(const Transform& other) const {
        return m11 == other.m11 && m12 == other.m12 && m21 == other.m21 && 
               m22 == other.m22 && dx == other.dx && dy == other.dy;
    }
    bool operator!=(const Transform& other) const { return !(*this == other); }
};

// Z-index and stacking context
using ZIndex = int32_t;

// Font metrics
struct FontMetrics {
    double ascent;
    double descent;
    double leading;
    double xHeight;
    double capHeight;
    
    FontMetrics() : ascent(0), descent(0), leading(0), xHeight(0), capHeight(0) {}
    FontMetrics(double ascent, double descent, double leading, double xHeight, double capHeight)
        : ascent(ascent), descent(descent), leading(leading), xHeight(xHeight), capHeight(capHeight) {}
    
    double lineHeight() const { return ascent + descent + leading; }
    double baseline() const { return ascent; }
};

// Text run for text layout
struct TextRun {
    std::string text;
    Point position;
    Size size;
    FontMetrics metrics;
    Color color;
    bool isWhitespace;
    
    TextRun() : position(0, 0), size(0, 0), color(0), isWhitespace(false) {}
    TextRun(const std::string& text, const Point& position, const Size& size, const FontMetrics& metrics, Color color, bool isWhitespace = false)
        : text(text), position(position), size(size), metrics(metrics), color(color), isWhitespace(isWhitespace) {}
};

// Layout constraints
struct LayoutConstraints {
    Size minSize;
    Size maxSize;
    
    LayoutConstraints() : minSize(0, 0), maxSize(std::numeric_limits<double>::max(), std::numeric_limits<double>::max()) {}
    LayoutConstraints(const Size& minSize, const Size& maxSize) : minSize(minSize), maxSize(maxSize) {}
    
    bool isValid() const { return minSize.width <= maxSize.width && minSize.height <= maxSize.height; }
    
    Size constrain(const Size& size) const {
        return Size(
            std::max(minSize.width, std::min(maxSize.width, size.width)),
            std::max(minSize.height, std::min(maxSize.height, size.height))
        );
    }
    
    LayoutConstraints tighten(const Size& size) const {
        return LayoutConstraints(
            Size(std::max(minSize.width, size.width), std::max(minSize.height, size.height)),
            Size(std::min(maxSize.width, size.width), std::min(maxSize.height, size.height))
        );
    }
};

} // namespace layout
