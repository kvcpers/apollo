#pragma once

#include "types.h"
#include "enums.h"
#include <memory>
#include <vector>

namespace layout {

// CSS box model representation
class BoxModel {
public:
    BoxModel();
    ~BoxModel();

    // Content area
    const Rect& contentRect() const { return contentRect_; }
    void setContentRect(const Rect& rect) { contentRect_ = rect; }

    // Padding area
    const EdgeInsets& padding() const { return padding_; }
    void setPadding(const EdgeInsets& padding) { padding_ = padding; }
    Rect paddingRect() const;

    // Border area
    const EdgeInsets& border() const { return border_; }
    void setBorder(const EdgeInsets& border) { border_ = border; }
    Rect borderRect() const;

    // Margin area
    const EdgeInsets& margin() const { return margin_; }
    void setMargin(const EdgeInsets& margin) { margin_ = margin; }
    Rect marginRect() const;

    // Total area including margins
    Rect totalRect() const;

    // Box sizing
    BoxSizing boxSizing() const { return boxSizing_; }
    void setBoxSizing(BoxSizing boxSizing) { boxSizing_ = boxSizing; }

    // Calculate content size from total size
    Size calculateContentSize(const Size& totalSize) const;

    // Calculate total size from content size
    Size calculateTotalSize(const Size& contentSize) const;

    // Check if box is empty
    bool isEmpty() const;

    // Get the visual bounding box
    Rect visualBounds() const;

    // Get the layout bounding box (including margins)
    Rect layoutBounds() const;

    // Check if point is inside the box
    bool contains(const Point& point) const;

    // Check if rectangle intersects with the box
    bool intersects(const Rect& rect) const;

    // Get intersection with another box
    Rect intersection(const BoxModel& other) const;

    // Get union with another box
    Rect unionRect(const BoxModel& other) const;

    // Transform the box
    void transform(const Transform& transform);

    // Reset to default values
    void reset();

    // Copy constructor and assignment
    BoxModel(const BoxModel& other);
    BoxModel& operator=(const BoxModel& other);

    // Move constructor and assignment
    BoxModel(BoxModel&& other) noexcept;
    BoxModel& operator=(BoxModel&& other) noexcept;

private:
    Rect contentRect_;
    EdgeInsets padding_;
    EdgeInsets border_;
    EdgeInsets margin_;
    BoxSizing boxSizing_;
};

// Layout box that extends BoxModel with layout-specific properties
class LayoutBox : public BoxModel {
public:
    LayoutBox();
    ~LayoutBox();

    // Display type
    Display display() const { return display_; }
    void setDisplay(Display display) { display_ = display_; }

    // Position
    Position position() const { return position_; }
    void setPosition(Position position) { position_ = position_; }

    // Float
    Float float() const { return float_; }
    void setFloat(Float float) { float_ = float_; }

    // Clear
    Clear clear() const { return clear_; }
    void setClear(Clear clear) { clear_ = clear_; }

    // Z-index
    ZIndex zIndex() const { return zIndex_; }
    void setZIndex(ZIndex zIndex) { zIndex_ = zIndex; }

    // Transform
    const Transform& transform() const { return transform_; }
    void setTransform(const Transform& transform) { transform_ = transform; }

    // Opacity
    double opacity() const { return opacity_; }
    void setOpacity(double opacity) { opacity_ = std::max(0.0, std::min(1.0, opacity)); }

    // Visibility
    Visibility visibility() const { return visibility_; }
    void setVisibility(Visibility visibility) { visibility_ = visibility; }

    // Overflow
    Overflow overflow() const { return overflow_; }
    void setOverflow(Overflow overflow) { overflow_ = overflow; }

    // Clipping rectangle
    const Rect& clipRect() const { return clipRect_; }
    void setClipRect(const Rect& clipRect) { clipRect_ = clipRect; }

    // Is positioned
    bool isPositioned() const;

    // Is floating
    bool isFloating() const;

    // Is block level
    bool isBlockLevel() const;

    // Is inline level
    bool isInlineLevel() const;

    // Is replaced element
    bool isReplaced() const { return isReplaced_; }
    void setIsReplaced(bool isReplaced) { isReplaced_ = isReplaced; }

    // Is anonymous
    bool isAnonymous() const { return isAnonymous_; }
    void setIsAnonymous(bool isAnonymous) { isAnonymous_ = isAnonymous; }

    // Is root element
    bool isRoot() const { return isRoot_; }
    void setIsRoot(bool isRoot) { isRoot_ = isRoot; }

    // Is table cell
    bool isTableCell() const;

    // Is table row
    bool isTableRow() const;

    // Is table
    bool isTable() const;

    // Is flex container
    bool isFlexContainer() const;

    // Is flex item
    bool isFlexItem() const;

    // Is grid container
    bool isGridContainer() const;

    // Is grid item
    bool isGridItem() const;

    // Is stacking context
    bool isStackingContext() const;

    // Is containing block
    bool isContainingBlock() const;

    // Is formatting context root
    bool isFormattingContextRoot() const;

    // Reset to default values
    void reset();

    // Copy constructor and assignment
    LayoutBox(const LayoutBox& other);
    LayoutBox& operator=(const LayoutBox& other);

    // Move constructor and assignment
    LayoutBox(LayoutBox&& other) noexcept;
    LayoutBox& operator=(LayoutBox&& other) noexcept;

private:
    Display display_;
    Position position_;
    Float float_;
    Clear clear_;
    ZIndex zIndex_;
    Transform transform_;
    double opacity_;
    Visibility visibility_;
    Overflow overflow_;
    Rect clipRect_;
    bool isReplaced_;
    bool isAnonymous_;
    bool isRoot_;
};

// Box tree node for hierarchical layout
class BoxNode {
public:
    BoxNode();
    explicit BoxNode(std::shared_ptr<LayoutBox> box);
    ~BoxNode();

    // Box data
    const LayoutBox* box() const { return box_.get(); }
    LayoutBox* box() { return box_.get(); }
    void setBox(std::shared_ptr<LayoutBox> box) { box_ = box; }

    // Parent
    BoxNode* parent() const { return parent_; }
    void setParent(BoxNode* parent) { parent_ = parent; }

    // Children
    const std::vector<BoxNode*>& children() const { return children_; }
    std::vector<BoxNode*>& children() { return children_; }

    // Add child
    void addChild(BoxNode* child);
    void insertChild(BoxNode* child, size_t index);

    // Remove child
    void removeChild(BoxNode* child);
    void removeChildAt(size_t index);

    // Remove all children
    void clearChildren();

    // Get child count
    size_t childCount() const { return children_.size(); }

    // Get child at index
    BoxNode* childAt(size_t index) const;

    // Get first child
    BoxNode* firstChild() const;

    // Get last child
    BoxNode* lastChild() const;

    // Get next sibling
    BoxNode* nextSibling() const;

    // Get previous sibling
    BoxNode* previousSibling() const;

    // Get index in parent
    size_t indexInParent() const;

    // Find child by index
    BoxNode* findChild(size_t index) const;

    // Find child by box
    BoxNode* findChild(const LayoutBox* box) const;

    // Get depth in tree
    size_t depth() const;

    // Get height of subtree
    size_t height() const;

    // Is root
    bool isRoot() const { return parent_ == nullptr; }

    // Is leaf
    bool isLeaf() const { return children_.empty(); }

    // Get all descendants
    std::vector<BoxNode*> getAllDescendants() const;

    // Get all ancestors
    std::vector<BoxNode*> getAllAncestors() const;

    // Get siblings
    std::vector<BoxNode*> getSiblings() const;

    // Get previous siblings
    std::vector<BoxNode*> getPreviousSiblings() const;

    // Get next siblings
    std::vector<BoxNode*> getNextSiblings() const;

    // Find common ancestor
    BoxNode* findCommonAncestor(BoxNode* other) const;

    // Get path to root
    std::vector<BoxNode*> getPathToRoot() const;

    // Get path to ancestor
    std::vector<BoxNode*> getPathToAncestor(BoxNode* ancestor) const;

    // Check if is ancestor of
    bool isAncestorOf(BoxNode* node) const;

    // Check if is descendant of
    bool isDescendantOf(BoxNode* node) const;

    // Check if is sibling of
    bool isSiblingOf(BoxNode* node) const;

    // Clone the node and its subtree
    std::unique_ptr<BoxNode> clone() const;

    // Clone the node without children
    std::unique_ptr<BoxNode> cloneShallow() const;

private:
    std::shared_ptr<LayoutBox> box_;
    BoxNode* parent_;
    std::vector<BoxNode*> children_;

    void updateParentChildRelationships();
};

// Box tree for managing the layout hierarchy
class BoxTree {
public:
    BoxTree();
    ~BoxTree();

    // Root node
    BoxNode* root() const { return root_.get(); }
    void setRoot(std::unique_ptr<BoxNode> root);

    // Create new node
    std::unique_ptr<BoxNode> createNode();
    std::unique_ptr<BoxNode> createNode(std::shared_ptr<LayoutBox> box);

    // Add node as child
    void addChild(BoxNode* parent, BoxNode* child);

    // Remove node
    void removeNode(BoxNode* node);

    // Move node
    void moveNode(BoxNode* node, BoxNode* newParent);

    // Clear all nodes
    void clear();

    // Get all nodes
    std::vector<BoxNode*> getAllNodes() const;

    // Get all leaf nodes
    std::vector<BoxNode*> getLeafNodes() const;

    // Get all nodes at depth
    std::vector<BoxNode*> getNodesAtDepth(size_t depth) const;

    // Get node count
    size_t nodeCount() const;

    // Get height of tree
    size_t height() const;

    // Find node by box
    BoxNode* findNode(const LayoutBox* box) const;

    // Find nodes by display type
    std::vector<BoxNode*> findNodesByDisplay(Display display) const;

    // Find nodes by position
    std::vector<BoxNode*> findNodesByPosition(Position position) const;

    // Find positioned nodes
    std::vector<BoxNode*> findPositionedNodes() const;

    // Find floating nodes
    std::vector<BoxNode*> findFloatingNodes() const;

    // Find block level nodes
    std::vector<BoxNode*> findBlockLevelNodes() const;

    // Find inline level nodes
    std::vector<BoxNode*> findInlineLevelNodes() const;

    // Find stacking context nodes
    std::vector<BoxNode*> findStackingContextNodes() const;

    // Validate tree structure
    bool isValid() const;

    // Clone the entire tree
    std::unique_ptr<BoxTree> clone() const;

private:
    std::unique_ptr<BoxNode> root_;

    void collectAllNodes(BoxNode* node, std::vector<BoxNode*>& nodes) const;
    void collectLeafNodes(BoxNode* node, std::vector<BoxNode*>& nodes) const;
    void collectNodesAtDepth(BoxNode* node, size_t depth, std::vector<BoxNode*>& nodes) const;
    void collectNodesByDisplay(BoxNode* node, Display display, std::vector<BoxNode*>& nodes) const;
    void collectNodesByPosition(BoxNode* node, Position position, std::vector<BoxNode*>& nodes) const;
    void collectPositionedNodes(BoxNode* node, std::vector<BoxNode*>& nodes) const;
    void collectFloatingNodes(BoxNode* node, std::vector<BoxNode*>& nodes) const;
    void collectBlockLevelNodes(BoxNode* node, std::vector<BoxNode*>& nodes) const;
    void collectInlineLevelNodes(BoxNode* node, std::vector<BoxNode*>& nodes) const;
    void collectStackingContextNodes(BoxNode* node, std::vector<BoxNode*>& nodes) const;
    bool validateNode(BoxNode* node) const;
    BoxNode* findNodeRecursive(BoxNode* node, const LayoutBox* box) const;
};

} // namespace layout
