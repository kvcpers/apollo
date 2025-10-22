#include "renderer/renderer.h"
#include <algorithm>
#include <cmath>

namespace renderer {

// Renderer implementation
Renderer::Renderer()
    : backend_(nullptr)
    , device_(nullptr)
    , context_(nullptr)
    , surface_(nullptr)
    , stateStack_()
    , currentState_()
    , currentMatrix_(Matrix::identity())
    , currentClip_(Rect())
    , isDirty_(false)
    , isValid_(false)
    , isReady_(false) {
    initialize();
}

Renderer::Renderer(std::shared_ptr<Backend> backend)
    : backend_(backend)
    , device_(nullptr)
    , context_(nullptr)
    , surface_(nullptr)
    , stateStack_()
    , currentState_()
    , currentMatrix_(Matrix::identity())
    , currentClip_(Rect())
    , isDirty_(false)
    , isValid_(false)
    , isReady_(false) {
    initialize();
}

Renderer::~Renderer() {
    cleanup();
}

void Renderer::setBackend(std::shared_ptr<Backend> backend) {
    backend_ = backend;
    updateBackend();
}

void Renderer::setDevice(std::shared_ptr<Device> device) {
    device_ = device;
    updateDevice();
}

void Renderer::setContext(std::shared_ptr<Context> context) {
    context_ = context;
    updateContext();
}

void Renderer::setSurface(std::shared_ptr<Surface> surface) {
    surface_ = surface;
    updateSurface();
}

std::unique_ptr<Canvas> Renderer::createCanvas() {
    if (!surface_) {
        return nullptr;
    }
    return std::make_unique<Canvas>(surface_);
}

std::unique_ptr<Canvas> Renderer::createCanvas(std::shared_ptr<Surface> surface) {
    return std::make_unique<Canvas>(surface);
}

std::unique_ptr<Canvas> Renderer::createCanvas(const Size& size) {
    // Create surface with specified size
    auto surface = createSurface(size);
    if (!surface) {
        return nullptr;
    }
    return std::make_unique<Canvas>(surface);
}

std::unique_ptr<Canvas> Renderer::createCanvas(int width, int height) {
    return createCanvas(Size(width, height));
}

std::unique_ptr<Paint> Renderer::createPaint() {
    return std::make_unique<Paint>();
}

std::unique_ptr<Paint> Renderer::createPaint(const Color& color) {
    auto paint = std::make_unique<Paint>();
    paint->setColor(color);
    return paint;
}

std::unique_ptr<Paint> Renderer::createPaint(const Paint& paint) {
    return std::make_unique<Paint>(paint);
}

void Renderer::render(Canvas* canvas) {
    if (!canvas || !isValid_) return;
    
    // Render canvas to surface
    // This is a simplified implementation
    // In a real implementation, this would handle the actual rendering
    isDirty_ = true;
}

void Renderer::render(Canvas* canvas, const Rect& bounds) {
    if (!canvas || !isValid_) return;
    
    // Render canvas to surface with bounds
    // This is a simplified implementation
    // In a real implementation, this would handle the actual rendering
    isDirty_ = true;
}

void Renderer::clear() {
    if (!surface_) return;
    
    // Clear surface
    // This is a simplified implementation
    // In a real implementation, this would clear the surface
    isDirty_ = true;
}

void Renderer::clear(const Color& color) {
    if (!surface_) return;
    
    // Clear surface with color
    // This is a simplified implementation
    // In a real implementation, this would clear the surface with color
    isDirty_ = true;
}

void Renderer::clear(const Rect& rect) {
    if (!surface_) return;
    
    // Clear surface rectangle
    // This is a simplified implementation
    // In a real implementation, this would clear the surface rectangle
    isDirty_ = true;
}

void Renderer::clear(const Rect& rect, const Color& color) {
    if (!surface_) return;
    
    // Clear surface rectangle with color
    // This is a simplified implementation
    // In a real implementation, this would clear the surface rectangle with color
    isDirty_ = true;
}

void Renderer::flush() {
    if (!surface_) return;
    
    // Flush surface
    // This is a simplified implementation
    // In a real implementation, this would flush the surface
    isDirty_ = false;
}

void Renderer::finish() {
    if (!surface_) return;
    
    // Finish surface
    // This is a simplified implementation
    // In a real implementation, this would finish the surface
    isDirty_ = false;
}

void Renderer::sync() {
    if (!surface_) return;
    
    // Sync surface
    // This is a simplified implementation
    // In a real implementation, this would sync the surface
    isDirty_ = false;
}

void Renderer::wait() {
    if (!surface_) return;
    
    // Wait for surface
    // This is a simplified implementation
    // In a real implementation, this would wait for the surface
    isDirty_ = false;
}

void Renderer::save() {
    stateStack_.push_back(currentState_);
    pushState();
}

void Renderer::restore() {
    if (!stateStack_.empty()) {
        currentState_ = stateStack_.back();
        stateStack_.pop_back();
        applyState(currentState_);
    }
}

void Renderer::reset() {
    stateStack_.clear();
    currentState_ = RendererState();
    currentMatrix_ = Matrix::identity();
    currentClip_ = Rect();
    isDirty_ = false;
    isValid_ = true;
    isReady_ = true;
}

void Renderer::translate(double dx, double dy) {
    currentMatrix_ = currentMatrix_ * Matrix::translation(dx, dy);
    updateMatrix();
}

void Renderer::scale(double sx, double sy) {
    currentMatrix_ = currentMatrix_ * Matrix::scale(sx, sy);
    updateMatrix();
}

void Renderer::rotate(double angle) {
    currentMatrix_ = currentMatrix_ * Matrix::rotation(angle);
    updateMatrix();
}

void Renderer::setMatrix(const Matrix& matrix) {
    currentMatrix_ = matrix;
    updateMatrix();
}

Matrix Renderer::getMatrix() const {
    return currentMatrix_;
}

void Renderer::clipRect(const Rect& rect) {
    if (currentClip_.isEmpty()) {
        currentClip_ = rect;
    } else {
        currentClip_ = currentClip_.intersection(rect);
    }
    updateClip();
}

void Renderer::clipPath(const Path& path) {
    // This is a simplified implementation
    // In a real implementation, this would handle path clipping
    updateClip();
}

Rect Renderer::getClipBounds() const {
    return currentClip_;
}

bool Renderer::isClipEmpty() const {
    return currentClip_.isEmpty();
}

void Renderer::drawColor(const Color& color) {
    if (!surface_) return;
    
    // Draw color to surface
    // This is a simplified implementation
    // In a real implementation, this would draw color to surface
    isDirty_ = true;
}

void Renderer::drawPaint(const Paint& paint) {
    if (!surface_) return;
    
    // Draw paint to surface
    // This is a simplified implementation
    // In a real implementation, this would draw paint to surface
    isDirty_ = true;
}

void Renderer::drawRect(const Rect& rect, const Paint& paint) {
    if (!surface_) return;
    
    // Draw rectangle to surface
    // This is a simplified implementation
    // In a real implementation, this would draw rectangle to surface
    isDirty_ = true;
}

void Renderer::drawCircle(const Point& center, double radius, const Paint& paint) {
    if (!surface_) return;
    
    // Draw circle to surface
    // This is a simplified implementation
    // In a real implementation, this would draw circle to surface
    isDirty_ = true;
}

void Renderer::drawPath(const Path& path, const Paint& paint) {
    if (!surface_) return;
    
    // Draw path to surface
    // This is a simplified implementation
    // In a real implementation, this would draw path to surface
    isDirty_ = true;
}

void Renderer::drawImage(const Image& image, const Point& point) {
    if (!surface_) return;
    
    // Draw image to surface
    // This is a simplified implementation
    // In a real implementation, this would draw image to surface
    isDirty_ = true;
}

void Renderer::drawText(const std::string& text, const Point& point, const Paint& paint) {
    if (!surface_) return;
    
    // Draw text to surface
    // This is a simplified implementation
    // In a real implementation, this would draw text to surface
    isDirty_ = true;
}

Size Renderer::measureText(const std::string& text, const Paint& paint) const {
    // This is a simplified implementation
    // In a real implementation, this would measure text
    return Size(text.length() * 10, 20); // Approximate size
}

Rect Renderer::getTextBounds(const std::string& text, const Paint& paint) const {
    // This is a simplified implementation
    // In a real implementation, this would get text bounds
    Size size = measureText(text, paint);
    return Rect(0, 0, size.width, size.height);
}

void Renderer::invalidate() {
    isDirty_ = true;
    isValid_ = false;
}

void Renderer::validate() {
    isDirty_ = false;
    isValid_ = true;
    isReady_ = true;
}

bool Renderer::isValid() const {
    return isValid_;
}

bool Renderer::isReady() const {
    return isReady_;
}

bool Renderer::isDirty() const {
    return isDirty_;
}

Size Renderer::size() const {
    return surface_ ? surface_->size() : Size(0, 0);
}

Rect Renderer::bounds() const {
    return surface_ ? surface_->bounds() : Rect();
}

double Renderer::width() const {
    return size().width;
}

double Renderer::height() const {
    return size().height;
}

Renderer::Renderer(const Renderer& other) {
    copyFrom(other);
}

Renderer& Renderer::operator=(const Renderer& other) {
    if (this != &other) {
        copyFrom(other);
    }
    return *this;
}

Renderer::Renderer(Renderer&& other) noexcept {
    moveFrom(std::move(other));
}

Renderer& Renderer::operator=(Renderer&& other) noexcept {
    if (this != &other) {
        moveFrom(std::move(other));
    }
    return *this;
}

bool Renderer::operator==(const Renderer& other) const {
    return backend_ == other.backend_ &&
           device_ == other.device_ &&
           context_ == other.context_ &&
           surface_ == other.surface_ &&
           currentState_ == other.currentState_ &&
           currentMatrix_ == other.currentMatrix_ &&
           currentClip_ == other.currentClip_ &&
           isDirty_ == other.isDirty_ &&
           isValid_ == other.isValid_ &&
           isReady_ == other.isReady_;
}

std::unique_ptr<Renderer> Renderer::clone() const {
    return std::make_unique<Renderer>(*this);
}

std::unique_ptr<Renderer> Renderer::fromBackend(std::shared_ptr<Backend> backend) {
    return std::make_unique<Renderer>(backend);
}

std::unique_ptr<Renderer> Renderer::fromDevice(std::shared_ptr<Device> device) {
    auto renderer = std::make_unique<Renderer>();
    renderer->setDevice(device);
    return renderer;
}

std::unique_ptr<Renderer> Renderer::fromContext(std::shared_ptr<Context> context) {
    auto renderer = std::make_unique<Renderer>();
    renderer->setContext(context);
    return renderer;
}

void Renderer::copyFrom(const Renderer& other) {
    backend_ = other.backend_;
    device_ = other.device_;
    context_ = other.context_;
    surface_ = other.surface_;
    stateStack_ = other.stateStack_;
    currentState_ = other.currentState_;
    currentMatrix_ = other.currentMatrix_;
    currentClip_ = other.currentClip_;
    isDirty_ = other.isDirty_;
    isValid_ = other.isValid_;
    isReady_ = other.isReady_;
}

void Renderer::moveFrom(Renderer&& other) {
    backend_ = std::move(other.backend_);
    device_ = std::move(other.device_);
    context_ = std::move(other.context_);
    surface_ = std::move(other.surface_);
    stateStack_ = std::move(other.stateStack_);
    currentState_ = std::move(other.currentState_);
    currentMatrix_ = std::move(other.currentMatrix_);
    currentClip_ = std::move(other.currentClip_);
    isDirty_ = other.isDirty_;
    isValid_ = other.isValid_;
    isReady_ = other.isReady_;
}

void Renderer::cleanup() {
    backend_.reset();
    device_.reset();
    context_.reset();
    surface_.reset();
    stateStack_.clear();
    currentState_ = RendererState();
    currentMatrix_ = Matrix::identity();
    currentClip_ = Rect();
    isDirty_ = false;
    isValid_ = false;
    isReady_ = false;
}

void Renderer::initialize() {
    currentState_ = RendererState();
    currentMatrix_ = Matrix::identity();
    currentClip_ = Rect();
    isDirty_ = false;
    isValid_ = true;
    isReady_ = true;
}

void Renderer::updateState() {
    currentState_.matrix = currentMatrix_;
    currentState_.clip = currentClip_;
    currentState_.isDirty = isDirty_;
    currentState_.isValid = isValid_;
    currentState_.isReady = isReady_;
}

void Renderer::validateState() {
    isValid_ = true;
    isReady_ = true;
    updateState();
}

void Renderer::invalidateState() {
    isValid_ = false;
    isReady_ = false;
    updateState();
}

void Renderer::resetState() {
    currentMatrix_ = Matrix::identity();
    currentClip_ = Rect();
    isDirty_ = false;
    isValid_ = true;
    isReady_ = true;
    updateState();
}

void Renderer::pushState() {
    updateState();
}

void Renderer::popState() {
    if (!stateStack_.empty()) {
        currentState_ = stateStack_.back();
        stateStack_.pop_back();
        applyState(currentState_);
    }
}

void Renderer::applyState(const RendererState& state) {
    currentMatrix_ = state.matrix;
    currentClip_ = state.clip;
    isDirty_ = state.isDirty;
    isValid_ = state.isValid;
    isReady_ = state.isReady;
}

void Renderer::updateMatrix() {
    updateState();
}

void Renderer::updateClip() {
    updateState();
}

void Renderer::updateBackend() {
    if (backend_) {
        updateState();
    }
}

void Renderer::updateDevice() {
    if (device_) {
        updateState();
    }
}

void Renderer::updateContext() {
    if (context_) {
        updateState();
    }
}

void Renderer::updateSurface() {
    if (surface_) {
        updateState();
    }
}

} // namespace renderer
