#pragma once

#include "box_model.h"
#include "types.h"
#include <memory>
#include <vector>
#include <string>

namespace layout {

// Forward declarations
class LayoutEngine;
class LayoutContext;

// Layout node that represents an element in the layout tree
class LayoutNode {
public:
    LayoutNode();
    explicit LayoutNode(std::shared_ptr<LayoutBox> box);
    ~LayoutNode();

    // Box data
    const LayoutBox* box() const { return box_.get(); }
    LayoutBox* box() { return box_.get(); }
    void setBox(std::shared_ptr<LayoutBox> box) { box_ = box; }

    // Layout properties
    const Rect& layoutRect() const { return layoutRect_; }
    void setLayoutRect(const Rect& rect) { layoutRect_ = rect; }

    const Size& intrinsicSize() const { return intrinsicSize_; }
    void setIntrinsicSize(const Size& size) { intrinsicSize_ = size; }

    const Size& minSize() const { return minSize_; }
    void setMinSize(const Size& size) { minSize_ = size; }

    const Size& maxSize() const { return maxSize_; }
    void setMaxSize(const Size& size) { maxSize_ = size; }

    // Layout state
    bool isLayoutDirty() const { return isLayoutDirty_; }
    void setLayoutDirty(bool dirty) { isLayoutDirty_ = dirty; }

    bool needsLayout() const { return needsLayout_; }
    void setNeedsLayout(bool needs) { needsLayout_ = needs; }

    bool isPositioned() const;
    bool isFloating() const;
    bool isBlockLevel() const;
    bool isInlineLevel() const;

    // Text content
    const std::string& textContent() const { return textContent_; }
    void setTextContent(const std::string& text) { textContent_ = text; }

    // Font metrics
    const FontMetrics& fontMetrics() const { return fontMetrics_; }
    void setFontMetrics(const FontMetrics& metrics) { fontMetrics_ = metrics; }

    // Line height
    double lineHeight() const { return lineHeight_; }
    void setLineHeight(double height) { lineHeight_ = height; }

    // Baseline
    double baseline() const { return baseline_; }
    void setBaseline(double baseline) { baseline_ = baseline; }

    // Parent-child relationships
    LayoutNode* parent() const { return parent_; }
    void setParent(LayoutNode* parent) { parent_ = parent; }

    const std::vector<LayoutNode*>& children() const { return children_; }
    std::vector<LayoutNode*>& children() { return children_; }

    // Add child
    void addChild(LayoutNode* child);
    void insertChild(LayoutNode* child, size_t index);

    // Remove child
    void removeChild(LayoutNode* child);
    void removeChildAt(size_t index);

    // Remove all children
    void clearChildren();

    // Get child count
    size_t childCount() const { return children_.size(); }

    // Get child at index
    LayoutNode* childAt(size_t index) const;

    // Get first child
    LayoutNode* firstChild() const;

    // Get last child
    LayoutNode* lastChild() const;

    // Get next sibling
    LayoutNode* nextSibling() const;

    // Get previous sibling
    LayoutNode* previousSibling() const;

    // Get index in parent
    size_t indexInParent() const;

    // Find child by index
    LayoutNode* findChild(size_t index) const;

    // Get depth in tree
    size_t depth() const;

    // Get height of subtree
    size_t height() const;

    // Is root
    bool isRoot() const { return parent_ == nullptr; }

    // Is leaf
    bool isLeaf() const { return children_.empty(); }

    // Get all descendants
    std::vector<LayoutNode*> getAllDescendants() const;

    // Get all ancestors
    std::vector<LayoutNode*> getAllAncestors() const;

    // Get siblings
    std::vector<LayoutNode*> getSiblings() const;

    // Get previous siblings
    std::vector<LayoutNode*> getPreviousSiblings() const;

    // Get next siblings
    std::vector<LayoutNode*> getNextSiblings() const;

    // Find common ancestor
    LayoutNode* findCommonAncestor(LayoutNode* other) const;

    // Get path to root
    std::vector<LayoutNode*> getPathToRoot() const;

    // Get path to ancestor
    std::vector<LayoutNode*> getPathToAncestor(LayoutNode* ancestor) const;

    // Check if is ancestor of
    bool isAncestorOf(LayoutNode* node) const;

    // Check if is descendant of
    bool isDescendantOf(LayoutNode* node) const;

    // Check if is sibling of
    bool isSiblingOf(LayoutNode* node) const;

    // Layout methods
    void layout(const LayoutConstraints& constraints);
    void layoutChildren(const LayoutConstraints& constraints);
    void layoutPositionedChildren();
    void layoutFloatingChildren();

    // Size calculation
    Size calculateEmptyIntrinsicSize() const;
    Size calculateIntrinsicSize() const;
    Size calculateMinSize() const;
    Size calculateMaxSize() const;

    // Position calculation
    Point calculatePosition() const;
    void updatePosition(const Point& position);

    // Margin collapse
    double getCollapsedMarginTop() const;
    double getCollapsedMarginBottom() const;
    void collapseMarginsWithParent();
    void collapseMarginsWithChildren();

    // Float handling
    void addFloat(LayoutNode* floatNode);
    void removeFloat(LayoutNode* floatNode);
    void clearFloats();
    const std::vector<LayoutNode*>& floats() const { return floats_; }

    // Clear handling
    void clearFloats(Clear clear);

    // Stacking context
    void createStackingContext();
    void destroyStackingContext();
    bool isStackingContext() const;

    // Containing block
    LayoutNode* getContainingBlock() const;
    bool isContainingBlock() const;

    // Formatting context
    LayoutNode* getFormattingContextRoot() const;
    bool isFormattingContextRoot() const;

    // Text layout
    void layoutText();
    void layoutInlineText();
    void layoutBlockText();

    // Line breaking
    void breakLines();
    void breakInlineLines();
    void breakBlockLines();

    // Word wrapping
    void wrapWords();
    void wrapInlineWords();
    void wrapBlockWords();

    // Text measurement
    Size measureText(const std::string& text) const;
    double measureTextWidth(const std::string& text) const;
    double measureTextHeight(const std::string& text) const;

    // Hit testing
    LayoutNode* hitTest(const Point& point) const;
    bool containsPoint(const Point& point) const;

    // Bounds calculation
    Rect getBounds() const;
    Rect getContentBounds() const;
    Rect getPaddingBounds() const;
    Rect getBorderBounds() const;
    Rect getMarginBounds() const;

    // Transform
    void applyTransform(const Transform& transform);
    Transform getCumulativeTransform() const;

    // Visibility
    bool isVisible() const;
    bool isHidden() const;
    void setVisible(bool visible);

    // Clipping
    void setClipRect(const Rect& clipRect);
    Rect getClipRect() const;
    bool isClipped() const;

    // Overflow
    void handleOverflow();
    void clipContent();
    void scrollContent(const Point& offset);

    // Invalidation
    void invalidateLayout();
    void invalidateChildren();
    void invalidateParent();
    void invalidateAll();

    // Update
    void updateLayout();
    void updateChildren();
    void updateParent();

    // Clone
    std::unique_ptr<LayoutNode> clone() const;
    std::unique_ptr<LayoutNode> cloneShallow() const;

    // Reset
    void reset();

    // Copy constructor and assignment
    LayoutNode(const LayoutNode& other);
    LayoutNode& operator=(const LayoutNode& other);

    // Move constructor and assignment
    LayoutNode(LayoutNode&& other) noexcept;
    LayoutNode& operator=(LayoutNode&& other) noexcept;

private:
    std::shared_ptr<LayoutBox> box_;
    Rect layoutRect_;
    Size intrinsicSize_;
    Size minSize_;
    Size maxSize_;
    
    bool isLayoutDirty_;
    bool needsLayout_;
    
    std::string textContent_;
    FontMetrics fontMetrics_;
    double lineHeight_;
    double baseline_;
    
    LayoutNode* parent_;
    std::vector<LayoutNode*> children_;
    std::vector<LayoutNode*> floats_;
    
    void updateParentChildRelationships();
    void collectDescendants(LayoutNode* node, std::vector<LayoutNode*>& descendants) const;
};

// Layout tree for managing the layout hierarchy
class LayoutTree {
public:
    LayoutTree();
    ~LayoutTree();

    // Root node
    LayoutNode* root() const { return root_.get(); }
    void setRoot(std::unique_ptr<LayoutNode> root);

    // Create new node
    std::unique_ptr<LayoutNode> createNode();
    std::unique_ptr<LayoutNode> createNode(std::shared_ptr<LayoutBox> box);

    // Add node as child
    void addChild(LayoutNode* parent, LayoutNode* child);

    // Remove node
    void removeNode(LayoutNode* node);

    // Move node
    void moveNode(LayoutNode* node, LayoutNode* newParent);

    // Clear all nodes
    void clear();

    // Get all nodes
    std::vector<LayoutNode*> getAllNodes() const;

    // Get all leaf nodes
    std::vector<LayoutNode*> getLeafNodes() const;

    // Get all nodes at depth
    std::vector<LayoutNode*> getNodesAtDepth(size_t depth) const;

    // Get node count
    size_t nodeCount() const;

    // Get height of tree
    size_t height() const;

    // Find node by box
    LayoutNode* findNode(const LayoutBox* box) const;

    // Find nodes by display type
    std::vector<LayoutNode*> findNodesByDisplay(Display display) const;

    // Find nodes by position
    std::vector<LayoutNode*> findNodesByPosition(Position position) const;

    // Find positioned nodes
    std::vector<LayoutNode*> findPositionedNodes() const;

    // Find floating nodes
    std::vector<LayoutNode*> findFloatingNodes() const;

    // Find block level nodes
    std::vector<LayoutNode*> findBlockLevelNodes() const;

    // Find inline level nodes
    std::vector<LayoutNode*> findInlineLevelNodes() const;

    // Find stacking context nodes
    std::vector<LayoutNode*> findStackingContextNodes() const;

    // Layout the entire tree
    void layout(const LayoutConstraints& constraints);

    // Update layout for dirty nodes
    void updateLayout();

    // Invalidate layout for all nodes
    void invalidateLayout();

    // Validate tree structure
    bool isValid() const;

    // Clone the entire tree
    std::unique_ptr<LayoutTree> clone() const;

private:
    std::unique_ptr<LayoutNode> root_;

    void collectAllNodes(LayoutNode* node, std::vector<LayoutNode*>& nodes) const;
    void collectLeafNodes(LayoutNode* node, std::vector<LayoutNode*>& nodes) const;
    void collectNodesAtDepth(LayoutNode* node, size_t depth, std::vector<LayoutNode*>& nodes) const;
    void collectNodesByDisplay(LayoutNode* node, Display display, std::vector<LayoutNode*>& nodes) const;
    void collectNodesByPosition(LayoutNode* node, Position position, std::vector<LayoutNode*>& nodes) const;
    void collectPositionedNodes(LayoutNode* node, std::vector<LayoutNode*>& nodes) const;
    void collectFloatingNodes(LayoutNode* node, std::vector<LayoutNode*>& nodes) const;
    void collectBlockLevelNodes(LayoutNode* node, std::vector<LayoutNode*>& nodes) const;
    void collectInlineLevelNodes(LayoutNode* node, std::vector<LayoutNode*>& nodes) const;
    void collectStackingContextNodes(LayoutNode* node, std::vector<LayoutNode*>& nodes) const;
    bool validateNode(LayoutNode* node) const;
    LayoutNode* findNodeRecursive(LayoutNode* node, const LayoutBox* box) const;
};

} // namespace layout
