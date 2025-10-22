#include "layout/layout_node.h"
#include <algorithm>
#include <limits>

namespace layout {

// LayoutNode implementation
LayoutNode::LayoutNode()
    : box_(nullptr)
    , layoutRect_(0, 0, 0, 0)
    , intrinsicSize_(0, 0)
    , minSize_(0, 0)
    , maxSize_(std::numeric_limits<double>::max(), std::numeric_limits<double>::max())
    , isLayoutDirty_(true)
    , needsLayout_(true)
    , lineHeight_(0)
    , baseline_(0)
    , parent_(nullptr) {
}

LayoutNode::LayoutNode(std::shared_ptr<LayoutBox> box)
    : box_(box)
    , layoutRect_(0, 0, 0, 0)
    , intrinsicSize_(0, 0)
    , minSize_(0, 0)
    , maxSize_(std::numeric_limits<double>::max(), std::numeric_limits<double>::max())
    , isLayoutDirty_(true)
    , needsLayout_(true)
    , lineHeight_(0)
    , baseline_(0)
    , parent_(nullptr) {
}

LayoutNode::~LayoutNode() {
    clearChildren();
}

bool LayoutNode::isPositioned() const {
    return box_ && box_->isPositioned();
}

bool LayoutNode::isFloating() const {
    return box_ && box_->isFloating();
}

bool LayoutNode::isBlockLevel() const {
    return box_ && box_->isBlockLevel();
}

bool LayoutNode::isInlineLevel() const {
    return box_ && box_->isInlineLevel();
}

void LayoutNode::addChild(LayoutNode* child) {
    if (child && child != this) {
        child->setParent(this);
        children_.push_back(child);
    }
}

void LayoutNode::insertChild(LayoutNode* child, size_t index) {
    if (child && child != this && index <= children_.size()) {
        child->setParent(this);
        children_.insert(children_.begin() + index, child);
    }
}

void LayoutNode::removeChild(LayoutNode* child) {
    auto it = std::find(children_.begin(), children_.end(), child);
    if (it != children_.end()) {
        (*it)->setParent(nullptr);
        children_.erase(it);
    }
}

void LayoutNode::removeChildAt(size_t index) {
    if (index < children_.size()) {
        children_[index]->setParent(nullptr);
        children_.erase(children_.begin() + index);
    }
}

void LayoutNode::clearChildren() {
    for (auto* child : children_) {
        child->setParent(nullptr);
    }
    children_.clear();
}

LayoutNode* LayoutNode::childAt(size_t index) const {
    return (index < children_.size()) ? children_[index] : nullptr;
}

LayoutNode* LayoutNode::firstChild() const {
    return children_.empty() ? nullptr : children_.front();
}

LayoutNode* LayoutNode::lastChild() const {
    return children_.empty() ? nullptr : children_.back();
}

LayoutNode* LayoutNode::nextSibling() const {
    if (!parent_) return nullptr;
    
    auto it = std::find(parent_->children_.begin(), parent_->children_.end(), this);
    if (it != parent_->children_.end() && ++it != parent_->children_.end()) {
        return *it;
    }
    return nullptr;
}

LayoutNode* LayoutNode::previousSibling() const {
    if (!parent_) return nullptr;
    
    auto it = std::find(parent_->children_.begin(), parent_->children_.end(), this);
    if (it != parent_->children_.begin()) {
        return *(--it);
    }
    return nullptr;
}

size_t LayoutNode::indexInParent() const {
    if (!parent_) return 0;
    
    auto it = std::find(parent_->children_.begin(), parent_->children_.end(), this);
    return (it != parent_->children_.end()) ? std::distance(parent_->children_.begin(), it) : 0;
}

LayoutNode* LayoutNode::findChild(size_t index) const {
    return childAt(index);
}

size_t LayoutNode::depth() const {
    size_t depth = 0;
    LayoutNode* current = parent_;
    while (current) {
        ++depth;
        current = current->parent_;
    }
    return depth;
}

size_t LayoutNode::height() const {
    if (children_.empty()) return 0;
    
    size_t maxChildHeight = 0;
    for (auto* child : children_) {
        maxChildHeight = std::max(maxChildHeight, child->height());
    }
    return maxChildHeight + 1;
}

std::vector<LayoutNode*> LayoutNode::getAllDescendants() const {
    std::vector<LayoutNode*> descendants;
    collectDescendants(const_cast<LayoutNode*>(this), descendants);
    return descendants;
}

std::vector<LayoutNode*> LayoutNode::getAllAncestors() const {
    std::vector<LayoutNode*> ancestors;
    LayoutNode* current = parent_;
    while (current) {
        ancestors.push_back(current);
        current = current->parent_;
    }
    return ancestors;
}

std::vector<LayoutNode*> LayoutNode::getSiblings() const {
    if (!parent_) return {};
    
    std::vector<LayoutNode*> siblings;
    for (auto* child : parent_->children_) {
        if (child != this) {
            siblings.push_back(child);
        }
    }
    return siblings;
}

std::vector<LayoutNode*> LayoutNode::getPreviousSiblings() const {
    if (!parent_) return {};
    
    std::vector<LayoutNode*> siblings;
    size_t index = indexInParent();
    for (size_t i = 0; i < index; ++i) {
        siblings.push_back(parent_->children_[i]);
    }
    return siblings;
}

std::vector<LayoutNode*> LayoutNode::getNextSiblings() const {
    if (!parent_) return {};
    
    std::vector<LayoutNode*> siblings;
    size_t index = indexInParent();
    for (size_t i = index + 1; i < parent_->children_.size(); ++i) {
        siblings.push_back(parent_->children_[i]);
    }
    return siblings;
}

LayoutNode* LayoutNode::findCommonAncestor(LayoutNode* other) const {
    if (!other) return nullptr;
    
    std::vector<LayoutNode*> thisAncestors = getAllAncestors();
    std::vector<LayoutNode*> otherAncestors = other->getAllAncestors();
    
    for (auto* ancestor : thisAncestors) {
        if (std::find(otherAncestors.begin(), otherAncestors.end(), ancestor) != otherAncestors.end()) {
            return ancestor;
        }
    }
    return nullptr;
}

std::vector<LayoutNode*> LayoutNode::getPathToRoot() const {
    std::vector<LayoutNode*> path;
    LayoutNode* current = const_cast<LayoutNode*>(this);
    while (current) {
        path.push_back(current);
        current = current->parent_;
    }
    return path;
}

std::vector<LayoutNode*> LayoutNode::getPathToAncestor(LayoutNode* ancestor) const {
    std::vector<LayoutNode*> path;
    LayoutNode* current = const_cast<LayoutNode*>(this);
    while (current && current != ancestor) {
        path.push_back(current);
        current = current->parent_;
    }
    if (current == ancestor) {
        path.push_back(current);
    }
    return path;
}

bool LayoutNode::isAncestorOf(LayoutNode* node) const {
    if (!node) return false;
    
    LayoutNode* current = node->parent_;
    while (current) {
        if (current == this) return true;
        current = current->parent_;
    }
    return false;
}

bool LayoutNode::isDescendantOf(LayoutNode* node) const {
    return node ? node->isAncestorOf(this) : false;
}

bool LayoutNode::isSiblingOf(LayoutNode* node) const {
    return node && parent_ && parent_ == node->parent_;
}

void LayoutNode::layout(const LayoutConstraints& constraints) {
    if (!needsLayout_) return;
    
    // Calculate intrinsic size
    intrinsicSize_ = calculateIntrinsicSize();
    
    // Calculate min/max sizes
    minSize_ = calculateMinSize();
    maxSize_ = calculateMaxSize();
    
    // Constrain size
    Size constrainedSize = constraints.constrain(intrinsicSize_);
    
    // Set layout rect
    layoutRect_ = Rect(0, 0, constrainedSize.width, constrainedSize.height);
    
    // Layout children
    layoutChildren(constraints);
    
    // Mark as not needing layout
    needsLayout_ = false;
    isLayoutDirty_ = false;
}

void LayoutNode::layoutChildren(const LayoutConstraints& constraints) {
    for (auto* child : children_) {
        if (child) {
            child->layout(constraints);
        }
    }
}

void LayoutNode::layoutPositionedChildren() {
    for (auto* child : children_) {
        if (child && child->isPositioned()) {
            // Layout positioned child
            // This would be implemented by the positioned layout algorithm
        }
    }
}

void LayoutNode::layoutFloatingChildren() {
    for (auto* child : children_) {
        if (child && child->isFloating()) {
            // Layout floating child
            // This would be implemented by the block layout algorithm
        }
    }
}

Size LayoutNode::calculateEmptyIntrinsicSize() const {
    return Size(0, 0);
}

Size LayoutNode::calculateIntrinsicSize() const {
    if (!box_) return Size(0, 0);
    
    // This is a simplified implementation
    // In a real implementation, this would consider the content and box model
    return Size(100, 100); // Default size
}

Size LayoutNode::calculateMinSize() const {
    if (!box_) return Size(0, 0);
    
    // This is a simplified implementation
    return Size(0, 0);
}

Size LayoutNode::calculateMaxSize() const {
    if (!box_) return Size(std::numeric_limits<double>::max(), std::numeric_limits<double>::max());
    
    // This is a simplified implementation
    return Size(std::numeric_limits<double>::max(), std::numeric_limits<double>::max());
}

Point LayoutNode::calculatePosition() const {
    return layoutRect_.origin();
}

void LayoutNode::updatePosition(const Point& position) {
    layoutRect_ = Rect(position, layoutRect_.size());
}

double LayoutNode::getCollapsedMarginTop() const {
    // Simplified margin collapse implementation
    return 0;
}

double LayoutNode::getCollapsedMarginBottom() const {
    // Simplified margin collapse implementation
    return 0;
}

void LayoutNode::collapseMarginsWithParent() {
    // Simplified margin collapse implementation
}

void LayoutNode::collapseMarginsWithChildren() {
    // Simplified margin collapse implementation
}

void LayoutNode::addFloat(LayoutNode* floatNode) {
    if (floatNode && floatNode->isFloating()) {
        floats_.push_back(floatNode);
    }
}

void LayoutNode::removeFloat(LayoutNode* floatNode) {
    auto it = std::find(floats_.begin(), floats_.end(), floatNode);
    if (it != floats_.end()) {
        floats_.erase(it);
    }
}

void LayoutNode::clearFloats() {
    floats_.clear();
}

void LayoutNode::clearFloats(Clear clear) {
    // Simplified clear implementation
    clearFloats();
}

void LayoutNode::createStackingContext() {
    // Simplified stacking context creation
}

void LayoutNode::destroyStackingContext() {
    // Simplified stacking context destruction
}

bool LayoutNode::isStackingContext() const {
    return box_ && box_->isStackingContext();
}

LayoutNode* LayoutNode::getContainingBlock() const {
    if (!parent_) return nullptr;
    
    // Simplified containing block calculation
    return parent_;
}

bool LayoutNode::isContainingBlock() const {
    return box_ && box_->isContainingBlock();
}

LayoutNode* LayoutNode::getFormattingContextRoot() const {
    if (!parent_) return this;
    
    // Simplified formatting context root calculation
    return parent_->getFormattingContextRoot();
}

bool LayoutNode::isFormattingContextRoot() const {
    return box_ && box_->isFormattingContextRoot();
}

void LayoutNode::layoutText() {
    // Simplified text layout
}

void LayoutNode::layoutInlineText() {
    // Simplified inline text layout
}

void LayoutNode::layoutBlockText() {
    // Simplified block text layout
}

void LayoutNode::breakLines() {
    // Simplified line breaking
}

void LayoutNode::breakInlineLines() {
    // Simplified inline line breaking
}

void LayoutNode::breakBlockLines() {
    // Simplified block line breaking
}

void LayoutNode::wrapWords() {
    // Simplified word wrapping
}

void LayoutNode::wrapInlineWords() {
    // Simplified inline word wrapping
}

void LayoutNode::wrapBlockWords() {
    // Simplified block word wrapping
}

Size LayoutNode::measureText(const std::string& text) const {
    // Simplified text measurement
    return Size(text.length() * 10, 20); // Approximate size
}

double LayoutNode::measureTextWidth(const std::string& text) const {
    return measureText(text).width;
}

double LayoutNode::measureTextHeight(const std::string& text) const {
    return measureText(text).height;
}

LayoutNode* LayoutNode::hitTest(const Point& point) const {
    if (!containsPoint(point)) return nullptr;
    
    // Test children in reverse order (top to bottom)
    for (auto it = children_.rbegin(); it != children_.rend(); ++it) {
        LayoutNode* result = (*it)->hitTest(point);
        if (result) return result;
    }
    
    return const_cast<LayoutNode*>(this);
}

bool LayoutNode::containsPoint(const Point& point) const {
    return layoutRect_.contains(point);
}

Rect LayoutNode::getBounds() const {
    return layoutRect_;
}

Rect LayoutNode::getContentBounds() const {
    if (!box_) return layoutRect_;
    return box_->contentRect();
}

Rect LayoutNode::getPaddingBounds() const {
    if (!box_) return layoutRect_;
    return box_->paddingRect();
}

Rect LayoutNode::getBorderBounds() const {
    if (!box_) return layoutRect_;
    return box_->borderRect();
}

Rect LayoutNode::getMarginBounds() const {
    if (!box_) return layoutRect_;
    return box_->marginRect();
}

void LayoutNode::applyTransform(const Transform& transform) {
    if (box_) {
        box_->transform(transform);
    }
    layoutRect_ = transform.transform(layoutRect_);
}

Transform LayoutNode::getCumulativeTransform() const {
    if (!box_) return Transform::identity();
    return box_->transform();
}

bool LayoutNode::isVisible() const {
    return !box_ || box_->visibility() == Visibility::Visible;
}

bool LayoutNode::isHidden() const {
    return box_ && box_->visibility() == Visibility::Hidden;
}

void LayoutNode::setVisible(bool visible) {
    if (box_) {
        box_->setVisibility(visible ? Visibility::Visible : Visibility::Hidden);
    }
}

void LayoutNode::setClipRect(const Rect& clipRect) {
    if (box_) {
        box_->setClipRect(clipRect);
    }
}

Rect LayoutNode::getClipRect() const {
    return box_ ? box_->clipRect() : Rect();
}

bool LayoutNode::isClipped() const {
    return box_ && !box_->clipRect().isEmpty();
}

void LayoutNode::handleOverflow() {
    // Simplified overflow handling
}

void LayoutNode::clipContent() {
    // Simplified content clipping
}

void LayoutNode::scrollContent(const Point& offset) {
    // Simplified content scrolling
}

void LayoutNode::invalidateLayout() {
    needsLayout_ = true;
    isLayoutDirty_ = true;
}

void LayoutNode::invalidateChildren() {
    for (auto* child : children_) {
        if (child) {
            child->invalidateLayout();
        }
    }
}

void LayoutNode::invalidateParent() {
    if (parent_) {
        parent_->invalidateLayout();
    }
}

void LayoutNode::invalidateAll() {
    invalidateLayout();
    invalidateChildren();
}

void LayoutNode::updateLayout() {
    if (needsLayout_) {
        layout(LayoutConstraints());
    }
}

void LayoutNode::updateChildren() {
    for (auto* child : children_) {
        if (child) {
            child->updateLayout();
        }
    }
}

void LayoutNode::updateParent() {
    if (parent_) {
        parent_->updateLayout();
    }
}

std::unique_ptr<LayoutNode> LayoutNode::clone() const {
    auto cloned = std::make_unique<LayoutNode>();
    if (box_) {
        cloned->box_ = std::make_shared<LayoutBox>(*box_);
    }
    cloned->layoutRect_ = layoutRect_;
    cloned->intrinsicSize_ = intrinsicSize_;
    cloned->minSize_ = minSize_;
    cloned->maxSize_ = maxSize_;
    cloned->isLayoutDirty_ = isLayoutDirty_;
    cloned->needsLayout_ = needsLayout_;
    cloned->textContent_ = textContent_;
    cloned->fontMetrics_ = fontMetrics_;
    cloned->lineHeight_ = lineHeight_;
    cloned->baseline_ = baseline_;
    
    for (auto* child : children_) {
        auto clonedChild = child->clone();
        cloned->addChild(clonedChild.release());
    }
    
    return cloned;
}

std::unique_ptr<LayoutNode> LayoutNode::cloneShallow() const {
    auto cloned = std::make_unique<LayoutNode>();
    if (box_) {
        cloned->box_ = std::make_shared<LayoutBox>(*box_);
    }
    cloned->layoutRect_ = layoutRect_;
    cloned->intrinsicSize_ = intrinsicSize_;
    cloned->minSize_ = minSize_;
    cloned->maxSize_ = maxSize_;
    cloned->isLayoutDirty_ = isLayoutDirty_;
    cloned->needsLayout_ = needsLayout_;
    cloned->textContent_ = textContent_;
    cloned->fontMetrics_ = fontMetrics_;
    cloned->lineHeight_ = lineHeight_;
    cloned->baseline_ = baseline_;
    return cloned;
}

void LayoutNode::reset() {
    box_.reset();
    layoutRect_ = Rect(0, 0, 0, 0);
    intrinsicSize_ = Size(0, 0);
    minSize_ = Size(0, 0);
    maxSize_ = Size(std::numeric_limits<double>::max(), std::numeric_limits<double>::max());
    isLayoutDirty_ = true;
    needsLayout_ = true;
    textContent_.clear();
    fontMetrics_ = FontMetrics();
    lineHeight_ = 0;
    baseline_ = 0;
    parent_ = nullptr;
    clearChildren();
    clearFloats();
}

LayoutNode::LayoutNode(const LayoutNode& other)
    : box_(other.box_ ? std::make_shared<LayoutBox>(*other.box_) : nullptr)
    , layoutRect_(other.layoutRect_)
    , intrinsicSize_(other.intrinsicSize_)
    , minSize_(other.minSize_)
    , maxSize_(other.maxSize_)
    , isLayoutDirty_(other.isLayoutDirty_)
    , needsLayout_(other.needsLayout_)
    , textContent_(other.textContent_)
    , fontMetrics_(other.fontMetrics_)
    , lineHeight_(other.lineHeight_)
    , baseline_(other.baseline_)
    , parent_(nullptr) {
}

LayoutNode& LayoutNode::operator=(const LayoutNode& other) {
    if (this != &other) {
        box_ = other.box_ ? std::make_shared<LayoutBox>(*other.box_) : nullptr;
        layoutRect_ = other.layoutRect_;
        intrinsicSize_ = other.intrinsicSize_;
        minSize_ = other.minSize_;
        maxSize_ = other.maxSize_;
        isLayoutDirty_ = other.isLayoutDirty_;
        needsLayout_ = other.needsLayout_;
        textContent_ = other.textContent_;
        fontMetrics_ = other.fontMetrics_;
        lineHeight_ = other.lineHeight_;
        baseline_ = other.baseline_;
        parent_ = nullptr;
        clearChildren();
        clearFloats();
    }
    return *this;
}

LayoutNode::LayoutNode(LayoutNode&& other) noexcept
    : box_(std::move(other.box_))
    , layoutRect_(std::move(other.layoutRect_))
    , intrinsicSize_(std::move(other.intrinsicSize_))
    , minSize_(std::move(other.minSize_))
    , maxSize_(std::move(other.maxSize_))
    , isLayoutDirty_(other.isLayoutDirty_)
    , needsLayout_(other.needsLayout_)
    , textContent_(std::move(other.textContent_))
    , fontMetrics_(std::move(other.fontMetrics_))
    , lineHeight_(other.lineHeight_)
    , baseline_(other.baseline_)
    , parent_(other.parent_)
    , children_(std::move(other.children_))
    , floats_(std::move(other.floats_)) {
}

LayoutNode& LayoutNode::operator=(LayoutNode&& other) noexcept {
    if (this != &other) {
        box_ = std::move(other.box_);
        layoutRect_ = std::move(other.layoutRect_);
        intrinsicSize_ = std::move(other.intrinsicSize_);
        minSize_ = std::move(other.minSize_);
        maxSize_ = std::move(other.maxSize_);
        isLayoutDirty_ = other.isLayoutDirty_;
        needsLayout_ = other.needsLayout_;
        textContent_ = std::move(other.textContent_);
        fontMetrics_ = std::move(other.fontMetrics_);
        lineHeight_ = other.lineHeight_;
        baseline_ = other.baseline_;
        parent_ = other.parent_;
        children_ = std::move(other.children_);
        floats_ = std::move(other.floats_);
    }
    return *this;
}

void LayoutNode::updateParentChildRelationships() {
    for (auto* child : children_) {
        child->parent_ = this;
    }
}

void LayoutNode::collectDescendants(LayoutNode* node, std::vector<LayoutNode*>& descendants) {
    for (auto* child : node->children_) {
        descendants.push_back(child);
        collectDescendants(child, descendants);
    }
}

// LayoutTree implementation
LayoutTree::LayoutTree() = default;

LayoutTree::~LayoutTree() = default;

void LayoutTree::setRoot(std::unique_ptr<LayoutNode> root) {
    root_ = std::move(root);
}

std::unique_ptr<LayoutNode> LayoutTree::createNode() {
    return std::make_unique<LayoutNode>();
}

std::unique_ptr<LayoutNode> LayoutTree::createNode(std::shared_ptr<LayoutBox> box) {
    return std::make_unique<LayoutNode>(box);
}

void LayoutTree::addChild(LayoutNode* parent, LayoutNode* child) {
    if (parent && child) {
        parent->addChild(child);
    }
}

void LayoutTree::removeNode(LayoutNode* node) {
    if (node && node->parent()) {
        node->parent()->removeChild(node);
    } else if (node == root_.get()) {
        root_.reset();
    }
}

void LayoutTree::moveNode(LayoutNode* node, LayoutNode* newParent) {
    if (node && newParent && node != newParent) {
        if (node->parent()) {
            node->parent()->removeChild(node);
        }
        newParent->addChild(node);
    }
}

void LayoutTree::clear() {
    root_.reset();
}

std::vector<LayoutNode*> LayoutTree::getAllNodes() const {
    std::vector<LayoutNode*> nodes;
    if (root_) {
        nodes.push_back(root_.get());
        collectAllNodes(root_.get(), nodes);
    }
    return nodes;
}

std::vector<LayoutNode*> LayoutTree::getLeafNodes() const {
    std::vector<LayoutNode*> nodes;
    if (root_) {
        collectLeafNodes(root_.get(), nodes);
    }
    return nodes;
}

std::vector<LayoutNode*> LayoutTree::getNodesAtDepth(size_t depth) const {
    std::vector<LayoutNode*> nodes;
    if (root_) {
        collectNodesAtDepth(root_.get(), depth, nodes);
    }
    return nodes;
}

size_t LayoutTree::nodeCount() const {
    return getAllNodes().size();
}

size_t LayoutTree::height() const {
    return root_ ? root_->height() : 0;
}

LayoutNode* LayoutTree::findNode(const LayoutBox* box) const {
    if (!root_ || !box) return nullptr;
    return findNodeRecursive(root_.get(), box);
}

std::vector<LayoutNode*> LayoutTree::findNodesByDisplay(Display display) const {
    std::vector<LayoutNode*> nodes;
    if (root_) {
        collectNodesByDisplay(root_.get(), display, nodes);
    }
    return nodes;
}

std::vector<LayoutNode*> LayoutTree::findNodesByPosition(Position position) const {
    std::vector<LayoutNode*> nodes;
    if (root_) {
        collectNodesByPosition(root_.get(), position, nodes);
    }
    return nodes;
}

std::vector<LayoutNode*> LayoutTree::findPositionedNodes() const {
    std::vector<LayoutNode*> nodes;
    if (root_) {
        collectPositionedNodes(root_.get(), nodes);
    }
    return nodes;
}

std::vector<LayoutNode*> LayoutTree::findFloatingNodes() const {
    std::vector<LayoutNode*> nodes;
    if (root_) {
        collectFloatingNodes(root_.get(), nodes);
    }
    return nodes;
}

std::vector<LayoutNode*> LayoutTree::findBlockLevelNodes() const {
    std::vector<LayoutNode*> nodes;
    if (root_) {
        collectBlockLevelNodes(root_.get(), nodes);
    }
    return nodes;
}

std::vector<LayoutNode*> LayoutTree::findInlineLevelNodes() const {
    std::vector<LayoutNode*> nodes;
    if (root_) {
        collectInlineLevelNodes(root_.get(), nodes);
    }
    return nodes;
}

std::vector<LayoutNode*> LayoutTree::findStackingContextNodes() const {
    std::vector<LayoutNode*> nodes;
    if (root_) {
        collectStackingContextNodes(root_.get(), nodes);
    }
    return nodes;
}

void LayoutTree::layout(const LayoutConstraints& constraints) {
    if (root_) {
        root_->layout(constraints);
    }
}

void LayoutTree::updateLayout() {
    if (root_) {
        root_->updateLayout();
    }
}

void LayoutTree::invalidateLayout() {
    if (root_) {
        root_->invalidateAll();
    }
}

bool LayoutTree::isValid() const {
    return root_ ? validateNode(root_.get()) : true;
}

std::unique_ptr<LayoutTree> LayoutTree::clone() const {
    auto cloned = std::make_unique<LayoutTree>();
    if (root_) {
        cloned->root_ = root_->clone();
    }
    return cloned;
}

void LayoutTree::collectAllNodes(LayoutNode* node, std::vector<LayoutNode*>& nodes) const {
    for (auto* child : node->children()) {
        nodes.push_back(child);
        collectAllNodes(child, nodes);
    }
}

void LayoutTree::collectLeafNodes(LayoutNode* node, std::vector<LayoutNode*>& nodes) const {
    if (node->children().empty()) {
        nodes.push_back(node);
    } else {
        for (auto* child : node->children()) {
            collectLeafNodes(child, nodes);
        }
    }
}

void LayoutTree::collectNodesAtDepth(LayoutNode* node, size_t depth, std::vector<LayoutNode*>& nodes) const {
    if (depth == 0) {
        nodes.push_back(node);
        return;
    }
    
    for (auto* child : node->children()) {
        collectNodesAtDepth(child, depth - 1, nodes);
    }
}

void LayoutTree::collectNodesByDisplay(LayoutNode* node, Display display, std::vector<LayoutNode*>& nodes) const {
    if (node->box() && node->box()->display() == display) {
        nodes.push_back(node);
    }
    
    for (auto* child : node->children()) {
        collectNodesByDisplay(child, display, nodes);
    }
}

void LayoutTree::collectNodesByPosition(LayoutNode* node, Position position, std::vector<LayoutNode*>& nodes) const {
    if (node->box() && node->box()->position() == position) {
        nodes.push_back(node);
    }
    
    for (auto* child : node->children()) {
        collectNodesByPosition(child, position, nodes);
    }
}

void LayoutTree::collectPositionedNodes(LayoutNode* node, std::vector<LayoutNode*>& nodes) const {
    if (node->isPositioned()) {
        nodes.push_back(node);
    }
    
    for (auto* child : node->children()) {
        collectPositionedNodes(child, nodes);
    }
}

void LayoutTree::collectFloatingNodes(LayoutNode* node, std::vector<LayoutNode*>& nodes) const {
    if (node->isFloating()) {
        nodes.push_back(node);
    }
    
    for (auto* child : node->children()) {
        collectFloatingNodes(child, nodes);
    }
}

void LayoutTree::collectBlockLevelNodes(LayoutNode* node, std::vector<LayoutNode*>& nodes) const {
    if (node->isBlockLevel()) {
        nodes.push_back(node);
    }
    
    for (auto* child : node->children()) {
        collectBlockLevelNodes(child, nodes);
    }
}

void LayoutTree::collectInlineLevelNodes(LayoutNode* node, std::vector<LayoutNode*>& nodes) const {
    if (node->isInlineLevel()) {
        nodes.push_back(node);
    }
    
    for (auto* child : node->children()) {
        collectInlineLevelNodes(child, nodes);
    }
}

void LayoutTree::collectStackingContextNodes(LayoutNode* node, std::vector<LayoutNode*>& nodes) const {
    if (node->isStackingContext()) {
        nodes.push_back(node);
    }
    
    for (auto* child : node->children()) {
        collectStackingContextNodes(child, nodes);
    }
}

bool LayoutTree::validateNode(LayoutNode* node) const {
    if (!node) return false;
    
    // Check parent-child relationships
    for (auto* child : node->children()) {
        if (child->parent() != node) {
            return false;
        }
        if (!validateNode(child)) {
            return false;
        }
    }
    
    return true;
}

LayoutNode* LayoutTree::findNodeRecursive(LayoutNode* node, const LayoutBox* box) const {
    if (node->box() == box) {
        return node;
    }
    
    for (auto* child : node->children()) {
        LayoutNode* found = findNodeRecursive(child, box);
        if (found) {
            return found;
        }
    }
    
    return nullptr;
}

} // namespace layout
