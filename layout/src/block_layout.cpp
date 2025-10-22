#include "layout/block_layout.h"
#include <algorithm>
#include <cmath>

namespace layout {

// BlockLayout implementation
BlockLayout::BlockLayout() = default;

BlockLayout::~BlockLayout() = default;

void BlockLayout::layoutBlock(LayoutNode* node, const LayoutConstraints& constraints) {
    if (!node || !node->box()) return;
    
    // Calculate block size
    Size blockSize = calculateBlockSize(node, constraints);
    
    // Set layout rect
    node->setLayoutRect(Rect(0, 0, blockSize.width, blockSize.height));
    
    // Layout block children
    layoutBlockChildren(node, constraints);
    
    // Handle margin collapse
    collapseMargins(node);
    
    // Handle floats
    handleFloats(node);
}

void BlockLayout::layoutBlockChildren(LayoutNode* node, const LayoutConstraints& constraints) {
    if (!node) return;
    
    for (auto* child : node->children()) {
        if (child && child->isBlockLevel()) {
            layoutBlockChild(child, constraints);
        }
    }
}

Size BlockLayout::calculateBlockSize(LayoutNode* node, const LayoutConstraints& constraints) {
    if (!node || !node->box()) return Size(0, 0);
    
    // Get box model properties
    const EdgeInsets& padding = node->box()->padding();
    const EdgeInsets& border = node->box()->border();
    const EdgeInsets& margin = node->box()->margin();
    
    // Calculate content size
    Size contentSize = node->calculateIntrinsicSize();
    
    // Apply box sizing
    Size totalSize;
    if (node->box()->boxSizing() == BoxSizing::BorderBox) {
        totalSize = contentSize;
    } else {
        totalSize = Size(
            contentSize.width + padding.horizontal() + border.horizontal(),
            contentSize.height + padding.vertical() + border.vertical()
        );
    }
    
    // Constrain size
    totalSize = constraints.constrain(totalSize);
    
    return totalSize;
}

Point BlockLayout::calculateBlockPosition(LayoutNode* node) {
    if (!node || !node->parent()) return Point(0, 0);
    
    // Calculate position based on parent's layout
    Point position = Point(0, 0);
    
    // Add margins
    const EdgeInsets& margin = node->box()->margin();
    position.x += margin.left;
    position.y += margin.top;
    
    return position;
}

void BlockLayout::collapseMargins(LayoutNode* node) {
    if (!node) return;
    
    // Collapse margins with parent
    collapseMarginsWithParent(node);
    
    // Collapse margins with children
    collapseMarginsWithChildren(node);
}

void BlockLayout::handleFloats(LayoutNode* node) {
    if (!node) return;
    
    // Handle floating children
    for (auto* child : node->children()) {
        if (child && child->isFloating()) {
            addFloat(node, child);
        }
    }
}

void BlockLayout::clearFloats(LayoutNode* node, Clear clear) {
    if (!node) return;
    
    switch (clear) {
        case Clear::None:
            break;
        case Clear::Left:
            clearFloatsLeft(node);
            break;
        case Clear::Right:
            clearFloatsRight(node);
            break;
        case Clear::Both:
            clearFloatsBoth(node);
            break;
    }
}

void BlockLayout::layoutBlockChild(LayoutNode* child, const LayoutConstraints& constraints) {
    if (!child) return;
    
    // Create constraints for child
    LayoutConstraints childConstraints = constraints;
    
    // Layout the child
    child->layout(childConstraints);
    
    // Position the child
    positionBlockChild(child);
}

void BlockLayout::positionBlockChild(LayoutNode* child) {
    if (!child || !child->parent()) return;
    
    // Calculate position
    Point position = calculateBlockPosition(child);
    
    // Update child position
    child->updatePosition(position);
}

void BlockLayout::collapseMarginsWithParent(LayoutNode* node) {
    if (!node || !node->parent() || !node->box()) return;
    
    // Simplified margin collapse implementation
    // In a real implementation, this would follow CSS margin collapse rules
    double collapsedTop = std::max(
        node->parent()->getCollapsedMarginBottom(),
        node->box()->margin().top
    );
    
    // Apply collapsed margin
    // This would be implemented by updating the node's position
}

void BlockLayout::collapseMarginsWithChildren(LayoutNode* node) {
    if (!node || !node->box()) return;
    
    // Simplified margin collapse implementation
    // In a real implementation, this would follow CSS margin collapse rules
    double collapsedBottom = 0;
    
    for (auto* child : node->children()) {
        if (child && child->box()) {
            collapsedBottom = std::max(collapsedBottom, child->box()->margin().bottom);
        }
    }
    
    // Apply collapsed margin
    // This would be implemented by updating the node's size
}

void BlockLayout::addFloat(LayoutNode* node, LayoutNode* floatNode) {
    if (!node || !floatNode) return;
    
    // Add float to node's float list
    node->addFloat(floatNode);
    
    // Position the float
    // This would be implemented by the float positioning algorithm
}

void BlockLayout::removeFloat(LayoutNode* node, LayoutNode* floatNode) {
    if (!node || !floatNode) return;
    
    // Remove float from node's float list
    node->removeFloat(floatNode);
}

void BlockLayout::clearFloatsLeft(LayoutNode* node) {
    if (!node) return;
    
    // Clear left floats
    // This would be implemented by the float clearing algorithm
}

void BlockLayout::clearFloatsRight(LayoutNode* node) {
    if (!node) return;
    
    // Clear right floats
    // This would be implemented by the float clearing algorithm
}

void BlockLayout::clearFloatsBoth(LayoutNode* node) {
    if (!node) return;
    
    // Clear both left and right floats
    clearFloatsLeft(node);
    clearFloatsRight(node);
}

} // namespace layout
