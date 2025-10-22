#include "layout/layout_engine.h"
#include "layout/block_layout.h"
#include "layout/flexbox_layout.h"
#include "layout/grid_layout.h"
#include "layout/inline_layout.h"
#include "layout/positioned_layout.h"
#include <algorithm>

namespace layout {

// LayoutEngine implementation
LayoutEngine::LayoutEngine() : viewport_(0, 0, 1024, 768) {
}

LayoutEngine::~LayoutEngine() = default;

void LayoutEngine::setTree(std::unique_ptr<LayoutTree> tree) {
    tree_ = std::move(tree);
}

void LayoutEngine::layout() {
    if (!tree_ || !tree_->root()) return;
    
    // Create layout constraints based on viewport
    LayoutConstraints constraints(
        Size(0, 0),
        Size(viewport_.width, viewport_.height)
    );
    
    // Layout the entire tree
    tree_->layout(constraints);
}

void LayoutEngine::updateLayout() {
    if (!tree_) return;
    
    // Update layout for dirty nodes only
    tree_->updateLayout();
}

void LayoutEngine::invalidateLayout() {
    if (!tree_) return;
    
    // Invalidate layout for all nodes
    tree_->invalidateLayout();
}

LayoutNode* LayoutEngine::hitTest(const Point& point) const {
    if (!tree_ || !tree_->root()) return nullptr;
    
    return tree_->root()->hitTest(point);
}

Rect LayoutEngine::getLayoutBounds() const {
    if (!tree_ || !tree_->root()) return Rect();
    
    return tree_->root()->getBounds();
}

} // namespace layout
