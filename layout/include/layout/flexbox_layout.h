#pragma once

#include "layout_node.h"
#include "types.h"
#include <memory>
#include <vector>

namespace layout {

// Flexbox layout algorithm implementation
class FlexboxLayout {
public:
    FlexboxLayout();
    ~FlexboxLayout();

    // Layout a flex container
    void layoutFlexContainer(LayoutNode* node, const LayoutConstraints& constraints);

    // Layout flex items
    void layoutFlexItems(LayoutNode* node, const LayoutConstraints& constraints);

    // Calculate flex container size
    Size calculateFlexContainerSize(LayoutNode* node, const LayoutConstraints& constraints);

    // Calculate flex item size
    Size calculateFlexItemSize(LayoutNode* item, const LayoutConstraints& constraints);

    // Calculate flex item position
    Point calculateFlexItemPosition(LayoutNode* item);

    // Handle flex direction
    void handleFlexDirection(LayoutNode* node);

    // Handle flex wrap
    void handleFlexWrap(LayoutNode* node);

    // Handle justify content
    void handleJustifyContent(LayoutNode* node);

    // Handle align items
    void handleAlignItems(LayoutNode* node);

    // Handle align content
    void handleAlignContent(LayoutNode* node);

    // Calculate flex basis
    double calculateFlexBasis(LayoutNode* item);

    // Calculate flex grow
    double calculateFlexGrow(LayoutNode* item);

    // Calculate flex shrink
    double calculateFlexShrink(LayoutNode* item);

private:
    void layoutFlexItem(LayoutNode* item, const LayoutConstraints& constraints);
    void positionFlexItem(LayoutNode* item);
    void distributeFlexSpace(LayoutNode* node);
    void alignFlexItems(LayoutNode* node);
    void wrapFlexItems(LayoutNode* node);
    void calculateFlexItemSizes(LayoutNode* node);
    void calculateFlexItemPositions(LayoutNode* node);
    void handleFlexWrapNowrap(LayoutNode* node);
    void handleFlexWrapWrap(LayoutNode* node);
    void handleFlexWrapWrapReverse(LayoutNode* node);
    void handleFlexDirectionRow(LayoutNode* node);
    void handleFlexDirectionRowReverse(LayoutNode* node);
    void handleFlexDirectionColumn(LayoutNode* node);
    void handleFlexDirectionColumnReverse(LayoutNode* node);
    void handleJustifyContentFlexStart(LayoutNode* node);
    void handleJustifyContentFlexEnd(LayoutNode* node);
    void handleJustifyContentCenter(LayoutNode* node);
    void handleJustifyContentSpaceBetween(LayoutNode* node);
    void handleJustifyContentSpaceAround(LayoutNode* node);
    void handleJustifyContentSpaceEvenly(LayoutNode* node);
    void handleAlignItemsStretch(LayoutNode* node);
    void handleAlignItemsFlexStart(LayoutNode* node);
    void handleAlignItemsFlexEnd(LayoutNode* node);
    void handleAlignItemsCenter(LayoutNode* node);
    void handleAlignItemsBaseline(LayoutNode* node);
    void handleAlignContentStretch(LayoutNode* node);
    void handleAlignContentFlexStart(LayoutNode* node);
    void handleAlignContentFlexEnd(LayoutNode* node);
    void handleAlignContentCenter(LayoutNode* node);
    void handleAlignContentSpaceBetween(LayoutNode* node);
    void handleAlignContentSpaceAround(LayoutNode* node);
};

} // namespace layout
