#include "renderer/paint.h"
#include <algorithm>
#include <cmath>

namespace renderer {

// Paint implementation
Paint::Paint()
    : style_(PaintStyle::Fill)
    , color_(Color::black())
    , strokeWidth_(1.0)
    , lineCap_(LineCap::Butt)
    , lineJoin_(LineJoin::Miter)
    , miterLimit_(4.0)
    , antialias_(AntialiasMode::High)
    , blendMode_(BlendMode::Normal)
    , opacity_(1.0)
    , shader_(nullptr)
    , gradient_(nullptr)
    , pattern_(nullptr)
    , image_(nullptr)
    , fillRule_(FillRule::NonZero)
    , textRenderingMode_(TextRenderingMode::Fill)
    , font_(FontMetrics())
    , textAlign_(TextAlign::Left)
    , textBaseline_(TextBaseline::Alphabetic)
    , textSize_(12.0)
    , textStyle_(FontStyle::Normal)
    , textWeight_(FontWeight::Normal)
    , textStretch_(FontStretch::Normal)
    , textFamily_("Arial")
    , hasShadow_(false)
    , shadowColor_(Color::black())
    , shadowOffset_(Point(0, 0))
    , shadowBlur_(0.0)
    , hasStrokeDash_(false)
    , strokeDashArray_()
    , strokeDashOffset_(0.0)
    , hasFilter_(false)
    , filterType_(FilterType::None)
    , filterColor_(Color::black())
    , filterBlur_(0.0)
    , filterOffset_(Point(0, 0))
    , transform_(Matrix::identity())
    , hasClip_(false)
    , clipRect_(Rect())
    , renderingHint_(RenderingHint::Quality)
    , memoryType_(MemoryType::Dynamic) {
}

Paint::~Paint() {
    cleanup();
}

Paint::Paint(const Paint& other) {
    copyFrom(other);
}

Paint& Paint::operator=(const Paint& other) {
    if (this != &other) {
        copyFrom(other);
    }
    return *this;
}

Paint::Paint(Paint&& other) noexcept {
    moveFrom(std::move(other));
}

Paint& Paint::operator=(Paint&& other) noexcept {
    if (this != &other) {
        moveFrom(std::move(other));
    }
    return *this;
}

bool Paint::operator==(const Paint& other) const {
    return style_ == other.style_ &&
           color_ == other.color_ &&
           strokeWidth_ == other.strokeWidth_ &&
           lineCap_ == other.lineCap_ &&
           lineJoin_ == other.lineJoin_ &&
           miterLimit_ == other.miterLimit_ &&
           antialias_ == other.antialias_ &&
           blendMode_ == other.blendMode_ &&
           opacity_ == other.opacity_ &&
           shader_ == other.shader_ &&
           gradient_ == other.gradient_ &&
           pattern_ == other.pattern_ &&
           image_ == other.image_ &&
           fillRule_ == other.fillRule_ &&
           textRenderingMode_ == other.textRenderingMode_ &&
           font_ == other.font_ &&
           textAlign_ == other.textAlign_ &&
           textBaseline_ == other.textBaseline_ &&
           textSize_ == other.textSize_ &&
           textStyle_ == other.textStyle_ &&
           textWeight_ == other.textWeight_ &&
           textStretch_ == other.textStretch_ &&
           textFamily_ == other.textFamily_ &&
           hasShadow_ == other.hasShadow_ &&
           shadowColor_ == other.shadowColor_ &&
           shadowOffset_ == other.shadowOffset_ &&
           shadowBlur_ == other.shadowBlur_ &&
           hasStrokeDash_ == other.hasStrokeDash_ &&
           strokeDashArray_ == other.strokeDashArray_ &&
           strokeDashOffset_ == other.strokeDashOffset_ &&
           hasFilter_ == other.hasFilter_ &&
           filterType_ == other.filterType_ &&
           filterColor_ == other.filterColor_ &&
           filterBlur_ == other.filterBlur_ &&
           filterOffset_ == other.filterOffset_ &&
           transform_ == other.transform_ &&
           hasClip_ == other.hasClip_ &&
           clipRect_ == other.clipRect_ &&
           renderingHint_ == other.renderingHint_ &&
           memoryType_ == other.memoryType_;
}

void Paint::reset() {
    *this = Paint();
}

std::unique_ptr<Paint> Paint::clone() const {
    return std::make_unique<Paint>(*this);
}

Paint Paint::fromColor(const Color& color) {
    Paint paint;
    paint.setColor(color);
    return paint;
}

Paint Paint::fromGradient(std::shared_ptr<Gradient> gradient) {
    Paint paint;
    paint.setGradient(gradient);
    return paint;
}

Paint Paint::fromPattern(std::shared_ptr<Pattern> pattern) {
    Paint paint;
    paint.setPattern(pattern);
    return paint;
}

Paint Paint::fromImage(std::shared_ptr<Image> image) {
    Paint paint;
    paint.setImage(image);
    return paint;
}

Paint Paint::fromShader(std::shared_ptr<Shader> shader) {
    Paint paint;
    paint.setShader(shader);
    return paint;
}

void Paint::copyFrom(const Paint& other) {
    style_ = other.style_;
    color_ = other.color_;
    strokeWidth_ = other.strokeWidth_;
    lineCap_ = other.lineCap_;
    lineJoin_ = other.lineJoin_;
    miterLimit_ = other.miterLimit_;
    antialias_ = other.antialias_;
    blendMode_ = other.blendMode_;
    opacity_ = other.opacity_;
    shader_ = other.shader_;
    gradient_ = other.gradient_;
    pattern_ = other.pattern_;
    image_ = other.image_;
    fillRule_ = other.fillRule_;
    textRenderingMode_ = other.textRenderingMode_;
    font_ = other.font_;
    textAlign_ = other.textAlign_;
    textBaseline_ = other.textBaseline_;
    textSize_ = other.textSize_;
    textStyle_ = other.textStyle_;
    textWeight_ = other.textWeight_;
    textStretch_ = other.textStretch_;
    textFamily_ = other.textFamily_;
    hasShadow_ = other.hasShadow_;
    shadowColor_ = other.shadowColor_;
    shadowOffset_ = other.shadowOffset_;
    shadowBlur_ = other.shadowBlur_;
    hasStrokeDash_ = other.hasStrokeDash_;
    strokeDashArray_ = other.strokeDashArray_;
    strokeDashOffset_ = other.strokeDashOffset_;
    hasFilter_ = other.hasFilter_;
    filterType_ = other.filterType_;
    filterColor_ = other.filterColor_;
    filterBlur_ = other.filterBlur_;
    filterOffset_ = other.filterOffset_;
    transform_ = other.transform_;
    hasClip_ = other.hasClip_;
    clipRect_ = other.clipRect_;
    renderingHint_ = other.renderingHint_;
    memoryType_ = other.memoryType_;
}

void Paint::moveFrom(Paint&& other) {
    style_ = other.style_;
    color_ = other.color_;
    strokeWidth_ = other.strokeWidth_;
    lineCap_ = other.lineCap_;
    lineJoin_ = other.lineJoin_;
    miterLimit_ = other.miterLimit_;
    antialias_ = other.antialias_;
    blendMode_ = other.blendMode_;
    opacity_ = other.opacity_;
    shader_ = std::move(other.shader_);
    gradient_ = std::move(other.gradient_);
    pattern_ = std::move(other.pattern_);
    image_ = std::move(other.image_);
    fillRule_ = other.fillRule_;
    textRenderingMode_ = other.textRenderingMode_;
    font_ = other.font_;
    textAlign_ = other.textAlign_;
    textBaseline_ = other.textBaseline_;
    textSize_ = other.textSize_;
    textStyle_ = other.textStyle_;
    textWeight_ = other.textWeight_;
    textStretch_ = other.textStretch_;
    textFamily_ = std::move(other.textFamily_);
    hasShadow_ = other.hasShadow_;
    shadowColor_ = other.shadowColor_;
    shadowOffset_ = other.shadowOffset_;
    shadowBlur_ = other.shadowBlur_;
    hasStrokeDash_ = other.hasStrokeDash_;
    strokeDashArray_ = std::move(other.strokeDashArray_);
    strokeDashOffset_ = other.strokeDashOffset_;
    hasFilter_ = other.hasFilter_;
    filterType_ = other.filterType_;
    filterColor_ = other.filterColor_;
    filterBlur_ = other.filterBlur_;
    filterOffset_ = other.filterOffset_;
    transform_ = other.transform_;
    hasClip_ = other.hasClip_;
    clipRect_ = other.clipRect_;
    renderingHint_ = other.renderingHint_;
    memoryType_ = other.memoryType_;
}

void Paint::cleanup() {
    shader_.reset();
    gradient_.reset();
    pattern_.reset();
    image_.reset();
}

} // namespace renderer
