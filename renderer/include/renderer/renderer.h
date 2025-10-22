#pragma once

#include "types.h"
#include "enums.h"
#include "canvas.h"
#include "paint.h"
#include <memory>
#include <vector>

namespace renderer {

// Forward declarations
class Canvas;
class Paint;
class Path;
class Image;
class Surface;
class Context;
class Device;
class Backend;
class Layer;
class Compositor;

// Main Renderer class
class Renderer {
public:
    Renderer();
    explicit Renderer(std::shared_ptr<Backend> backend);
    ~Renderer();

    // Backend management
    std::shared_ptr<Backend> backend() const { return backend_; }
    void setBackend(std::shared_ptr<Backend> backend);

    // Device management
    std::shared_ptr<Device> device() const { return device_; }
    void setDevice(std::shared_ptr<Device> device);

    // Context management
    std::shared_ptr<Context> context() const { return context_; }
    void setContext(std::shared_ptr<Context> context);

    // Surface management
    std::shared_ptr<Surface> surface() const { return surface_; }
    void setSurface(std::shared_ptr<Surface> surface);

    // Canvas creation
    std::unique_ptr<Canvas> createCanvas();
    std::unique_ptr<Canvas> createCanvas(std::shared_ptr<Surface> surface);
    std::unique_ptr<Canvas> createCanvas(const Size& size);
    std::unique_ptr<Canvas> createCanvas(int width, int height);

    // Paint creation
    std::unique_ptr<Paint> createPaint();
    std::unique_ptr<Paint> createPaint(const Color& color);
    std::unique_ptr<Paint> createPaint(const Paint& paint);

    // Path creation
    std::unique_ptr<Path> createPath();
    std::unique_ptr<Path> createPath(const Path& path);

    // Image creation
    std::unique_ptr<Image> createImage();
    std::unique_ptr<Image> createImage(const Size& size);
    std::unique_ptr<Image> createImage(int width, int height);
    std::unique_ptr<Image> createImage(const ImageData& data);

    // Surface creation
    std::unique_ptr<Surface> createSurface();
    std::unique_ptr<Surface> createSurface(const Size& size);
    std::unique_ptr<Surface> createSurface(int width, int height);
    std::unique_ptr<Surface> createSurface(SurfaceType type);

    // Layer creation
    std::unique_ptr<Layer> createLayer();
    std::unique_ptr<Layer> createLayer(const Size& size);
    std::unique_ptr<Layer> createLayer(int width, int height);

    // Compositor creation
    std::unique_ptr<Compositor> createCompositor();
    std::unique_ptr<Compositor> createCompositor(CompositorType type);

    // Rendering operations
    void render(Canvas* canvas);
    void render(Canvas* canvas, const Rect& bounds);
    void render(Layer* layer);
    void render(Layer* layer, const Rect& bounds);
    void render(Surface* surface);
    void render(Surface* surface, const Rect& bounds);

    // Compositing operations
    void composite(Layer* layer);
    void composite(Layer* layer, const Point& position);
    void composite(Layer* layer, const Rect& destRect);
    void composite(Layer* layer, const Rect& srcRect, const Rect& destRect);

    // Clear operations
    void clear();
    void clear(const Color& color);
    void clear(const Rect& rect);
    void clear(const Rect& rect, const Color& color);

    // Flush operations
    void flush();
    void finish();
    void sync();
    void wait();

    // State management
    void save();
    void restore();
    void reset();

    // Transform operations
    void translate(double dx, double dy);
    void scale(double sx, double sy);
    void rotate(double angle);
    void setMatrix(const Matrix& matrix);
    Matrix getMatrix() const;

    // Clipping operations
    void clipRect(const Rect& rect);
    void clipPath(const Path& path);
    Rect getClipBounds() const;
    bool isClipEmpty() const;

    // Drawing operations
    void drawColor(const Color& color);
    void drawPaint(const Paint& paint);
    void drawRect(const Rect& rect, const Paint& paint);
    void drawCircle(const Point& center, double radius, const Paint& paint);
    void drawPath(const Path& path, const Paint& paint);
    void drawImage(const Image& image, const Point& point);
    void drawText(const std::string& text, const Point& point, const Paint& paint);

    // Measurement operations
    Size measureText(const std::string& text, const Paint& paint) const;
    Rect getTextBounds(const std::string& text, const Paint& paint) const;

    // Utility operations
    void invalidate();
    void validate();
    bool isValid() const;
    bool isReady() const;
    bool isDirty() const;

    // Size queries
    Size size() const;
    Rect bounds() const;
    double width() const;
    double height() const;

    // Copy and assignment
    Renderer(const Renderer& other);
    Renderer& operator=(const Renderer& other);
    Renderer(Renderer&& other) noexcept;
    Renderer& operator=(Renderer&& other) noexcept;

    // Equality
    bool operator==(const Renderer& other) const;
    bool operator!=(const Renderer& other) const { return !(*this == other); }

    // Clone renderer
    std::unique_ptr<Renderer> clone() const;

    // Create renderer from backend
    static std::unique_ptr<Renderer> fromBackend(std::shared_ptr<Backend> backend);

    // Create renderer from device
    static std::unique_ptr<Renderer> fromDevice(std::shared_ptr<Device> device);

    // Create renderer from context
    static std::unique_ptr<Renderer> fromContext(std::shared_ptr<Context> context);

private:
    // Core components
    std::shared_ptr<Backend> backend_;
    std::shared_ptr<Device> device_;
    std::shared_ptr<Context> context_;
    std::shared_ptr<Surface> surface_;

    // State management
    std::vector<RendererState> stateStack_;
    RendererState currentState_;

    // Drawing state
    Matrix currentMatrix_;
    Rect currentClip_;
    bool isDirty_;
    bool isValid_;
    bool isReady_;

    // Helper methods
    void copyFrom(const Renderer& other);
    void moveFrom(Renderer&& other);
    void cleanup();
    void initialize();
    void updateState();
    void validateState();
    void invalidateState();
    void resetState();
    void pushState();
    void popState();
    void applyState(const RendererState& state);
    void updateMatrix();
    void updateClip();
    void updateBackend();
    void updateDevice();
    void updateContext();
    void updateSurface();
};

// Renderer state for save/restore operations
struct RendererState {
    Matrix matrix;
    Rect clip;
    bool isDirty;
    bool isValid;
    bool isReady;

    RendererState() : matrix(Matrix::identity()), clip(), isDirty(false), isValid(true), isReady(true) {}
    RendererState(const Matrix& matrix, const Rect& clip)
        : matrix(matrix), clip(clip), isDirty(false), isValid(true), isReady(true) {}

    RendererState(const RendererState& other) = default;
    RendererState& operator=(const RendererState& other) = default;
    RendererState(RendererState&& other) noexcept = default;
    RendererState& operator=(RendererState&& other) noexcept = default;

    bool operator==(const RendererState& other) const {
        return matrix == other.matrix && clip == other.clip &&
               isDirty == other.isDirty && isValid == other.isValid && isReady == other.isReady;
    }
    bool operator!=(const RendererState& other) const { return !(*this == other); }
};

} // namespace renderer
