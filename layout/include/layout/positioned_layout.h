#pragma once

#include "layout_node.h"
#include "types.h"
#include <memory>
#include <vector>

namespace layout {

// Positioned layout algorithm implementation
class PositionedLayout {
public:
    PositionedLayout();
    ~PositionedLayout();

    // Layout positioned elements
    void layoutPositioned(LayoutNode* node, const LayoutConstraints& constraints);

    // Layout absolutely positioned elements
    void layoutAbsolute(LayoutNode* node, const LayoutConstraints& constraints);

    // Layout relatively positioned elements
    void layoutRelative(LayoutNode* node, const LayoutConstraints& constraints);

    // Layout fixed positioned elements
    void layoutFixed(LayoutNode* node, const LayoutConstraints& constraints);

    // Layout sticky positioned elements
    void layoutSticky(LayoutNode* node, const LayoutConstraints& constraints);

    // Calculate positioned size
    Size calculatePositionedSize(LayoutNode* node, const LayoutConstraints& constraints);

    // Calculate positioned position
    Point calculatePositionedPosition(LayoutNode* node);

    // Handle top property
    void handleTop(LayoutNode* node);

    // Handle right property
    void handleRight(LayoutNode* node);

    // Handle bottom property
    void handleBottom(LayoutNode* node);

    // Handle left property
    void handleLeft(LayoutNode* node);

    // Handle z-index
    void handleZIndex(LayoutNode* node);

    // Handle containing block
    void handleContainingBlock(LayoutNode* node);

    // Handle stacking context
    void handleStackingContext(LayoutNode* node);

    // Handle transform
    void handleTransform(LayoutNode* node);

    // Handle opacity
    void handleOpacity(LayoutNode* node);

    // Handle visibility
    void handleVisibility(LayoutNode* node);

    // Handle clip
    void handleClip(LayoutNode* node);

    // Handle overflow
    void handleOverflow(LayoutNode* node);

    // Calculate containing block
    LayoutNode* calculateContainingBlock(LayoutNode* node);

    // Calculate stacking context
    void calculateStackingContext(LayoutNode* node);

    // Calculate transform
    Transform calculateTransform(LayoutNode* node);

    // Calculate clip rectangle
    Rect calculateClipRect(LayoutNode* node);

    // Calculate overflow rectangle
    Rect calculateOverflowRect(LayoutNode* node);

private:
    void layoutPositionedChild(LayoutNode* child, const LayoutConstraints& constraints);
    void positionPositionedChild(LayoutNode* child);
    void layoutAbsoluteChild(LayoutNode* child, const LayoutConstraints& constraints);
    void layoutRelativeChild(LayoutNode* child, const LayoutConstraints& constraints);
    void layoutFixedChild(LayoutNode* child, const LayoutConstraints& constraints);
    void layoutStickyChild(LayoutNode* child, const LayoutConstraints& constraints);
    void calculatePositionedSize(LayoutNode* node);
    void calculatePositionedPosition(LayoutNode* node);
    void handleTopProperty(LayoutNode* node);
    void handleRightProperty(LayoutNode* node);
    void handleBottomProperty(LayoutNode* node);
    void handleLeftProperty(LayoutNode* node);
    void handleZIndexProperty(LayoutNode* node);
    void handleContainingBlockProperty(LayoutNode* node);
    void handleStackingContextProperty(LayoutNode* node);
    void handleTransformProperty(LayoutNode* node);
    void handleOpacityProperty(LayoutNode* node);
    void handleVisibilityProperty(LayoutNode* node);
    void handleClipProperty(LayoutNode* node);
    void handleOverflowProperty(LayoutNode* node);
    LayoutNode* calculateContainingBlockRecursive(LayoutNode* node);
    void calculateStackingContextRecursive(LayoutNode* node);
    Transform calculateTransformRecursive(LayoutNode* node);
    Rect calculateClipRectRecursive(LayoutNode* node);
    Rect calculateOverflowRectRecursive(LayoutNode* node);
};

} // namespace layout
