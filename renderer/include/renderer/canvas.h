#pragma once

#include "types.h"
#include "enums.h"
#include "paint.h"
#include <memory>
#include <vector>
#include <functional>

namespace renderer {

// Forward declarations
class Paint;
class Path;
class Image;
class Text;
class Shader;
class Gradient;
class Pattern;
class Surface;
class Context;
class Device;
class Backend;
class Layer;
class Compositor;

// Canvas class for drawing operations
class Canvas {
public:
    Canvas();
    explicit Canvas(std::shared_ptr<Surface> surface);
    ~Canvas();

    // Surface management
    std::shared_ptr<Surface> surface() const { return surface_; }
    void setSurface(std::shared_ptr<Surface> surface);

    // Context management
    std::shared_ptr<Context> context() const { return context_; }
    void setContext(std::shared_ptr<Context> context);

    // Device management
    std::shared_ptr<Device> device() const { return device_; }
    void setDevice(std::shared_ptr<Device> device);

    // Backend management
    std::shared_ptr<Backend> backend() const { return backend_; }
    void setBackend(std::shared_ptr<Backend> backend);

    // Canvas state
    void save();
    void restore();
    void saveLayer(const Paint& paint);
    void saveLayer(const Rect& bounds, const Paint& paint);
    void saveLayerAlpha(uint8_t alpha);
    void saveLayerAlpha(const Rect& bounds, uint8_t alpha);

    // Transform operations
    void translate(double dx, double dy);
    void translate(const Point& offset);
    void scale(double sx, double sy);
    void scale(double scale);
    void scale(const Point& scale);
    void rotate(double angle);
    void rotate(double angle, const Point& center);
    void skew(double sx, double sy);
    void skew(const Point& skew);
    void setMatrix(const Matrix& matrix);
    void concat(const Matrix& matrix);
    Matrix getMatrix() const;
    void resetMatrix();

    // Clipping operations
    void clipRect(const Rect& rect);
    void clipRect(const Rect& rect, bool antiAlias);
    void clipPath(const Path& path);
    void clipPath(const Path& path, bool antiAlias);
    void clipRegion(const Rect& region);
    void clipRegion(const Rect& region, bool antiAlias);
    void clipOutRect(const Rect& rect);
    void clipOutRect(const Rect& rect, bool antiAlias);
    void clipOutPath(const Path& path);
    void clipOutPath(const Path& path, bool antiAlias);
    void clipOutRegion(const Rect& region);
    void clipOutRegion(const Rect& region, bool antiAlias);
    Rect getClipBounds() const;
    bool isClipEmpty() const;
    bool isClipRect() const;
    bool isClipPath() const;
    bool isClipRegion() const;
    bool isClipOut() const;

    // Drawing operations
    void drawColor(const Color& color);
    void drawColor(const Color& color, BlendMode blendMode);
    void drawPaint(const Paint& paint);
    void drawRect(const Rect& rect, const Paint& paint);
    void drawRoundRect(const Rect& rect, double rx, double ry, const Paint& paint);
    void drawCircle(const Point& center, double radius, const Paint& paint);
    void drawOval(const Rect& rect, const Paint& paint);
    void drawArc(const Rect& rect, double startAngle, double sweepAngle, bool useCenter, const Paint& paint);
    void drawPath(const Path& path, const Paint& paint);
    void drawLine(const Point& start, const Point& end, const Paint& paint);
    void drawPoint(const Point& point, const Paint& paint);
    void drawPoints(const std::vector<Point>& points, const Paint& paint);
    void drawPoints(const std::vector<Point>& points, PointMode mode, const Paint& paint);

    // Image drawing
    void drawImage(const Image& image, const Point& point);
    void drawImage(const Image& image, const Point& point, const Paint& paint);
    void drawImage(const Image& image, const Rect& destRect);
    void drawImage(const Image& image, const Rect& destRect, const Paint& paint);
    void drawImage(const Image& image, const Rect& srcRect, const Rect& destRect);
    void drawImage(const Image& image, const Rect& srcRect, const Rect& destRect, const Paint& paint);
    void drawImageRect(const Image& image, const Rect& srcRect, const Rect& destRect, const Paint& paint);
    void drawImageRect(const Image& image, const Rect& srcRect, const Rect& destRect, const Paint& paint, SrcRectConstraint constraint);

    // Text drawing
    void drawText(const std::string& text, const Point& point, const Paint& paint);
    void drawText(const std::string& text, double x, double y, const Paint& paint);
    void drawText(const std::string& text, const Point& point, const Paint& paint, const Rect& bounds);
    void drawText(const std::string& text, double x, double y, const Paint& paint, const Rect& bounds);
    void drawTextOnPath(const std::string& text, const Path& path, const Paint& paint);
    void drawTextOnPath(const std::string& text, const Path& path, double hOffset, double vOffset, const Paint& paint);
    void drawTextBlob(const TextBlob& blob, const Point& point, const Paint& paint);
    void drawTextBlob(const TextBlob& blob, double x, double y, const Paint& paint);

    // Gradient drawing
    void drawGradient(const Gradient& gradient, const Rect& rect, const Paint& paint);
    void drawGradient(const Gradient& gradient, const Path& path, const Paint& paint);

    // Pattern drawing
    void drawPattern(const Pattern& pattern, const Rect& rect, const Paint& paint);
    void drawPattern(const Pattern& pattern, const Path& path, const Paint& paint);

    // Shader drawing
    void drawShader(const Shader& shader, const Rect& rect, const Paint& paint);
    void drawShader(const Shader& shader, const Path& path, const Paint& paint);

    // Layer operations
    void drawLayer(const Layer& layer);
    void drawLayer(const Layer& layer, const Point& point);
    void drawLayer(const Layer& layer, const Rect& destRect);
    void drawLayer(const Layer& layer, const Rect& srcRect, const Rect& destRect);

    // Compositing operations
    void drawCanvas(const Canvas& canvas);
    void drawCanvas(const Canvas& canvas, const Point& point);
    void drawCanvas(const Canvas& canvas, const Rect& destRect);
    void drawCanvas(const Canvas& canvas, const Rect& srcRect, const Rect& destRect);

    // Clear operations
    void clear();
    void clear(const Color& color);
    void clear(const Rect& rect);
    void clear(const Rect& rect, const Color& color);

    // Fill operations
    void fillRect(const Rect& rect, const Color& color);
    void fillRoundRect(const Rect& rect, double rx, double ry, const Color& color);
    void fillCircle(const Point& center, double radius, const Color& color);
    void fillOval(const Rect& rect, const Color& color);
    void fillArc(const Rect& rect, double startAngle, double sweepAngle, bool useCenter, const Color& color);
    void fillPath(const Path& path, const Color& color);

    // Stroke operations
    void strokeRect(const Rect& rect, const Color& color);
    void strokeRect(const Rect& rect, const Color& color, double strokeWidth);
    void strokeRoundRect(const Rect& rect, double rx, double ry, const Color& color);
    void strokeRoundRect(const Rect& rect, double rx, double ry, const Color& color, double strokeWidth);
    void strokeCircle(const Point& center, double radius, const Color& color);
    void strokeCircle(const Point& center, double radius, const Color& color, double strokeWidth);
    void strokeOval(const Rect& rect, const Color& color);
    void strokeOval(const Rect& rect, const Color& color, double strokeWidth);
    void strokeArc(const Rect& rect, double startAngle, double sweepAngle, bool useCenter, const Color& color);
    void strokeArc(const Rect& rect, double startAngle, double sweepAngle, bool useCenter, const Color& color, double strokeWidth);
    void strokePath(const Path& path, const Color& color);
    void strokePath(const Path& path, const Color& color, double strokeWidth);
    void strokeLine(const Point& start, const Point& end, const Color& color);
    void strokeLine(const Point& start, const Point& end, const Color& color, double strokeWidth);

    // Measurement operations
    Size measureText(const std::string& text, const Paint& paint) const;
    double measureTextWidth(const std::string& text, const Paint& paint) const;
    double measureTextHeight(const std::string& text, const Paint& paint) const;
    Rect getTextBounds(const std::string& text, const Paint& paint) const;
    Rect getTextBounds(const std::string& text, const Point& point, const Paint& paint) const;
    std::vector<Rect> getTextRuns(const std::string& text, const Paint& paint) const;
    std::vector<Rect> getTextRuns(const std::string& text, const Point& point, const Paint& paint) const;

    // Hit testing
    bool containsPoint(const Point& point) const;
    bool containsRect(const Rect& rect) const;
    bool intersectsRect(const Rect& rect) const;
    Point mapPoint(const Point& point) const;
    Rect mapRect(const Rect& rect) const;
    Point unmapPoint(const Point& point) const;
    Rect unmapRect(const Rect& rect) const;

    // Utility operations
    void flush();
    void finish();
    void sync();
    void wait();
    void reset();
    void invalidate();
    void validate();

    // State queries
    bool isValid() const;
    bool isReady() const;
    bool isDirty() const;
    bool isEmpty() const;
    bool isFull() const;
    bool isClipped() const;
    bool isTransformed() const;
    bool isScaled() const;
    bool isRotated() const;
    bool isSkewed() const;

    // Size queries
    Size size() const;
    Rect bounds() const;
    Rect clipBounds() const;
    Rect visibleBounds() const;
    Rect dirtyBounds() const;
    Rect invalidBounds() const;

    // Property queries
    double width() const;
    double height() const;
    double scaleX() const;
    double scaleY() const;
    double rotation() const;
    double skewX() const;
    double skewY() const;
    Point translation() const;
    Point scale() const;
    Point skew() const;

    // Copy and assignment
    Canvas(const Canvas& other);
    Canvas& operator=(const Canvas& other);

    // Move constructor and assignment
    Canvas(Canvas&& other) noexcept;
    Canvas& operator=(Canvas&& other) noexcept;

    // Equality
    bool operator==(const Canvas& other) const;
    bool operator!=(const Canvas& other) const { return !(*this == other); }

    // Clone canvas
    std::unique_ptr<Canvas> clone() const;

    // Create canvas from surface
    static std::unique_ptr<Canvas> fromSurface(std::shared_ptr<Surface> surface);

    // Create canvas from image
    static std::unique_ptr<Canvas> fromImage(std::shared_ptr<Image> image);

    // Create canvas from context
    static std::unique_ptr<Canvas> fromContext(std::shared_ptr<Context> context);

    // Create canvas from device
    static std::unique_ptr<Canvas> fromDevice(std::shared_ptr<Device> device);

    // Create canvas from backend
    static std::unique_ptr<Canvas> fromBackend(std::shared_ptr<Backend> backend);

private:
    // Core components
    std::shared_ptr<Surface> surface_;
    std::shared_ptr<Context> context_;
    std::shared_ptr<Device> device_;
    std::shared_ptr<Backend> backend_;

    // State management
    std::vector<CanvasState> stateStack_;
    CanvasState currentState_;

    // Drawing state
    Paint currentPaint_;
    Matrix currentMatrix_;
    Rect currentClip_;
    bool isDirty_;
    bool isValid_;
    bool isReady_;

    // Helper methods
    void copyFrom(const Canvas& other);
    void moveFrom(Canvas&& other);
    void cleanup();
    void initialize();
    void updateState();
    void validateState();
    void invalidateState();
    void resetState();
    void pushState();
    void popState();
    void applyState(const CanvasState& state);
    void updateMatrix();
    void updateClip();
    void updatePaint();
    void updateSurface();
    void updateContext();
    void updateDevice();
    void updateBackend();
};

// Canvas state for save/restore operations
struct CanvasState {
    Matrix matrix;
    Rect clip;
    Paint paint;
    bool isDirty;
    bool isValid;
    bool isReady;

    CanvasState() : matrix(Matrix::identity()), clip(), paint(), isDirty(false), isValid(true), isReady(true) {}
    CanvasState(const Matrix& matrix, const Rect& clip, const Paint& paint)
        : matrix(matrix), clip(clip), paint(paint), isDirty(false), isValid(true), isReady(true) {}

    CanvasState(const CanvasState& other) = default;
    CanvasState& operator=(const CanvasState& other) = default;
    CanvasState(CanvasState&& other) noexcept = default;
    CanvasState& operator=(CanvasState&& other) noexcept = default;

    bool operator==(const CanvasState& other) const {
        return matrix == other.matrix && clip == other.clip && paint == other.paint &&
               isDirty == other.isDirty && isValid == other.isValid && isReady == other.isReady;
    }
    bool operator!=(const CanvasState& other) const { return !(*this == other); }
};

// Point mode for drawing points
enum class PointMode {
    Points,
    Lines,
    Polygon
};

// Source rectangle constraint
enum class SrcRectConstraint {
    Strict,
    Fast
};

// Text blob for text rendering
class TextBlob {
public:
    TextBlob();
    ~TextBlob();

    // Text content
    const std::string& text() const { return text_; }
    void setText(const std::string& text) { text_ = text; }

    // Font metrics
    const FontMetrics& font() const { return font_; }
    void setFont(const FontMetrics& font) { font_ = font; }

    // Text runs
    const std::vector<TextRun>& runs() const { return runs_; }
    void setRuns(const std::vector<TextRun>& runs) { runs_ = runs; }

    // Bounds
    const Rect& bounds() const { return bounds_; }
    void setBounds(const Rect& bounds) { bounds_ = bounds; }

    // Size
    const Size& size() const { return size_; }
    void setSize(const Size& size) { size_ = size; }

    // Copy and assignment
    TextBlob(const TextBlob& other) = default;
    TextBlob& operator=(const TextBlob& other) = default;
    TextBlob(TextBlob&& other) noexcept = default;
    TextBlob& operator=(TextBlob&& other) noexcept = default;

    // Equality
    bool operator==(const TextBlob& other) const {
        return text_ == other.text_ && font_ == other.font_ && runs_ == other.runs_ &&
               bounds_ == other.bounds_ && size_ == other.size_;
    }
    bool operator!=(const TextBlob& other) const { return !(*this == other); }

    // Create text blob from text and paint
    static TextBlob fromText(const std::string& text, const Paint& paint);

    // Create text blob from text runs
    static TextBlob fromRuns(const std::vector<TextRun>& runs);

private:
    std::string text_;
    FontMetrics font_;
    std::vector<TextRun> runs_;
    Rect bounds_;
    Size size_;
};

} // namespace renderer
