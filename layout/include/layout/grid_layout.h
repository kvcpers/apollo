#pragma once

#include "layout_node.h"
#include "types.h"
#include <memory>
#include <vector>

namespace layout {

// CSS Grid layout algorithm implementation
class GridLayout {
public:
    GridLayout();
    ~GridLayout();

    // Layout a grid container
    void layoutGridContainer(LayoutNode* node, const LayoutConstraints& constraints);

    // Layout grid items
    void layoutGridItems(LayoutNode* node, const LayoutConstraints& constraints);

    // Calculate grid container size
    Size calculateGridContainerSize(LayoutNode* node, const LayoutConstraints& constraints);

    // Calculate grid item size
    Size calculateGridItemSize(LayoutNode* item, const LayoutConstraints& constraints);

    // Calculate grid item position
    Point calculateGridItemPosition(LayoutNode* item);

    // Handle grid template columns
    void handleGridTemplateColumns(LayoutNode* node);

    // Handle grid template rows
    void handleGridTemplateRows(LayoutNode* node);

    // Handle grid template areas
    void handleGridTemplateAreas(LayoutNode* node);

    // Handle grid auto columns
    void handleGridAutoColumns(LayoutNode* node);

    // Handle grid auto rows
    void handleGridAutoRows(LayoutNode* node);

    // Handle grid auto flow
    void handleGridAutoFlow(LayoutNode* node);

    // Handle grid gap
    void handleGridGap(LayoutNode* node);

    // Handle grid column gap
    void handleGridColumnGap(LayoutNode* node);

    // Handle grid row gap
    void handleGridRowGap(LayoutNode* node);

    // Calculate grid tracks
    void calculateGridTracks(LayoutNode* node);

    // Calculate grid areas
    void calculateGridAreas(LayoutNode* node);

    // Place grid items
    void placeGridItems(LayoutNode* node);

    // Auto-place grid items
    void autoPlaceGridItems(LayoutNode* node);

    // Calculate grid item placement
    void calculateGridItemPlacement(LayoutNode* item);

    // Calculate grid item span
    void calculateGridItemSpan(LayoutNode* item);

    // Calculate grid item position
    void calculateGridItemPosition(LayoutNode* item);

    // Handle grid alignment
    void handleGridAlignment(LayoutNode* node);

    // Handle justify items
    void handleJustifyItems(LayoutNode* node);

    // Handle align items
    void handleAlignItems(LayoutNode* node);

    // Handle justify content
    void handleJustifyContent(LayoutNode* node);

    // Handle align content
    void handleAlignContent(LayoutNode* node);

    // Handle justify self
    void handleJustifySelf(LayoutNode* item);

    // Handle align self
    void handleAlignSelf(LayoutNode* item);

private:
    void layoutGridItem(LayoutNode* item, const LayoutConstraints& constraints);
    void positionGridItem(LayoutNode* item);
    void calculateGridTrackSizes(LayoutNode* node);
    void calculateGridTrackPositions(LayoutNode* node);
    void calculateGridItemSizes(LayoutNode* node);
    void calculateGridItemPositions(LayoutNode* node);
    void handleGridAutoFlowRow(LayoutNode* node);
    void handleGridAutoFlowColumn(LayoutNode* node);
    void handleGridAutoFlowDense(LayoutNode* node);
    void handleGridAutoFlowRowDense(LayoutNode* node);
    void handleGridAutoFlowColumnDense(LayoutNode* node);
    void handleJustifyItemsStart(LayoutNode* node);
    void handleJustifyItemsEnd(LayoutNode* node);
    void handleJustifyItemsCenter(LayoutNode* node);
    void handleJustifyItemsStretch(LayoutNode* node);
    void handleAlignItemsStart(LayoutNode* node);
    void handleAlignItemsEnd(LayoutNode* node);
    void handleAlignItemsCenter(LayoutNode* node);
    void handleAlignItemsStretch(LayoutNode* node);
    void handleJustifyContentStart(LayoutNode* node);
    void handleJustifyContentEnd(LayoutNode* node);
    void handleJustifyContentCenter(LayoutNode* node);
    void handleJustifyContentStretch(LayoutNode* node);
    void handleJustifyContentSpaceAround(LayoutNode* node);
    void handleJustifyContentSpaceBetween(LayoutNode* node);
    void handleJustifyContentSpaceEvenly(LayoutNode* node);
    void handleAlignContentStart(LayoutNode* node);
    void handleAlignContentEnd(LayoutNode* node);
    void handleAlignContentCenter(LayoutNode* node);
    void handleAlignContentStretch(LayoutNode* node);
    void handleAlignContentSpaceAround(LayoutNode* node);
    void handleAlignContentSpaceBetween(LayoutNode* node);
    void handleAlignContentSpaceEvenly(LayoutNode* node);
    void handleJustifySelfStart(LayoutNode* item);
    void handleJustifySelfEnd(LayoutNode* item);
    void handleJustifySelfCenter(LayoutNode* item);
    void handleJustifySelfStretch(LayoutNode* item);
    void handleAlignSelfStart(LayoutNode* item);
    void handleAlignSelfEnd(LayoutNode* item);
    void handleAlignSelfCenter(LayoutNode* item);
    void handleAlignSelfStretch(LayoutNode* item);
};

} // namespace layout
