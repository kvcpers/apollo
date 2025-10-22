#include "renderer/canvas.h"
#include <algorithm>
#include <cmath>

namespace renderer {

// Canvas implementation
Canvas::Canvas()
    : surface_(nullptr)
    , context_(nullptr)
    , device_(nullptr)
    , backend_(nullptr)
    , stateStack_()
    , currentState_()
    , currentPaint_()
    , currentMatrix_(Matrix::identity())
    , currentClip_(Rect())
    , isDirty_(false)
    , isValid_(false)
    , isReady_(false) {
    initialize();
}

Canvas::Canvas(std::shared_ptr<Surface> surface)
    : surface_(surface)
    , context_(nullptr)
    , device_(nullptr)
    , backend_(nullptr)
    , stateStack_()
    , currentState_()
    , currentPaint_()
    , currentMatrix_(Matrix::identity())
    , currentClip_(Rect())
    , isDirty_(false)
    , isValid_(false)
    , isReady_(false) {
    initialize();
}

Canvas::~Canvas() {
    cleanup();
}

void Canvas::setSurface(std::shared_ptr<Surface> surface) {
    surface_ = surface;
    updateSurface();
}

void Canvas::setContext(std::shared_ptr<Context> context) {
    context_ = context;
    updateContext();
}

void Canvas::setDevice(std::shared_ptr<Device> device) {
    device_ = device;
    updateDevice();
}

void Canvas::setBackend(std::shared_ptr<Backend> backend) {
    backend_ = backend;
    updateBackend();
}

void Canvas::save() {
    stateStack_.push_back(currentState_);
    pushState();
}

void Canvas::restore() {
    if (!stateStack_.empty()) {
        currentState_ = stateStack_.back();
        stateStack_.pop_back();
        applyState(currentState_);
    }
}

void Canvas::saveLayer(const Paint& paint) {
    // This is a simplified implementation
    // In a real implementation, this would save a layer with paint
    save();
}

void Canvas::saveLayer(const Rect& bounds, const Paint& paint) {
    // This is a simplified implementation
    // In a real implementation, this would save a layer with bounds and paint
    save();
}

void Canvas::saveLayerAlpha(uint8_t alpha) {
    // This is a simplified implementation
    // In a real implementation, this would save a layer with alpha
    save();
}

void Canvas::saveLayerAlpha(const Rect& bounds, uint8_t alpha) {
    // This is a simplified implementation
    // In a real implementation, this would save a layer with bounds and alpha
    save();
}

void Canvas::translate(double dx, double dy) {
    currentMatrix_ = currentMatrix_ * Matrix::translation(dx, dy);
    updateMatrix();
}

void Canvas::translate(const Point& offset) {
    translate(offset.x, offset.y);
}

void Canvas::scale(double sx, double sy) {
    currentMatrix_ = currentMatrix_ * Matrix::scale(sx, sy);
    updateMatrix();
}

void Canvas::scale(double scale) {
    this->scale(scale, scale);
}

void Canvas::scale(const Point& scale) {
    this->scale(scale.x, scale.y);
}

void Canvas::rotate(double angle) {
    currentMatrix_ = currentMatrix_ * Matrix::rotation(angle);
    updateMatrix();
}

void Canvas::rotate(double angle, const Point& center) {
    translate(-center.x, -center.y);
    rotate(angle);
    translate(center.x, center.y);
}

void Canvas::skew(double sx, double sy) {
    // This is a simplified implementation
    // In a real implementation, this would handle skewing
    updateMatrix();
}

void Canvas::skew(const Point& skew) {
    this->skew(skew.x, skew.y);
}

void Canvas::setMatrix(const Matrix& matrix) {
    currentMatrix_ = matrix;
    updateMatrix();
}

void Canvas::concat(const Matrix& matrix) {
    currentMatrix_ = currentMatrix_ * matrix;
    updateMatrix();
}

Matrix Canvas::getMatrix() const {
    return currentMatrix_;
}

void Canvas::resetMatrix() {
    currentMatrix_ = Matrix::identity();
    updateMatrix();
}

void Canvas::clipRect(const Rect& rect) {
    if (currentClip_.isEmpty()) {
        currentClip_ = rect;
    } else {
        currentClip_ = currentClip_.intersection(rect);
    }
    updateClip();
}

void Canvas::clipRect(const Rect& rect, bool antiAlias) {
    // This is a simplified implementation
    // In a real implementation, this would handle anti-aliasing
    clipRect(rect);
}

void Canvas::clipPath(const Path& path) {
    // This is a simplified implementation
    // In a real implementation, this would handle path clipping
    updateClip();
}

void Canvas::clipPath(const Path& path, bool antiAlias) {
    // This is a simplified implementation
    // In a real implementation, this would handle path clipping with anti-aliasing
    clipPath(path);
}

void Canvas::clipRegion(const Rect& region) {
    clipRect(region);
}

void Canvas::clipRegion(const Rect& region, bool antiAlias) {
    clipRect(region, antiAlias);
}

void Canvas::clipOutRect(const Rect& rect) {
    // This is a simplified implementation
    // In a real implementation, this would handle clip out operations
    updateClip();
}

void Canvas::clipOutRect(const Rect& rect, bool antiAlias) {
    // This is a simplified implementation
    // In a real implementation, this would handle clip out operations with anti-aliasing
    clipOutRect(rect);
}

void Canvas::clipOutPath(const Path& path) {
    // This is a simplified implementation
    // In a real implementation, this would handle clip out path operations
    updateClip();
}

void Canvas::clipOutPath(const Path& path, bool antiAlias) {
    // This is a simplified implementation
    // In a real implementation, this would handle clip out path operations with anti-aliasing
    clipOutPath(path);
}

void Canvas::clipOutRegion(const Rect& region) {
    clipOutRect(region);
}

void Canvas::clipOutRegion(const Rect& region, bool antiAlias) {
    clipOutRect(region, antiAlias);
}

Rect Canvas::getClipBounds() const {
    return currentClip_;
}

bool Canvas::isClipEmpty() const {
    return currentClip_.isEmpty();
}

bool Canvas::isClipRect() const {
    // This is a simplified implementation
    // In a real implementation, this would check if clip is a rectangle
    return true;
}

bool Canvas::isClipPath() const {
    // This is a simplified implementation
    // In a real implementation, this would check if clip is a path
    return false;
}

bool Canvas::isClipRegion() const {
    // This is a simplified implementation
    // In a real implementation, this would check if clip is a region
    return false;
}

bool Canvas::isClipOut() const {
    // This is a simplified implementation
    // In a real implementation, this would check if clip is an out operation
    return false;
}

void Canvas::drawColor(const Color& color) {
    if (!surface_) return;
    
    // Draw color to surface
    // This is a simplified implementation
    // In a real implementation, this would draw color to surface
    isDirty_ = true;
}

void Canvas::drawColor(const Color& color, BlendMode blendMode) {
    if (!surface_) return;
    
    // Draw color to surface with blend mode
    // This is a simplified implementation
    // In a real implementation, this would draw color to surface with blend mode
    isDirty_ = true;
}

void Canvas::drawPaint(const Paint& paint) {
    if (!surface_) return;
    
    // Draw paint to surface
    // This is a simplified implementation
    // In a real implementation, this would draw paint to surface
    isDirty_ = true;
}

void Canvas::drawRect(const Rect& rect, const Paint& paint) {
    if (!surface_) return;
    
    // Draw rectangle to surface
    // This is a simplified implementation
    // In a real implementation, this would draw rectangle to surface
    isDirty_ = true;
}

void Canvas::drawRoundRect(const Rect& rect, double rx, double ry, const Paint& paint) {
    if (!surface_) return;
    
    // Draw rounded rectangle to surface
    // This is a simplified implementation
    // In a real implementation, this would draw rounded rectangle to surface
    isDirty_ = true;
}

void Canvas::drawCircle(const Point& center, double radius, const Paint& paint) {
    if (!surface_) return;
    
    // Draw circle to surface
    // This is a simplified implementation
    // In a real implementation, this would draw circle to surface
    isDirty_ = true;
}

void Canvas::drawOval(const Rect& rect, const Paint& paint) {
    if (!surface_) return;
    
    // Draw oval to surface
    // This is a simplified implementation
    // In a real implementation, this would draw oval to surface
    isDirty_ = true;
}

void Canvas::drawArc(const Rect& rect, double startAngle, double sweepAngle, bool useCenter, const Paint& paint) {
    if (!surface_) return;
    
    // Draw arc to surface
    // This is a simplified implementation
    // In a real implementation, this would draw arc to surface
    isDirty_ = true;
}

void Canvas::drawPath(const Path& path, const Paint& paint) {
    if (!surface_) return;
    
    // Draw path to surface
    // This is a simplified implementation
    // In a real implementation, this would draw path to surface
    isDirty_ = true;
}

void Canvas::drawLine(const Point& start, const Point& end, const Paint& paint) {
    if (!surface_) return;
    
    // Draw line to surface
    // This is a simplified implementation
    // In a real implementation, this would draw line to surface
    isDirty_ = true;
}

void Canvas::drawPoint(const Point& point, const Paint& paint) {
    if (!surface_) return;
    
    // Draw point to surface
    // This is a simplified implementation
    // In a real implementation, this would draw point to surface
    isDirty_ = true;
}

void Canvas::drawPoints(const std::vector<Point>& points, const Paint& paint) {
    if (!surface_) return;
    
    // Draw points to surface
    // This is a simplified implementation
    // In a real implementation, this would draw points to surface
    isDirty_ = true;
}

void Canvas::drawPoints(const std::vector<Point>& points, PointMode mode, const Paint& paint) {
    if (!surface_) return;
    
    // Draw points to surface with mode
    // This is a simplified implementation
    // In a real implementation, this would draw points to surface with mode
    isDirty_ = true;
}

void Canvas::drawImage(const Image& image, const Point& point) {
    if (!surface_) return;
    
    // Draw image to surface
    // This is a simplified implementation
    // In a real implementation, this would draw image to surface
    isDirty_ = true;
}

void Canvas::drawImage(const Image& image, const Point& point, const Paint& paint) {
    if (!surface_) return;
    
    // Draw image to surface with paint
    // This is a simplified implementation
    // In a real implementation, this would draw image to surface with paint
    isDirty_ = true;
}

void Canvas::drawImage(const Image& image, const Rect& destRect) {
    if (!surface_) return;
    
    // Draw image to surface with destination rectangle
    // This is a simplified implementation
    // In a real implementation, this would draw image to surface with destination rectangle
    isDirty_ = true;
}

void Canvas::drawImage(const Image& image, const Rect& destRect, const Paint& paint) {
    if (!surface_) return;
    
    // Draw image to surface with destination rectangle and paint
    // This is a simplified implementation
    // In a real implementation, this would draw image to surface with destination rectangle and paint
    isDirty_ = true;
}

void Canvas::drawImage(const Image& image, const Rect& srcRect, const Rect& destRect) {
    if (!surface_) return;
    
    // Draw image to surface with source and destination rectangles
    // This is a simplified implementation
    // In a real implementation, this would draw image to surface with source and destination rectangles
    isDirty_ = true;
}

void Canvas::drawImage(const Image& image, const Rect& srcRect, const Rect& destRect, const Paint& paint) {
    if (!surface_) return;
    
    // Draw image to surface with source and destination rectangles and paint
    // This is a simplified implementation
    // In a real implementation, this would draw image to surface with source and destination rectangles and paint
    isDirty_ = true;
}

void Canvas::drawImageRect(const Image& image, const Rect& srcRect, const Rect& destRect, const Paint& paint) {
    drawImage(image, srcRect, destRect, paint);
}

void Canvas::drawImageRect(const Image& image, const Rect& srcRect, const Rect& destRect, const Paint& paint, SrcRectConstraint constraint) {
    // This is a simplified implementation
    // In a real implementation, this would handle source rectangle constraint
    drawImage(image, srcRect, destRect, paint);
}

void Canvas::drawText(const std::string& text, const Point& point, const Paint& paint) {
    if (!surface_) return;
    
    // Draw text to surface
    // This is a simplified implementation
    // In a real implementation, this would draw text to surface
    isDirty_ = true;
}

void Canvas::drawText(const std::string& text, double x, double y, const Paint& paint) {
    drawText(text, Point(x, y), paint);
}

void Canvas::drawText(const std::string& text, const Point& point, const Paint& paint, const Rect& bounds) {
    if (!surface_) return;
    
    // Draw text to surface with bounds
    // This is a simplified implementation
    // In a real implementation, this would draw text to surface with bounds
    isDirty_ = true;
}

void Canvas::drawText(const std::string& text, double x, double y, const Paint& paint, const Rect& bounds) {
    drawText(text, Point(x, y), paint, bounds);
}

void Canvas::drawTextOnPath(const std::string& text, const Path& path, const Paint& paint) {
    if (!surface_) return;
    
    // Draw text on path to surface
    // This is a simplified implementation
    // In a real implementation, this would draw text on path to surface
    isDirty_ = true;
}

void Canvas::drawTextOnPath(const std::string& text, const Path& path, double hOffset, double vOffset, const Paint& paint) {
    if (!surface_) return;
    
    // Draw text on path to surface with offset
    // This is a simplified implementation
    // In a real implementation, this would draw text on path to surface with offset
    isDirty_ = true;
}

void Canvas::drawTextBlob(const TextBlob& blob, const Point& point, const Paint& paint) {
    if (!surface_) return;
    
    // Draw text blob to surface
    // This is a simplified implementation
    // In a real implementation, this would draw text blob to surface
    isDirty_ = true;
}

void Canvas::drawTextBlob(const TextBlob& blob, double x, double y, const Paint& paint) {
    drawTextBlob(blob, Point(x, y), paint);
}

void Canvas::drawGradient(const Gradient& gradient, const Rect& rect, const Paint& paint) {
    if (!surface_) return;
    
    // Draw gradient to surface
    // This is a simplified implementation
    // In a real implementation, this would draw gradient to surface
    isDirty_ = true;
}

void Canvas::drawGradient(const Gradient& gradient, const Path& path, const Paint& paint) {
    if (!surface_) return;
    
    // Draw gradient to surface
    // This is a simplified implementation
    // In a real implementation, this would draw gradient to surface
    isDirty_ = true;
}

void Canvas::drawPattern(const Pattern& pattern, const Rect& rect, const Paint& paint) {
    if (!surface_) return;
    
    // Draw pattern to surface
    // This is a simplified implementation
    // In a real implementation, this would draw pattern to surface
    isDirty_ = true;
}

void Canvas::drawPattern(const Pattern& pattern, const Path& path, const Paint& paint) {
    if (!surface_) return;
    
    // Draw pattern to surface
    // This is a simplified implementation
    // In a real implementation, this would draw pattern to surface
    isDirty_ = true;
}

void Canvas::drawShader(const Shader& shader, const Rect& rect, const Paint& paint) {
    if (!surface_) return;
    
    // Draw shader to surface
    // This is a simplified implementation
    // In a real implementation, this would draw shader to surface
    isDirty_ = true;
}

void Canvas::drawShader(const Shader& shader, const Path& path, const Paint& paint) {
    if (!surface_) return;
    
    // Draw shader to surface
    // This is a simplified implementation
    // In a real implementation, this would draw shader to surface
    isDirty_ = true;
}

void Canvas::drawLayer(const Layer& layer) {
    if (!surface_) return;
    
    // Draw layer to surface
    // This is a simplified implementation
    // In a real implementation, this would draw layer to surface
    isDirty_ = true;
}

void Canvas::drawLayer(const Layer& layer, const Point& point) {
    if (!surface_) return;
    
    // Draw layer to surface with point
    // This is a simplified implementation
    // In a real implementation, this would draw layer to surface with point
    isDirty_ = true;
}

void Canvas::drawLayer(const Layer& layer, const Rect& destRect) {
    if (!surface_) return;
    
    // Draw layer to surface with destination rectangle
    // This is a simplified implementation
    // In a real implementation, this would draw layer to surface with destination rectangle
    isDirty_ = true;
}

void Canvas::drawLayer(const Layer& layer, const Rect& srcRect, const Rect& destRect) {
    if (!surface_) return;
    
    // Draw layer to surface with source and destination rectangles
    // This is a simplified implementation
    // In a real implementation, this would draw layer to surface with source and destination rectangles
    isDirty_ = true;
}

void Canvas::drawCanvas(const Canvas& canvas) {
    if (!surface_) return;
    
    // Draw canvas to surface
    // This is a simplified implementation
    // In a real implementation, this would draw canvas to surface
    isDirty_ = true;
}

void Canvas::drawCanvas(const Canvas& canvas, const Point& point) {
    if (!surface_) return;
    
    // Draw canvas to surface with point
    // This is a simplified implementation
    // In a real implementation, this would draw canvas to surface with point
    isDirty_ = true;
}

void Canvas::drawCanvas(const Canvas& canvas, const Rect& destRect) {
    if (!surface_) return;
    
    // Draw canvas to surface with destination rectangle
    // This is a simplified implementation
    // In a real implementation, this would draw canvas to surface with destination rectangle
    isDirty_ = true;
}

void Canvas::drawCanvas(const Canvas& canvas, const Rect& srcRect, const Rect& destRect) {
    if (!surface_) return;
    
    // Draw canvas to surface with source and destination rectangles
    // This is a simplified implementation
    // In a real implementation, this would draw canvas to surface with source and destination rectangles
    isDirty_ = true;
}

void Canvas::clear() {
    if (!surface_) return;
    
    // Clear surface
    // This is a simplified implementation
    // In a real implementation, this would clear the surface
    isDirty_ = true;
}

void Canvas::clear(const Color& color) {
    if (!surface_) return;
    
    // Clear surface with color
    // This is a simplified implementation
    // In a real implementation, this would clear the surface with color
    isDirty_ = true;
}

void Canvas::clear(const Rect& rect) {
    if (!surface_) return;
    
    // Clear surface rectangle
    // This is a simplified implementation
    // In a real implementation, this would clear the surface rectangle
    isDirty_ = true;
}

void Canvas::clear(const Rect& rect, const Color& color) {
    if (!surface_) return;
    
    // Clear surface rectangle with color
    // This is a simplified implementation
    // In a real implementation, this would clear the surface rectangle with color
    isDirty_ = true;
}

void Canvas::fillRect(const Rect& rect, const Color& color) {
    if (!surface_) return;
    
    // Fill rectangle with color
    // This is a simplified implementation
    // In a real implementation, this would fill rectangle with color
    isDirty_ = true;
}

void Canvas::fillRoundRect(const Rect& rect, double rx, double ry, const Color& color) {
    if (!surface_) return;
    
    // Fill rounded rectangle with color
    // This is a simplified implementation
    // In a real implementation, this would fill rounded rectangle with color
    isDirty_ = true;
}

void Canvas::fillCircle(const Point& center, double radius, const Color& color) {
    if (!surface_) return;
    
    // Fill circle with color
    // This is a simplified implementation
    // In a real implementation, this would fill circle with color
    isDirty_ = true;
}

void Canvas::fillOval(const Rect& rect, const Color& color) {
    if (!surface_) return;
    
    // Fill oval with color
    // This is a simplified implementation
    // In a real implementation, this would fill oval with color
    isDirty_ = true;
}

void Canvas::fillArc(const Rect& rect, double startAngle, double sweepAngle, bool useCenter, const Color& color) {
    if (!surface_) return;
    
    // Fill arc with color
    // This is a simplified implementation
    // In a real implementation, this would fill arc with color
    isDirty_ = true;
}

void Canvas::fillPath(const Path& path, const Color& color) {
    if (!surface_) return;
    
    // Fill path with color
    // This is a simplified implementation
    // In a real implementation, this would fill path with color
    isDirty_ = true;
}

void Canvas::strokeRect(const Rect& rect, const Color& color) {
    if (!surface_) return;
    
    // Stroke rectangle with color
    // This is a simplified implementation
    // In a real implementation, this would stroke rectangle with color
    isDirty_ = true;
}

void Canvas::strokeRect(const Rect& rect, const Color& color, double strokeWidth) {
    if (!surface_) return;
    
    // Stroke rectangle with color and width
    // This is a simplified implementation
    // In a real implementation, this would stroke rectangle with color and width
    isDirty_ = true;
}

void Canvas::strokeRoundRect(const Rect& rect, double rx, double ry, const Color& color) {
    if (!surface_) return;
    
    // Stroke rounded rectangle with color
    // This is a simplified implementation
    // In a real implementation, this would stroke rounded rectangle with color
    isDirty_ = true;
}

void Canvas::strokeRoundRect(const Rect& rect, double rx, double ry, const Color& color, double strokeWidth) {
    if (!surface_) return;
    
    // Stroke rounded rectangle with color and width
    // This is a simplified implementation
    // In a real implementation, this would stroke rounded rectangle with color and width
    isDirty_ = true;
}

void Canvas::strokeCircle(const Point& center, double radius, const Color& color) {
    if (!surface_) return;
    
    // Stroke circle with color
    // This is a simplified implementation
    // In a real implementation, this would stroke circle with color
    isDirty_ = true;
}

void Canvas::strokeCircle(const Point& center, double radius, const Color& color, double strokeWidth) {
    if (!surface_) return;
    
    // Stroke circle with color and width
    // This is a simplified implementation
    // In a real implementation, this would stroke circle with color and width
    isDirty_ = true;
}

void Canvas::strokeOval(const Rect& rect, const Color& color) {
    if (!surface_) return;
    
    // Stroke oval with color
    // This is a simplified implementation
    // In a real implementation, this would stroke oval with color
    isDirty_ = true;
}

void Canvas::strokeOval(const Rect& rect, const Color& color, double strokeWidth) {
    if (!surface_) return;
    
    // Stroke oval with color and width
    // This is a simplified implementation
    // In a real implementation, this would stroke oval with color and width
    isDirty_ = true;
}

void Canvas::strokeArc(const Rect& rect, double startAngle, double sweepAngle, bool useCenter, const Color& color) {
    if (!surface_) return;
    
    // Stroke arc with color
    // This is a simplified implementation
    // In a real implementation, this would stroke arc with color
    isDirty_ = true;
}

void Canvas::strokeArc(const Rect& rect, double startAngle, double sweepAngle, bool useCenter, const Color& color, double strokeWidth) {
    if (!surface_) return;
    
    // Stroke arc with color and width
    // This is a simplified implementation
    // In a real implementation, this would stroke arc with color and width
    isDirty_ = true;
}

void Canvas::strokePath(const Path& path, const Color& color) {
    if (!surface_) return;
    
    // Stroke path with color
    // This is a simplified implementation
    // In a real implementation, this would stroke path with color
    isDirty_ = true;
}

void Canvas::strokePath(const Path& path, const Color& color, double strokeWidth) {
    if (!surface_) return;
    
    // Stroke path with color and width
    // This is a simplified implementation
    // In a real implementation, this would stroke path with color and width
    isDirty_ = true;
}

void Canvas::strokeLine(const Point& start, const Point& end, const Color& color) {
    if (!surface_) return;
    
    // Stroke line with color
    // This is a simplified implementation
    // In a real implementation, this would stroke line with color
    isDirty_ = true;
}

void Canvas::strokeLine(const Point& start, const Point& end, const Color& color, double strokeWidth) {
    if (!surface_) return;
    
    // Stroke line with color and width
    // This is a simplified implementation
    // In a real implementation, this would stroke line with color and width
    isDirty_ = true;
}

Size Canvas::measureText(const std::string& text, const Paint& paint) const {
    // This is a simplified implementation
    // In a real implementation, this would measure text
    return Size(text.length() * 10, 20); // Approximate size
}

double Canvas::measureTextWidth(const std::string& text, const Paint& paint) const {
    return measureText(text, paint).width;
}

double Canvas::measureTextHeight(const std::string& text, const Paint& paint) const {
    return measureText(text, paint).height;
}

Rect Canvas::getTextBounds(const std::string& text, const Paint& paint) const {
    // This is a simplified implementation
    // In a real implementation, this would get text bounds
    Size size = measureText(text, paint);
    return Rect(0, 0, size.width, size.height);
}

Rect Canvas::getTextBounds(const std::string& text, const Point& point, const Paint& paint) const {
    // This is a simplified implementation
    // In a real implementation, this would get text bounds at point
    Size size = measureText(text, paint);
    return Rect(point.x, point.y, size.width, size.height);
}

std::vector<Rect> Canvas::getTextRuns(const std::string& text, const Paint& paint) const {
    // This is a simplified implementation
    // In a real implementation, this would get text runs
    std::vector<Rect> runs;
    Size size = measureText(text, paint);
    runs.push_back(Rect(0, 0, size.width, size.height));
    return runs;
}

std::vector<Rect> Canvas::getTextRuns(const std::string& text, const Point& point, const Paint& paint) const {
    // This is a simplified implementation
    // In a real implementation, this would get text runs at point
    std::vector<Rect> runs;
    Size size = measureText(text, paint);
    runs.push_back(Rect(point.x, point.y, size.width, size.height));
    return runs;
}

bool Canvas::containsPoint(const Point& point) const {
    return surface_ ? surface_->containsPoint(point) : false;
}

bool Canvas::containsRect(const Rect& rect) const {
    return surface_ ? surface_->containsRect(rect) : false;
}

bool Canvas::intersectsRect(const Rect& rect) const {
    return surface_ ? surface_->intersectsRect(rect) : false;
}

Point Canvas::mapPoint(const Point& point) const {
    return currentMatrix_.transform(point);
}

Rect Canvas::mapRect(const Rect& rect) const {
    return currentMatrix_.transform(rect);
}

Point Canvas::unmapPoint(const Point& point) const {
    // This is a simplified implementation
    // In a real implementation, this would unmap point using inverse matrix
    return point;
}

Rect Canvas::unmapRect(const Rect& rect) const {
    // This is a simplified implementation
    // In a real implementation, this would unmap rect using inverse matrix
    return rect;
}

void Canvas::flush() {
    if (!surface_) return;
    
    // Flush surface
    // This is a simplified implementation
    // In a real implementation, this would flush the surface
    isDirty_ = false;
}

void Canvas::finish() {
    if (!surface_) return;
    
    // Finish surface
    // This is a simplified implementation
    // In a real implementation, this would finish the surface
    isDirty_ = false;
}

void Canvas::sync() {
    if (!surface_) return;
    
    // Sync surface
    // This is a simplified implementation
    // In a real implementation, this would sync the surface
    isDirty_ = false;
}

void Canvas::wait() {
    if (!surface_) return;
    
    // Wait for surface
    // This is a simplified implementation
    // In a real implementation, this would wait for the surface
    isDirty_ = false;
}

void Canvas::reset() {
    stateStack_.clear();
    currentState_ = CanvasState();
    currentPaint_ = Paint();
    currentMatrix_ = Matrix::identity();
    currentClip_ = Rect();
    isDirty_ = false;
    isValid_ = true;
    isReady_ = true;
}

void Canvas::invalidate() {
    isDirty_ = true;
    isValid_ = false;
}

void Canvas::validate() {
    isDirty_ = false;
    isValid_ = true;
    isReady_ = true;
}

bool Canvas::isValid() const {
    return isValid_;
}

bool Canvas::isReady() const {
    return isReady_;
}

bool Canvas::isDirty() const {
    return isDirty_;
}

bool Canvas::isEmpty() const {
    return surface_ ? surface_->isEmpty() : true;
}

bool Canvas::isFull() const {
    return surface_ ? surface_->isFull() : false;
}

bool Canvas::isClipped() const {
    return !currentClip_.isEmpty();
}

bool Canvas::isTransformed() const {
    return currentMatrix_ != Matrix::identity();
}

bool Canvas::isScaled() const {
    // This is a simplified implementation
    // In a real implementation, this would check if matrix has scaling
    return false;
}

bool Canvas::isRotated() const {
    // This is a simplified implementation
    // In a real implementation, this would check if matrix has rotation
    return false;
}

bool Canvas::isSkewed() const {
    // This is a simplified implementation
    // In a real implementation, this would check if matrix has skewing
    return false;
}

Size Canvas::size() const {
    return surface_ ? surface_->size() : Size(0, 0);
}

Rect Canvas::bounds() const {
    return surface_ ? surface_->bounds() : Rect();
}

Rect Canvas::clipBounds() const {
    return currentClip_;
}

Rect Canvas::visibleBounds() const {
    // This is a simplified implementation
    // In a real implementation, this would calculate visible bounds
    return bounds();
}

Rect Canvas::dirtyBounds() const {
    // This is a simplified implementation
    // In a real implementation, this would calculate dirty bounds
    return bounds();
}

Rect Canvas::invalidBounds() const {
    // This is a simplified implementation
    // In a real implementation, this would calculate invalid bounds
    return bounds();
}

double Canvas::width() const {
    return size().width;
}

double Canvas::height() const {
    return size().height;
}

double Canvas::scaleX() const {
    // This is a simplified implementation
    // In a real implementation, this would extract scale X from matrix
    return 1.0;
}

double Canvas::scaleY() const {
    // This is a simplified implementation
    // In a real implementation, this would extract scale Y from matrix
    return 1.0;
}

double Canvas::rotation() const {
    // This is a simplified implementation
    // In a real implementation, this would extract rotation from matrix
    return 0.0;
}

double Canvas::skewX() const {
    // This is a simplified implementation
    // In a real implementation, this would extract skew X from matrix
    return 0.0;
}

double Canvas::skewY() const {
    // This is a simplified implementation
    // In a real implementation, this would extract skew Y from matrix
    return 0.0;
}

Point Canvas::translation() const {
    // This is a simplified implementation
    // In a real implementation, this would extract translation from matrix
    return Point(0, 0);
}

Point Canvas::scale() const {
    return Point(scaleX(), scaleY());
}

Point Canvas::skew() const {
    return Point(skewX(), skewY());
}

Canvas::Canvas(const Canvas& other) {
    copyFrom(other);
}

Canvas& Canvas::operator=(const Canvas& other) {
    if (this != &other) {
        copyFrom(other);
    }
    return *this;
}

Canvas::Canvas(Canvas&& other) noexcept {
    moveFrom(std::move(other));
}

Canvas& Canvas::operator=(Canvas&& other) noexcept {
    if (this != &other) {
        moveFrom(std::move(other));
    }
    return *this;
}

bool Canvas::operator==(const Canvas& other) const {
    return surface_ == other.surface_ &&
           context_ == other.context_ &&
           device_ == other.device_ &&
           backend_ == other.backend_ &&
           currentState_ == other.currentState_ &&
           currentPaint_ == other.currentPaint_ &&
           currentMatrix_ == other.currentMatrix_ &&
           currentClip_ == other.currentClip_ &&
           isDirty_ == other.isDirty_ &&
           isValid_ == other.isValid_ &&
           isReady_ == other.isReady_;
}

std::unique_ptr<Canvas> Canvas::clone() const {
    return std::make_unique<Canvas>(*this);
}

std::unique_ptr<Canvas> Canvas::fromSurface(std::shared_ptr<Surface> surface) {
    return std::make_unique<Canvas>(surface);
}

std::unique_ptr<Canvas> Canvas::fromImage(std::shared_ptr<Image> image) {
    // This is a simplified implementation
    // In a real implementation, this would create canvas from image
    return nullptr;
}

std::unique_ptr<Canvas> Canvas::fromContext(std::shared_ptr<Context> context) {
    auto canvas = std::make_unique<Canvas>();
    canvas->setContext(context);
    return canvas;
}

std::unique_ptr<Canvas> Canvas::fromDevice(std::shared_ptr<Device> device) {
    auto canvas = std::make_unique<Canvas>();
    canvas->setDevice(device);
    return canvas;
}

std::unique_ptr<Canvas> Canvas::fromBackend(std::shared_ptr<Backend> backend) {
    auto canvas = std::make_unique<Canvas>();
    canvas->setBackend(backend);
    return canvas;
}

void Canvas::copyFrom(const Canvas& other) {
    surface_ = other.surface_;
    context_ = other.context_;
    device_ = other.device_;
    backend_ = other.backend_;
    stateStack_ = other.stateStack_;
    currentState_ = other.currentState_;
    currentPaint_ = other.currentPaint_;
    currentMatrix_ = other.currentMatrix_;
    currentClip_ = other.currentClip_;
    isDirty_ = other.isDirty_;
    isValid_ = other.isValid_;
    isReady_ = other.isReady_;
}

void Canvas::moveFrom(Canvas&& other) {
    surface_ = std::move(other.surface_);
    context_ = std::move(other.context_);
    device_ = std::move(other.device_);
    backend_ = std::move(other.backend_);
    stateStack_ = std::move(other.stateStack_);
    currentState_ = std::move(other.currentState_);
    currentPaint_ = std::move(other.currentPaint_);
    currentMatrix_ = std::move(other.currentMatrix_);
    currentClip_ = std::move(other.currentClip_);
    isDirty_ = other.isDirty_;
    isValid_ = other.isValid_;
    isReady_ = other.isReady_;
}

void Canvas::cleanup() {
    surface_.reset();
    context_.reset();
    device_.reset();
    backend_.reset();
    stateStack_.clear();
    currentState_ = CanvasState();
    currentPaint_ = Paint();
    currentMatrix_ = Matrix::identity();
    currentClip_ = Rect();
    isDirty_ = false;
    isValid_ = false;
    isReady_ = false;
}

void Canvas::initialize() {
    currentState_ = CanvasState();
    currentPaint_ = Paint();
    currentMatrix_ = Matrix::identity();
    currentClip_ = Rect();
    isDirty_ = false;
    isValid_ = true;
    isReady_ = true;
}

void Canvas::updateState() {
    currentState_.matrix = currentMatrix_;
    currentState_.clip = currentClip_;
    currentState_.paint = currentPaint_;
    currentState_.isDirty = isDirty_;
    currentState_.isValid = isValid_;
    currentState_.isReady = isReady_;
}

void Canvas::validateState() {
    isValid_ = true;
    isReady_ = true;
    updateState();
}

void Canvas::invalidateState() {
    isValid_ = false;
    isReady_ = false;
    updateState();
}

void Canvas::resetState() {
    currentMatrix_ = Matrix::identity();
    currentClip_ = Rect();
    currentPaint_ = Paint();
    isDirty_ = false;
    isValid_ = true;
    isReady_ = true;
    updateState();
}

void Canvas::pushState() {
    updateState();
}

void Canvas::popState() {
    if (!stateStack_.empty()) {
        currentState_ = stateStack_.back();
        stateStack_.pop_back();
        applyState(currentState_);
    }
}

void Canvas::applyState(const CanvasState& state) {
    currentMatrix_ = state.matrix;
    currentClip_ = state.clip;
    currentPaint_ = state.paint;
    isDirty_ = state.isDirty;
    isValid_ = state.isValid;
    isReady_ = state.isReady;
}

void Canvas::updateMatrix() {
    updateState();
}

void Canvas::updateClip() {
    updateState();
}

void Canvas::updatePaint() {
    updateState();
}

void Canvas::updateSurface() {
    if (surface_) {
        updateState();
    }
}

void Canvas::updateContext() {
    if (context_) {
        updateState();
    }
}

void Canvas::updateDevice() {
    if (device_) {
        updateState();
    }
}

void Canvas::updateBackend() {
    if (backend_) {
        updateState();
    }
}

} // namespace renderer
