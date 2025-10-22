#pragma once

#include "types.h"
#include "enums.h"
#include <memory>
#include <vector>

namespace renderer {

// Forward declarations
class Shader;
class Image;
class Gradient;
class Pattern;

// Paint class for drawing operations
class Paint {
public:
    Paint();
    ~Paint();

    // Paint style
    PaintStyle style() const { return style_; }
    void setStyle(PaintStyle style) { style_ = style; }

    // Color
    const Color& color() const { return color_; }
    void setColor(const Color& color) { color_ = color; }

    // Stroke width
    double strokeWidth() const { return strokeWidth_; }
    void setStrokeWidth(double width) { strokeWidth_ = width; }

    // Line cap
    LineCap lineCap() const { return lineCap_; }
    void setLineCap(LineCap cap) { lineCap_ = cap; }

    // Line join
    LineJoin lineJoin() const { return lineJoin_; }
    void setLineJoin(LineJoin join) { lineJoin_ = join; }

    // Miter limit
    double miterLimit() const { return miterLimit_; }
    void setMiterLimit(double limit) { miterLimit_ = limit; }

    // Antialiasing
    AntialiasMode antialias() const { return antialias_; }
    void setAntialias(AntialiasMode mode) { antialias_ = mode; }

    // Blend mode
    BlendMode blendMode() const { return blendMode_; }
    void setBlendMode(BlendMode mode) { blendMode_ = mode; }

    // Opacity
    double opacity() const { return opacity_; }
    void setOpacity(double opacity) { opacity_ = opacity; }

    // Shader
    Shader* shader() const { return shader_.get(); }
    void setShader(std::shared_ptr<Shader> shader) { shader_ = shader; }

    // Gradient
    Gradient* gradient() const { return gradient_.get(); }
    void setGradient(std::shared_ptr<Gradient> gradient) { gradient_ = gradient; }

    // Pattern
    Pattern* pattern() const { return pattern_.get(); }
    void setPattern(std::shared_ptr<Pattern> pattern) { pattern_ = pattern; }

    // Image
    Image* image() const { return image_.get(); }
    void setImage(std::shared_ptr<Image> image) { image_ = image; }

    // Fill rule
    FillRule fillRule() const { return fillRule_; }
    void setFillRule(FillRule rule) { fillRule_ = rule; }

    // Text rendering mode
    TextRenderingMode textRenderingMode() const { return textRenderingMode_; }
    void setTextRenderingMode(TextRenderingMode mode) { textRenderingMode_ = mode; }

    // Font
    const FontMetrics& font() const { return font_; }
    void setFont(const FontMetrics& font) { font_ = font; }

    // Text alignment
    TextAlign textAlign() const { return textAlign_; }
    void setTextAlign(TextAlign align) { textAlign_ = align; }

    // Text baseline
    TextBaseline textBaseline() const { return textBaseline_; }
    void setTextBaseline(TextBaseline baseline) { textBaseline_ = baseline; }

    // Text size
    double textSize() const { return textSize_; }
    void setTextSize(double size) { textSize_ = size; }

    // Text style
    FontStyle textStyle() const { return textStyle_; }
    void setTextStyle(FontStyle style) { textStyle_ = style; }

    // Text weight
    FontWeight textWeight() const { return textWeight_; }
    void setTextWeight(FontWeight weight) { textWeight_ = weight; }

    // Text stretch
    FontStretch textStretch() const { return textStretch_; }
    void setTextStretch(FontStretch stretch) { textStretch_ = stretch; }

    // Text family
    const std::string& textFamily() const { return textFamily_; }
    void setTextFamily(const std::string& family) { textFamily_ = family; }

    // Shadow
    bool hasShadow() const { return hasShadow_; }
    void setShadow(bool hasShadow) { hasShadow_ = hasShadow; }

    const Color& shadowColor() const { return shadowColor_; }
    void setShadowColor(const Color& color) { shadowColor_ = color; }

    const Point& shadowOffset() const { return shadowOffset_; }
    void setShadowOffset(const Point& offset) { shadowOffset_ = offset; }

    double shadowBlur() const { return shadowBlur_; }
    void setShadowBlur(double blur) { shadowBlur_ = blur; }

    // Stroke dash
    bool hasStrokeDash() const { return hasStrokeDash_; }
    void setStrokeDash(bool hasDash) { hasStrokeDash_ = hasDash; }

    const std::vector<double>& strokeDashArray() const { return strokeDashArray_; }
    void setStrokeDashArray(const std::vector<double>& dashArray) { strokeDashArray_ = dashArray; }

    double strokeDashOffset() const { return strokeDashOffset_; }
    void setStrokeDashOffset(double offset) { strokeDashOffset_ = offset; }

    // Filters
    bool hasFilter() const { return hasFilter_; }
    void setFilter(bool hasFilter) { hasFilter_ = hasFilter; }

    FilterType filterType() const { return filterType_; }
    void setFilterType(FilterType type) { filterType_ = type; }

    const Color& filterColor() const { return filterColor_; }
    void setFilterColor(const Color& color) { filterColor_ = color; }

    double filterBlur() const { return filterBlur_; }
    void setFilterBlur(double blur) { filterBlur_ = blur; }

    const Point& filterOffset() const { return filterOffset_; }
    void setFilterOffset(const Point& offset) { filterOffset_ = offset; }

    // Transform
    const Matrix& transform() const { return transform_; }
    void setTransform(const Matrix& transform) { transform_ = transform; }

    // Clipping
    bool hasClip() const { return hasClip_; }
    void setClip(bool hasClip) { hasClip_ = hasClip; }

    const Rect& clipRect() const { return clipRect_; }
    void setClipRect(const Rect& rect) { clipRect_ = rect; }

    // Rendering hints
    RenderingHint renderingHint() const { return renderingHint_; }
    void setRenderingHint(RenderingHint hint) { renderingHint_ = hint; }

    // Memory management
    MemoryType memoryType() const { return memoryType_; }
    void setMemoryType(MemoryType type) { memoryType_ = type; }

    // Copy and assignment
    Paint(const Paint& other);
    Paint& operator=(const Paint& other);

    // Move constructor and assignment
    Paint(Paint&& other) noexcept;
    Paint& operator=(Paint&& other) noexcept;

    // Equality
    bool operator==(const Paint& other) const;
    bool operator!=(const Paint& other) const { return !(*this == other); }

    // Reset to default values
    void reset();

    // Clone paint
    std::unique_ptr<Paint> clone() const;

    // Create paint from color
    static Paint fromColor(const Color& color);

    // Create paint from gradient
    static Paint fromGradient(std::shared_ptr<Gradient> gradient);

    // Create paint from pattern
    static Paint fromPattern(std::shared_ptr<Pattern> pattern);

    // Create paint from image
    static Paint fromImage(std::shared_ptr<Image> image);

    // Create paint from shader
    static Paint fromShader(std::shared_ptr<Shader> shader);

private:
    // Basic properties
    PaintStyle style_;
    Color color_;
    double strokeWidth_;
    LineCap lineCap_;
    LineJoin lineJoin_;
    double miterLimit_;
    AntialiasMode antialias_;
    BlendMode blendMode_;
    double opacity_;

    // Shader and pattern
    std::shared_ptr<Shader> shader_;
    std::shared_ptr<Gradient> gradient_;
    std::shared_ptr<Pattern> pattern_;
    std::shared_ptr<Image> image_;

    // Fill properties
    FillRule fillRule_;

    // Text properties
    TextRenderingMode textRenderingMode_;
    FontMetrics font_;
    TextAlign textAlign_;
    TextBaseline textBaseline_;
    double textSize_;
    FontStyle textStyle_;
    FontWeight textWeight_;
    FontStretch textStretch_;
    std::string textFamily_;

    // Shadow properties
    bool hasShadow_;
    Color shadowColor_;
    Point shadowOffset_;
    double shadowBlur_;

    // Stroke dash properties
    bool hasStrokeDash_;
    std::vector<double> strokeDashArray_;
    double strokeDashOffset_;

    // Filter properties
    bool hasFilter_;
    FilterType filterType_;
    Color filterColor_;
    double filterBlur_;
    Point filterOffset_;

    // Transform properties
    Matrix transform_;

    // Clipping properties
    bool hasClip_;
    Rect clipRect_;

    // Rendering properties
    RenderingHint renderingHint_;
    MemoryType memoryType_;

    // Helper methods
    void copyFrom(const Paint& other);
    void moveFrom(Paint&& other);
    void cleanup();
};

} // namespace renderer
