#pragma once

#include "layout_node.h"
#include "types.h"
#include <memory>
#include <vector>

namespace layout {

// Block layout algorithm implementation
class BlockLayout {
public:
    BlockLayout();
    ~BlockLayout();

    // Layout a block-level element
    void layoutBlock(LayoutNode* node, const LayoutConstraints& constraints);

    // Layout block children
    void layoutBlockChildren(LayoutNode* node, const LayoutConstraints& constraints);

    // Calculate block size
    Size calculateBlockSize(LayoutNode* node, const LayoutConstraints& constraints);

    // Calculate block position
    Point calculateBlockPosition(LayoutNode* node);

    // Handle margin collapse
    void collapseMargins(LayoutNode* node);

    // Handle floats
    void handleFloats(LayoutNode* node);

    // Clear floats
    void clearFloats(LayoutNode* node, Clear clear);

private:
    void layoutBlockChild(LayoutNode* child, const LayoutConstraints& constraints);
    void positionBlockChild(LayoutNode* child);
    void collapseMarginsWithParent(LayoutNode* node);
    void collapseMarginsWithChildren(LayoutNode* node);
    void addFloat(LayoutNode* node, LayoutNode* floatNode);
    void removeFloat(LayoutNode* node, LayoutNode* floatNode);
    void clearFloatsLeft(LayoutNode* node);
    void clearFloatsRight(LayoutNode* node);
    void clearFloatsBoth(LayoutNode* node);
};

} // namespace layout
