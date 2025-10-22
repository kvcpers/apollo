#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include <memory>
#include <functional>

namespace renderer {

// Basic geometric types
struct Point {
    double x;
    double y;

    Point() : x(0), y(0) {}
    Point(double x, double y) : x(x), y(y) {}

    Point operator+(const Point& other) const { return Point(x + other.x, y + other.y); }
    Point operator-(const Point& other) const { return Point(x - other.x, y - other.y); }
    Point operator*(double factor) const { return Point(x * factor, y * factor); }
    Point operator/(double factor) const { return Point(x / factor, y / factor); }

    Point& operator+=(const Point& other) { x += other.x; y += other.y; return *this; }
    Point& operator-=(const Point& other) { x -= other.x; y -= other.y; return *this; }
    Point& operator*=(double factor) { x *= factor; y *= factor; return *this; }
    Point& operator/=(double factor) { x /= factor; y /= factor; return *this; }

    bool operator==(const Point& other) const { return x == other.x && y == other.y; }
    bool operator!=(const Point& other) const { return !(*this == other); }
};

struct Size {
    double width;
    double height;

    Size() : width(0), height(0) {}
    Size(double width, double height) : width(width), height(height) {}

    Size operator+(const Size& other) const { return Size(width + other.width, height + other.height); }
    Size operator-(const Size& other) const { return Size(width - other.width, height - other.height); }
    Size operator*(double factor) const { return Size(width * factor, height * factor); }
    Size operator/(double factor) const { return Size(width / factor, height / factor); }

    Size& operator+=(const Size& other) { width += other.width; height += other.height; return *this; }
    Size& operator-=(const Size& other) { width -= other.width; height -= other.height; return *this; }
    Size& operator*=(double factor) { width *= factor; height *= factor; return *this; }
    Size& operator/=(double factor) { width /= factor; height /= factor; return *this; }

    bool operator==(const Size& other) const { return width == other.width && height == other.height; }
    bool operator!=(const Size& other) const { return !(*this == other); }

    bool isEmpty() const { return width <= 0 || height <= 0; }
    bool isValid() const { return width >= 0 && height >= 0; }
};

struct Rect {
    Point origin;
    Size size;

    Rect() : origin(), size() {}
    Rect(const Point& origin, const Size& size) : origin(origin), size(size) {}
    Rect(double x, double y, double width, double height) : origin(x, y), size(width, height) {}

    double x() const { return origin.x; }
    double y() const { return origin.y; }
    double width() const { return size.width; }
    double height() const { return size.height; }

    double left() const { return origin.x; }
    double right() const { return origin.x + size.width; }
    double top() const { return origin.y; }
    double bottom() const { return origin.y + size.height; }

    Point topLeft() const { return origin; }
    Point topRight() const { return Point(right(), top()); }
    Point bottomLeft() const { return Point(left(), bottom()); }
    Point bottomRight() const { return Point(right(), bottom()); }

    Point center() const { return Point(origin.x + size.width / 2, origin.y + size.height / 2); }

    bool isEmpty() const { return size.isEmpty(); }
    bool isValid() const { return size.isValid(); }

    bool contains(const Point& point) const {
        return point.x >= left() && point.x <= right() && point.y >= top() && point.y <= bottom();
    }

    bool contains(const Rect& rect) const {
        return rect.left() >= left() && rect.right() <= right() && rect.top() >= top() && rect.bottom() <= bottom();
    }

    bool intersects(const Rect& rect) const {
        return !(rect.left() > right() || rect.right() < left() || rect.top() > bottom() || rect.bottom() < top());
    }

    Rect intersection(const Rect& rect) const {
        if (!intersects(rect)) return Rect();
        
        double left = std::max(this->left(), rect.left());
        double top = std::max(this->top(), rect.top());
        double right = std::min(this->right(), rect.right());
        double bottom = std::min(this->bottom(), rect.bottom());
        
        return Rect(left, top, right - left, bottom - top);
    }

    Rect unionRect(const Rect& rect) const {
        if (isEmpty()) return rect;
        if (rect.isEmpty()) return *this;
        
        double left = std::min(this->left(), rect.left());
        double top = std::min(this->top(), rect.top());
        double right = std::max(this->right(), rect.right());
        double bottom = std::max(this->bottom(), rect.bottom());
        
        return Rect(left, top, right - left, bottom - top);
    }

    Rect& operator+=(const Point& offset) { origin += offset; return *this; }
    Rect& operator-=(const Point& offset) { origin -= offset; return *this; }

    Rect operator+(const Point& offset) const { return Rect(origin + offset, size); }
    Rect operator-(const Point& offset) const { return Rect(origin - offset, size); }

    bool operator==(const Rect& other) const { return origin == other.origin && size == other.size; }
    bool operator!=(const Rect& other) const { return !(*this == other); }
};

// Color types
struct Color {
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;

    Color() : r(0), g(0), b(0), a(255) {}
    Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255) : r(r), g(g), b(b), a(a) {}
    Color(uint32_t rgba) : r((rgba >> 24) & 0xFF), g((rgba >> 16) & 0xFF), b((rgba >> 8) & 0xFF), a(rgba & 0xFF) {}

    uint32_t toRGBA() const { return (r << 24) | (g << 16) | (b << 8) | a; }
    uint32_t toARGB() const { return (a << 24) | (r << 16) | (g << 8) | b; }

    bool operator==(const Color& other) const { return r == other.r && g == other.g && b == other.b && a == other.a; }
    bool operator!=(const Color& other) const { return !(*this == other); }

    static Color transparent() { return Color(0, 0, 0, 0); }
    static Color black() { return Color(0, 0, 0, 255); }
    static Color white() { return Color(255, 255, 255, 255); }
    static Color red() { return Color(255, 0, 0, 255); }
    static Color green() { return Color(0, 255, 0, 255); }
    static Color blue() { return Color(0, 0, 255, 255); }
};

// Matrix types
struct Matrix {
    double m11, m12, m13;
    double m21, m22, m23;
    double m31, m32, m33;

    Matrix() : m11(1), m12(0), m13(0), m21(0), m22(1), m23(0), m31(0), m32(0), m33(1) {}
    Matrix(double m11, double m12, double m13, double m21, double m22, double m23, double m31, double m32, double m33)
        : m11(m11), m12(m12), m13(m13), m21(m21), m22(m22), m23(m23), m31(m31), m32(m32), m33(m33) {}

    static Matrix identity() { return Matrix(); }
    static Matrix translation(double x, double y) { return Matrix(1, 0, x, 0, 1, y, 0, 0, 1); }
    static Matrix scale(double x, double y) { return Matrix(x, 0, 0, 0, y, 0, 0, 0, 1); }
    static Matrix rotation(double angle) {
        double c = std::cos(angle);
        double s = std::sin(angle);
        return Matrix(c, -s, 0, s, c, 0, 0, 0, 1);
    }

    Matrix operator*(const Matrix& other) const {
        return Matrix(
            m11 * other.m11 + m12 * other.m21 + m13 * other.m31,
            m11 * other.m12 + m12 * other.m22 + m13 * other.m32,
            m11 * other.m13 + m12 * other.m23 + m13 * other.m33,
            m21 * other.m11 + m22 * other.m21 + m23 * other.m31,
            m21 * other.m12 + m22 * other.m22 + m23 * other.m32,
            m21 * other.m13 + m22 * other.m23 + m23 * other.m33,
            m31 * other.m11 + m32 * other.m21 + m33 * other.m31,
            m31 * other.m12 + m32 * other.m22 + m33 * other.m32,
            m31 * other.m13 + m32 * other.m23 + m33 * other.m33
        );
    }

    Point transform(const Point& point) const {
        double x = m11 * point.x + m12 * point.y + m13;
        double y = m21 * point.x + m22 * point.y + m23;
        double w = m31 * point.x + m32 * point.y + m33;
        return Point(x / w, y / w);
    }

    Rect transform(const Rect& rect) const {
        Point topLeft = transform(rect.topLeft());
        Point topRight = transform(rect.topRight());
        Point bottomLeft = transform(rect.bottomLeft());
        Point bottomRight = transform(rect.bottomRight());
        
        double left = std::min({topLeft.x, topRight.x, bottomLeft.x, bottomRight.x});
        double top = std::min({topLeft.y, topRight.y, bottomLeft.y, bottomRight.y});
        double right = std::max({topLeft.x, topRight.x, bottomLeft.x, bottomRight.x});
        double bottom = std::max({topLeft.y, topRight.y, bottomLeft.y, bottomRight.y});
        
        return Rect(left, top, right - left, bottom - top);
    }

    Matrix& operator*=(const Matrix& other) { return *this = *this * other; }

    bool operator==(const Matrix& other) const {
        return m11 == other.m11 && m12 == other.m12 && m13 == other.m13 &&
               m21 == other.m21 && m22 == other.m22 && m23 == other.m23 &&
               m31 == other.m31 && m32 == other.m32 && m33 == other.m33;
    }
    bool operator!=(const Matrix& other) const { return !(*this == other); }
};

// Edge insets
struct EdgeInsets {
    double top;
    double right;
    double bottom;
    double left;

    EdgeInsets() : top(0), right(0), bottom(0), left(0) {}
    EdgeInsets(double top, double right, double bottom, double left) : top(top), right(right), bottom(bottom), left(left) {}
    EdgeInsets(double vertical, double horizontal) : top(vertical), right(horizontal), bottom(vertical), left(horizontal) {}
    EdgeInsets(double all) : top(all), right(all), bottom(all), left(all) {}

    double horizontal() const { return left + right; }
    double vertical() const { return top + bottom; }

    EdgeInsets operator+(const EdgeInsets& other) const {
        return EdgeInsets(top + other.top, right + other.right, bottom + other.bottom, left + other.left);
    }
    EdgeInsets operator-(const EdgeInsets& other) const {
        return EdgeInsets(top - other.top, right - other.right, bottom - other.bottom, left - other.left);
    }
    EdgeInsets operator*(double factor) const {
        return EdgeInsets(top * factor, right * factor, bottom * factor, left * factor);
    }
    EdgeInsets operator/(double factor) const {
        return EdgeInsets(top / factor, right / factor, bottom / factor, left / factor);
    }

    EdgeInsets& operator+=(const EdgeInsets& other) {
        top += other.top; right += other.right; bottom += other.bottom; left += other.left;
        return *this;
    }
    EdgeInsets& operator-=(const EdgeInsets& other) {
        top -= other.top; right -= other.right; bottom -= other.bottom; left -= other.left;
        return *this;
    }
    EdgeInsets& operator*=(double factor) {
        top *= factor; right *= factor; bottom *= factor; left *= factor;
        return *this;
    }
    EdgeInsets& operator/=(double factor) {
        top /= factor; right /= factor; bottom /= factor; left /= factor;
        return *this;
    }

    bool operator==(const EdgeInsets& other) const {
        return top == other.top && right == other.right && bottom == other.bottom && left == other.left;
    }
    bool operator!=(const EdgeInsets& other) const { return !(*this == other); }
};

// Font metrics
struct FontMetrics {
    std::string family;
    double size;
    double weight;
    bool italic;
    bool bold;
    double ascent;
    double descent;
    double leading;
    double xHeight;
    double capHeight;

    FontMetrics() : family("Arial"), size(12), weight(400), italic(false), bold(false),
                    ascent(0), descent(0), leading(0), xHeight(0), capHeight(0) {}
    FontMetrics(const std::string& family, double size, double weight = 400, bool italic = false)
        : family(family), size(size), weight(weight), italic(italic), bold(weight >= 700),
          ascent(0), descent(0), leading(0), xHeight(0), capHeight(0) {}

    double lineHeight() const { return ascent + descent + leading; }
    double baseline() const { return ascent; }

    bool operator==(const FontMetrics& other) const {
        return family == other.family && size == other.size && weight == other.weight &&
               italic == other.italic && bold == other.bold;
    }
    bool operator!=(const FontMetrics& other) const { return !(*this == other); }
};

// Text run
struct TextRun {
    std::string text;
    FontMetrics font;
    Color color;
    Point position;
    Size size;

    TextRun() : text(), font(), color(), position(), size() {}
    TextRun(const std::string& text, const FontMetrics& font, const Color& color, const Point& position, const Size& size)
        : text(text), font(font), color(color), position(position), size(size) {}

    bool operator==(const TextRun& other) const {
        return text == other.text && font == other.font && color == other.color &&
               position == other.position && size == other.size;
    }
    bool operator!=(const TextRun& other) const { return !(*this == other); }
};

// Image data
struct ImageData {
    uint8_t* data;
    int width;
    int height;
    int channels;
    int stride;

    ImageData() : data(nullptr), width(0), height(0), channels(0), stride(0) {}
    ImageData(uint8_t* data, int width, int height, int channels, int stride = 0)
        : data(data), width(width), height(height), channels(channels), stride(stride) {
        if (stride == 0) {
            this->stride = width * channels;
        }
    }

    bool isValid() const { return data != nullptr && width > 0 && height > 0 && channels > 0; }
    size_t size() const { return height * stride; }
    uint8_t* pixel(int x, int y) const { return data + y * stride + x * channels; }
};

// Callback types
using DrawCallback = std::function<void()>;
using PaintCallback = std::function<void()>;
using PathCallback = std::function<void()>;
using TextCallback = std::function<void()>;
using ImageCallback = std::function<void()>;

// Error types
enum class RenderError {
    None,
    InvalidParameter,
    OutOfMemory,
    InvalidOperation,
    UnsupportedFormat,
    DeviceLost,
    DriverError,
    Unknown
};

// Result type
template<typename T>
struct Result {
    T value;
    RenderError error;

    Result() : value(), error(RenderError::None) {}
    Result(const T& value) : value(value), error(RenderError::None) {}
    Result(RenderError error) : value(), error(error) {}

    bool isOk() const { return error == RenderError::None; }
    bool isError() const { return error != RenderError::None; }

    operator bool() const { return isOk(); }
    T& operator*() { return value; }
    const T& operator*() const { return value; }
    T* operator->() { return &value; }
    const T* operator->() const { return &value; }
};

} // namespace renderer
