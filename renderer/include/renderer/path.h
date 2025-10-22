#pragma once

namespace renderer {

class Path {
public:
    Path();
    ~Path();

    void move_to(float x, float y);
    void line_to(float x, float y);
    void curve_to(float x1, float y1, float x2, float y2, float x3, float y3);
    void close_path();

    void add_rect(float x, float y, float width, float height);
    void add_circle(float cx, float cy, float radius);
    void add_ellipse(float cx, float cy, float rx, float ry);

    void clear();
    bool is_empty() const;

private:
    // Implementation details would go here
};

} // namespace renderer
