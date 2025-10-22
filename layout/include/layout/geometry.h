#pragma once

#include "types.h"
#include <memory>
#include <vector>

namespace layout {

// Geometry utilities for layout calculations
class Geometry {
public:
    Geometry();
    ~Geometry();

    // Point operations
    static Point addPoints(const Point& a, const Point& b);
    static Point subtractPoints(const Point& a, const Point& b);
    static Point multiplyPoint(const Point& point, double factor);
    static Point dividePoint(const Point& point, double factor);
    static double distanceBetweenPoints(const Point& a, const Point& b);
    static double angleBetweenPoints(const Point& a, const Point& b);

    // Size operations
    static Size addSizes(const Size& a, const Size& b);
    static Size subtractSizes(const Size& a, const Size& b);
    static Size multiplySize(const Size& size, double factor);
    static Size divideSize(const Size& size, double factor);
    static Size maxSize(const Size& a, const Size& b);
    static Size minSize(const Size& a, const Size& b);
    static Size constrainSize(const Size& size, const Size& minSize, const Size& maxSize);

    // Rectangle operations
    static Rect addRect(const Rect& rect, const Point& offset);
    static Rect subtractRect(const Rect& rect, const Point& offset);
    static Rect multiplyRect(const Rect& rect, double factor);
    static Rect divideRect(const Rect& rect, double factor);
    static Rect maxRect(const Rect& a, const Rect& b);
    static Rect minRect(const Rect& a, const Rect& b);
    static Rect constrainRect(const Rect& rect, const Rect& bounds);
    static Rect expandRect(const Rect& rect, const EdgeInsets& insets);
    static Rect shrinkRect(const Rect& rect, const EdgeInsets& insets);
    static Rect centerRect(const Rect& rect, const Rect& bounds);
    static Rect alignRect(const Rect& rect, const Rect& bounds, const Point& alignment);

    // Transform operations
    static Point transformPoint(const Point& point, const Transform& transform);
    static Size transformSize(const Size& size, const Transform& transform);
    static Rect transformRect(const Rect& rect, const Transform& transform);
    static Transform combineTransforms(const Transform& a, const Transform& b);
    static Transform invertTransform(const Transform& transform);

    // Edge insets operations
    static EdgeInsets addEdgeInsets(const EdgeInsets& a, const EdgeInsets& b);
    static EdgeInsets subtractEdgeInsets(const EdgeInsets& a, const EdgeInsets& b);
    static EdgeInsets multiplyEdgeInsets(const EdgeInsets& insets, double factor);
    static EdgeInsets divideEdgeInsets(const EdgeInsets& insets, double factor);
    static EdgeInsets maxEdgeInsets(const EdgeInsets& a, const EdgeInsets& b);
    static EdgeInsets minEdgeInsets(const EdgeInsets& a, const EdgeInsets& b);

    // Layout calculations
    static Size calculateContentSize(const Size& totalSize, const EdgeInsets& padding, const EdgeInsets& border, BoxSizing boxSizing);
    static Size calculateTotalSize(const Size& contentSize, const EdgeInsets& padding, const EdgeInsets& border, const EdgeInsets& margin, BoxSizing boxSizing);
    static Point calculatePosition(const Point& offset, const EdgeInsets& margin);
    static Rect calculateBounds(const Point& position, const Size& size);
    static Rect calculateContentBounds(const Rect& bounds, const EdgeInsets& padding, const EdgeInsets& border);
    static Rect calculatePaddingBounds(const Rect& bounds, const EdgeInsets& padding);
    static Rect calculateBorderBounds(const Rect& bounds, const EdgeInsets& border);
    static Rect calculateMarginBounds(const Rect& bounds, const EdgeInsets& margin);

    // Collision detection
    static bool pointInRect(const Point& point, const Rect& rect);
    static bool rectIntersects(const Rect& a, const Rect& b);
    static bool rectContains(const Rect& container, const Rect& contained);
    static Rect rectIntersection(const Rect& a, const Rect& b);
    static Rect rectUnion(const Rect& a, const Rect& b);

    // Distance calculations
    static double distanceFromPointToRect(const Point& point, const Rect& rect);
    static double distanceFromRectToRect(const Rect& a, const Rect& b);
    static double distanceFromPointToLine(const Point& point, const Point& lineStart, const Point& lineEnd);

    // Angle calculations
    static double angleFromPoint(const Point& point);
    static double angleBetweenLines(const Point& line1Start, const Point& line1End, const Point& line2Start, const Point& line2End);
    static double angleBetweenVectors(const Point& vector1, const Point& vector2);

    // Interpolation
    static Point interpolatePoints(const Point& start, const Point& end, double t);
    static Size interpolateSizes(const Size& start, const Size& end, double t);
    static Rect interpolateRects(const Rect& start, const Rect& end, double t);
    static EdgeInsets interpolateEdgeInsets(const EdgeInsets& start, const EdgeInsets& end, double t);

    // Clamping
    static double clamp(double value, double min, double max);
    static Point clampPoint(const Point& point, const Rect& bounds);
    static Size clampSize(const Size& size, const Size& minSize, const Size& maxSize);
    static Rect clampRect(const Rect& rect, const Rect& bounds);

    // Rounding
    static Point roundPoint(const Point& point);
    static Size roundSize(const Size& size);
    static Rect roundRect(const Rect& rect);

    // Validation
    static bool isValidPoint(const Point& point);
    static bool isValidSize(const Size& size);
    static bool isValidRect(const Rect& rect);
    static bool isValidTransform(const Transform& transform);

    // Normalization
    static Point normalizePoint(const Point& point);
    static Size normalizeSize(const Size& size);
    static Rect normalizeRect(const Rect& rect);

private:
    static const double EPSILON;
    static bool isZero(double value);
    static bool isEqual(double a, double b);
    static bool isLessThan(double a, double b);
    static bool isGreaterThan(double a, double b);
};

} // namespace layout
