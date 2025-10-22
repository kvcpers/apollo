#pragma once

#include "layout_node.h"
#include "types.h"
#include <memory>
#include <vector>

namespace layout {

// Main layout engine that orchestrates the layout process
class LayoutEngine {
public:
    LayoutEngine();
    ~LayoutEngine();

    // Layout tree
    LayoutTree* tree() const { return tree_.get(); }
    void setTree(std::unique_ptr<LayoutTree> tree);

    // Viewport
    const Rect& viewport() const { return viewport_; }
    void setViewport(const Rect& viewport) { viewport_ = viewport; }

    // Layout the entire tree
    void layout();

    // Update layout for dirty nodes
    void updateLayout();

    // Invalidate layout
    void invalidateLayout();

    // Hit testing
    LayoutNode* hitTest(const Point& point) const;

    // Get layout bounds
    Rect getLayoutBounds() const;

private:
    std::unique_ptr<LayoutTree> tree_;
    Rect viewport_;
};

} // namespace layout
