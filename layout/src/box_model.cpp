#include "layout/box_model.h"
#include <algorithm>
#include <limits>

namespace layout {

// BoxModel implementation
BoxModel::BoxModel() 
    : contentRect_(0, 0, 0, 0)
    , padding_(0)
    , border_(0)
    , margin_(0)
    , boxSizing_(BoxSizing::ContentBox) {
}

BoxModel::~BoxModel() = default;

Rect BoxModel::paddingRect() const {
    return Rect(
        contentRect_.x - padding_.left,
        contentRect_.y - padding_.top,
        contentRect_.width + padding_.horizontal(),
        contentRect_.height + padding_.vertical()
    );
}

Rect BoxModel::borderRect() const {
    return Rect(
        contentRect_.x - padding_.left - border_.left,
        contentRect_.y - padding_.top - border_.top,
        contentRect_.width + padding_.horizontal() + border_.horizontal(),
        contentRect_.height + padding_.vertical() + border_.vertical()
    );
}

Rect BoxModel::marginRect() const {
    return Rect(
        contentRect_.x - padding_.left - border_.left - margin_.left,
        contentRect_.y - padding_.top - border_.top - margin_.top,
        contentRect_.width + padding_.horizontal() + border_.horizontal() + margin_.horizontal(),
        contentRect_.height + padding_.vertical() + border_.vertical() + margin_.vertical()
    );
}

Rect BoxModel::totalRect() const {
    return marginRect();
}

Size BoxModel::calculateContentSize(const Size& totalSize) const {
    if (boxSizing_ == BoxSizing::BorderBox) {
        return Size(
            totalSize.width - padding_.horizontal() - border_.horizontal(),
            totalSize.height - padding_.vertical() - border_.vertical()
        );
    } else {
        return totalSize;
    }
}

Size BoxModel::calculateTotalSize(const Size& contentSize) const {
    if (boxSizing_ == BoxSizing::BorderBox) {
        return Size(
            contentSize.width + padding_.horizontal() + border_.horizontal(),
            contentSize.height + padding_.vertical() + border_.vertical()
        );
    } else {
        return Size(
            contentSize.width + padding_.horizontal() + border_.horizontal() + margin_.horizontal(),
            contentSize.height + padding_.vertical() + border_.vertical() + margin_.vertical()
        );
    }
}

bool BoxModel::isEmpty() const {
    return contentRect_.isEmpty() && padding_.horizontal() == 0 && padding_.vertical() == 0 &&
           border_.horizontal() == 0 && border_.vertical() == 0 && margin_.horizontal() == 0 && margin_.vertical() == 0;
}

Rect BoxModel::visualBounds() const {
    return borderRect();
}

Rect BoxModel::layoutBounds() const {
    return marginRect();
}

bool BoxModel::contains(const Point& point) const {
    return borderRect().contains(point);
}

bool BoxModel::intersects(const Rect& rect) const {
    return borderRect().intersects(rect);
}

Rect BoxModel::intersection(const BoxModel& other) const {
    return borderRect().intersection(other.borderRect());
}

Rect BoxModel::unionRect(const BoxModel& other) const {
    return borderRect().unionRect(other.borderRect());
}

void BoxModel::transform(const Transform& transform) {
    contentRect_ = transform.transform(contentRect_);
}

void BoxModel::reset() {
    contentRect_ = Rect(0, 0, 0, 0);
    padding_ = EdgeInsets(0);
    border_ = EdgeInsets(0);
    margin_ = EdgeInsets(0);
    boxSizing_ = BoxSizing::ContentBox;
}

BoxModel::BoxModel(const BoxModel& other)
    : contentRect_(other.contentRect_)
    , padding_(other.padding_)
    , border_(other.border_)
    , margin_(other.margin_)
    , boxSizing_(other.boxSizing_) {
}

BoxModel& BoxModel::operator=(const BoxModel& other) {
    if (this != &other) {
        contentRect_ = other.contentRect_;
        padding_ = other.padding_;
        border_ = other.border_;
        margin_ = other.margin_;
        boxSizing_ = other.boxSizing_;
    }
    return *this;
}

BoxModel::BoxModel(BoxModel&& other) noexcept
    : contentRect_(std::move(other.contentRect_))
    , padding_(std::move(other.padding_))
    , border_(std::move(other.border_))
    , margin_(std::move(other.margin_))
    , boxSizing_(other.boxSizing_) {
}

BoxModel& BoxModel::operator=(BoxModel&& other) noexcept {
    if (this != &other) {
        contentRect_ = std::move(other.contentRect_);
        padding_ = std::move(other.padding_);
        border_ = std::move(other.border_);
        margin_ = std::move(other.margin_);
        boxSizing_ = other.boxSizing_;
    }
    return *this;
}

// LayoutBox implementation
LayoutBox::LayoutBox()
    : display_(Display::Block)
    , position_(Position::Static)
    , float_(Float::None)
    , clear_(Clear::None)
    , zIndex_(0)
    , transform_(Transform::identity())
    , opacity_(1.0)
    , visibility_(Visibility::Visible)
    , overflow_(Overflow::Visible)
    , clipRect_(0, 0, 0, 0)
    , isReplaced_(false)
    , isAnonymous_(false)
    , isRoot_(false) {
}

LayoutBox::~LayoutBox() = default;

bool LayoutBox::isPositioned() const {
    return position_ == Position::Absolute || position_ == Position::Fixed || 
           position_ == Position::Relative || position_ == Position::Sticky;
}

bool LayoutBox::isFloating() const {
    return float_ != Float::None;
}

bool LayoutBox::isBlockLevel() const {
    return display_ == Display::Block || display_ == Display::ListItem || 
           display_ == Display::Table || display_ == Display::Flex || 
           display_ == Display::Grid || display_ == Display::InlineBlock ||
           display_ == Display::InlineFlex || display_ == Display::InlineGrid ||
           display_ == Display::InlineTable;
}

bool LayoutBox::isInlineLevel() const {
    return display_ == Display::Inline || display_ == Display::InlineBlock ||
           display_ == Display::InlineFlex || display_ == Display::InlineGrid ||
           display_ == Display::InlineTable;
}

bool LayoutBox::isTableCell() const {
    return display_ == Display::TableCell;
}

bool LayoutBox::isTableRow() const {
    return display_ == Display::TableRow;
}

bool LayoutBox::isTable() const {
    return display_ == Display::Table || display_ == Display::InlineTable;
}

bool LayoutBox::isFlexContainer() const {
    return display_ == Display::Flex || display_ == Display::InlineFlex;
}

bool LayoutBox::isFlexItem() const {
    // A flex item is a child of a flex container
    // This would need to be determined by the parent's display type
    return false;
}

bool LayoutBox::isGridContainer() const {
    return display_ == Display::Grid || display_ == Display::InlineGrid;
}

bool LayoutBox::isGridItem() const {
    // A grid item is a child of a grid container
    // This would need to be determined by the parent's display type
    return false;
}

bool LayoutBox::isStackingContext() const {
    return isPositioned() || isFlexContainer() || isGridContainer() || 
           opacity_ < 1.0 || transform_ != Transform::identity() ||
           zIndex_ != 0 || isRoot_;
}

bool LayoutBox::isContainingBlock() const {
    return isPositioned() || isRoot_;
}

bool LayoutBox::isFormattingContextRoot() const {
    return isRoot_ || isFlexContainer() || isGridContainer() || isTable();
}

void LayoutBox::reset() {
    BoxModel::reset();
    display_ = Display::Block;
    position_ = Position::Static;
    float_ = Float::None;
    clear_ = Clear::None;
    zIndex_ = 0;
    transform_ = Transform::identity();
    opacity_ = 1.0;
    visibility_ = Visibility::Visible;
    overflow_ = Overflow::Visible;
    clipRect_ = Rect(0, 0, 0, 0);
    isReplaced_ = false;
    isAnonymous_ = false;
    isRoot_ = false;
}

LayoutBox::LayoutBox(const LayoutBox& other)
    : BoxModel(other)
    , display_(other.display_)
    , position_(other.position_)
    , float_(other.float_)
    , clear_(other.clear_)
    , zIndex_(other.zIndex_)
    , transform_(other.transform_)
    , opacity_(other.opacity_)
    , visibility_(other.visibility_)
    , overflow_(other.overflow_)
    , clipRect_(other.clipRect_)
    , isReplaced_(other.isReplaced_)
    , isAnonymous_(other.isAnonymous_)
    , isRoot_(other.isRoot_) {
}

LayoutBox& LayoutBox::operator=(const LayoutBox& other) {
    if (this != &other) {
        BoxModel::operator=(other);
        display_ = other.display_;
        position_ = other.position_;
        float_ = other.float_;
        clear_ = other.clear_;
        zIndex_ = other.zIndex_;
        transform_ = other.transform_;
        opacity_ = other.opacity_;
        visibility_ = other.visibility_;
        overflow_ = other.overflow_;
        clipRect_ = other.clipRect_;
        isReplaced_ = other.isReplaced_;
        isAnonymous_ = other.isAnonymous_;
        isRoot_ = other.isRoot_;
    }
    return *this;
}

LayoutBox::LayoutBox(LayoutBox&& other) noexcept
    : BoxModel(std::move(other))
    , display_(other.display_)
    , position_(other.position_)
    , float_(other.float_)
    , clear_(other.clear_)
    , zIndex_(other.zIndex_)
    , transform_(std::move(other.transform_))
    , opacity_(other.opacity_)
    , visibility_(other.visibility_)
    , overflow_(other.overflow_)
    , clipRect_(std::move(other.clipRect_))
    , isReplaced_(other.isReplaced_)
    , isAnonymous_(other.isAnonymous_)
    , isRoot_(other.isRoot_) {
}

LayoutBox& LayoutBox::operator=(LayoutBox&& other) noexcept {
    if (this != &other) {
        BoxModel::operator=(std::move(other));
        display_ = other.display_;
        position_ = other.position_;
        float_ = other.float_;
        clear_ = other.clear_;
        zIndex_ = other.zIndex_;
        transform_ = std::move(other.transform_);
        opacity_ = other.opacity_;
        visibility_ = other.visibility_;
        overflow_ = other.overflow_;
        clipRect_ = std::move(other.clipRect_);
        isReplaced_ = other.isReplaced_;
        isAnonymous_ = other.isAnonymous_;
        isRoot_ = other.isRoot_;
    }
    return *this;
}

// BoxNode implementation
BoxNode::BoxNode() : box_(nullptr), parent_(nullptr) {
}

BoxNode::BoxNode(std::shared_ptr<LayoutBox> box) : box_(box), parent_(nullptr) {
}

BoxNode::~BoxNode() {
    clearChildren();
}

void BoxNode::addChild(BoxNode* child) {
    if (child && child != this) {
        child->setParent(this);
        children_.push_back(child);
    }
}

void BoxNode::insertChild(BoxNode* child, size_t index) {
    if (child && child != this && index <= children_.size()) {
        child->setParent(this);
        children_.insert(children_.begin() + index, child);
    }
}

void BoxNode::removeChild(BoxNode* child) {
    auto it = std::find(children_.begin(), children_.end(), child);
    if (it != children_.end()) {
        (*it)->setParent(nullptr);
        children_.erase(it);
    }
}

void BoxNode::removeChildAt(size_t index) {
    if (index < children_.size()) {
        children_[index]->setParent(nullptr);
        children_.erase(children_.begin() + index);
    }
}

void BoxNode::clearChildren() {
    for (auto* child : children_) {
        child->setParent(nullptr);
    }
    children_.clear();
}

BoxNode* BoxNode::childAt(size_t index) const {
    return (index < children_.size()) ? children_[index] : nullptr;
}

BoxNode* BoxNode::firstChild() const {
    return children_.empty() ? nullptr : children_.front();
}

BoxNode* BoxNode::lastChild() const {
    return children_.empty() ? nullptr : children_.back();
}

BoxNode* BoxNode::nextSibling() const {
    if (!parent_) return nullptr;
    
    auto it = std::find(parent_->children_.begin(), parent_->children_.end(), this);
    if (it != parent_->children_.end() && ++it != parent_->children_.end()) {
        return *it;
    }
    return nullptr;
}

BoxNode* BoxNode::previousSibling() const {
    if (!parent_) return nullptr;
    
    auto it = std::find(parent_->children_.begin(), parent_->children_.end(), this);
    if (it != parent_->children_.begin()) {
        return *(--it);
    }
    return nullptr;
}

size_t BoxNode::indexInParent() const {
    if (!parent_) return 0;
    
    auto it = std::find(parent_->children_.begin(), parent_->children_.end(), this);
    return (it != parent_->children_.end()) ? std::distance(parent_->children_.begin(), it) : 0;
}

BoxNode* BoxNode::findChild(size_t index) const {
    return childAt(index);
}

BoxNode* BoxNode::findChild(const LayoutBox* box) const {
    for (auto* child : children_) {
        if (child->box() == box) {
            return child;
        }
    }
    return nullptr;
}

size_t BoxNode::depth() const {
    size_t depth = 0;
    BoxNode* current = parent_;
    while (current) {
        ++depth;
        current = current->parent_;
    }
    return depth;
}

size_t BoxNode::height() const {
    if (children_.empty()) return 0;
    
    size_t maxChildHeight = 0;
    for (auto* child : children_) {
        maxChildHeight = std::max(maxChildHeight, child->height());
    }
    return maxChildHeight + 1;
}

std::vector<BoxNode*> BoxNode::getAllDescendants() const {
    std::vector<BoxNode*> descendants;
    collectDescendants(this, descendants);
    return descendants;
}

std::vector<BoxNode*> BoxNode::getAllAncestors() const {
    std::vector<BoxNode*> ancestors;
    BoxNode* current = parent_;
    while (current) {
        ancestors.push_back(current);
        current = current->parent_;
    }
    return ancestors;
}

std::vector<BoxNode*> BoxNode::getSiblings() const {
    if (!parent_) return {};
    
    std::vector<BoxNode*> siblings;
    for (auto* child : parent_->children_) {
        if (child != this) {
            siblings.push_back(child);
        }
    }
    return siblings;
}

std::vector<BoxNode*> BoxNode::getPreviousSiblings() const {
    if (!parent_) return {};
    
    std::vector<BoxNode*> siblings;
    size_t index = indexInParent();
    for (size_t i = 0; i < index; ++i) {
        siblings.push_back(parent_->children_[i]);
    }
    return siblings;
}

std::vector<BoxNode*> BoxNode::getNextSiblings() const {
    if (!parent_) return {};
    
    std::vector<BoxNode*> siblings;
    size_t index = indexInParent();
    for (size_t i = index + 1; i < parent_->children_.size(); ++i) {
        siblings.push_back(parent_->children_[i]);
    }
    return siblings;
}

BoxNode* BoxNode::findCommonAncestor(BoxNode* other) const {
    if (!other) return nullptr;
    
    std::vector<BoxNode*> thisAncestors = getAllAncestors();
    std::vector<BoxNode*> otherAncestors = other->getAllAncestors();
    
    for (auto* ancestor : thisAncestors) {
        if (std::find(otherAncestors.begin(), otherAncestors.end(), ancestor) != otherAncestors.end()) {
            return ancestor;
        }
    }
    return nullptr;
}

std::vector<BoxNode*> BoxNode::getPathToRoot() const {
    std::vector<BoxNode*> path;
    BoxNode* current = const_cast<BoxNode*>(this);
    while (current) {
        path.push_back(current);
        current = current->parent_;
    }
    return path;
}

std::vector<BoxNode*> BoxNode::getPathToAncestor(BoxNode* ancestor) const {
    std::vector<BoxNode*> path;
    BoxNode* current = const_cast<BoxNode*>(this);
    while (current && current != ancestor) {
        path.push_back(current);
        current = current->parent_;
    }
    if (current == ancestor) {
        path.push_back(current);
    }
    return path;
}

bool BoxNode::isAncestorOf(BoxNode* node) const {
    if (!node) return false;
    
    BoxNode* current = node->parent_;
    while (current) {
        if (current == this) return true;
        current = current->parent_;
    }
    return false;
}

bool BoxNode::isDescendantOf(BoxNode* node) const {
    return node ? node->isAncestorOf(this) : false;
}

bool BoxNode::isSiblingOf(BoxNode* node) const {
    return node && parent_ && parent_ == node->parent_;
}

std::unique_ptr<BoxNode> BoxNode::clone() const {
    auto cloned = std::make_unique<BoxNode>();
    if (box_) {
        cloned->box_ = std::make_shared<LayoutBox>(*box_);
    }
    
    for (auto* child : children_) {
        auto clonedChild = child->clone();
        cloned->addChild(clonedChild.release());
    }
    
    return cloned;
}

std::unique_ptr<BoxNode> BoxNode::cloneShallow() const {
    auto cloned = std::make_unique<BoxNode>();
    if (box_) {
        cloned->box_ = std::make_shared<LayoutBox>(*box_);
    }
    return cloned;
}

void BoxNode::updateParentChildRelationships() {
    for (auto* child : children_) {
        child->parent_ = this;
    }
}

void BoxNode::collectDescendants(const BoxNode* node, std::vector<BoxNode*>& descendants) {
    for (auto* child : node->children_) {
        descendants.push_back(child);
        collectDescendants(child, descendants);
    }
}

// BoxTree implementation
BoxTree::BoxTree() = default;

BoxTree::~BoxTree() = default;

void BoxTree::setRoot(std::unique_ptr<BoxNode> root) {
    root_ = std::move(root);
}

std::unique_ptr<BoxNode> BoxTree::createNode() {
    return std::make_unique<BoxNode>();
}

std::unique_ptr<BoxNode> BoxTree::createNode(std::shared_ptr<LayoutBox> box) {
    return std::make_unique<BoxNode>(box);
}

void BoxTree::addChild(BoxNode* parent, BoxNode* child) {
    if (parent && child) {
        parent->addChild(child);
    }
}

void BoxTree::removeNode(BoxNode* node) {
    if (node && node->parent()) {
        node->parent()->removeChild(node);
    } else if (node == root_.get()) {
        root_.reset();
    }
}

void BoxTree::moveNode(BoxNode* node, BoxNode* newParent) {
    if (node && newParent && node != newParent) {
        if (node->parent()) {
            node->parent()->removeChild(node);
        }
        newParent->addChild(node);
    }
}

void BoxTree::clear() {
    root_.reset();
}

std::vector<BoxNode*> BoxTree::getAllNodes() const {
    std::vector<BoxNode*> nodes;
    if (root_) {
        nodes.push_back(root_.get());
        collectAllNodes(root_.get(), nodes);
    }
    return nodes;
}

std::vector<BoxNode*> BoxTree::getLeafNodes() const {
    std::vector<BoxNode*> nodes;
    if (root_) {
        collectLeafNodes(root_.get(), nodes);
    }
    return nodes;
}

std::vector<BoxNode*> BoxTree::getNodesAtDepth(size_t depth) const {
    std::vector<BoxNode*> nodes;
    if (root_) {
        collectNodesAtDepth(root_.get(), depth, nodes);
    }
    return nodes;
}

size_t BoxTree::nodeCount() const {
    return getAllNodes().size();
}

size_t BoxTree::height() const {
    return root_ ? root_->height() : 0;
}

BoxNode* BoxTree::findNode(const LayoutBox* box) const {
    if (!root_ || !box) return nullptr;
    return findNodeRecursive(root_.get(), box);
}

std::vector<BoxNode*> BoxTree::findNodesByDisplay(Display display) const {
    std::vector<BoxNode*> nodes;
    if (root_) {
        collectNodesByDisplay(root_.get(), display, nodes);
    }
    return nodes;
}

std::vector<BoxNode*> BoxTree::findNodesByPosition(Position position) const {
    std::vector<BoxNode*> nodes;
    if (root_) {
        collectNodesByPosition(root_.get(), position, nodes);
    }
    return nodes;
}

std::vector<BoxNode*> BoxTree::findPositionedNodes() const {
    std::vector<BoxNode*> nodes;
    if (root_) {
        collectPositionedNodes(root_.get(), nodes);
    }
    return nodes;
}

std::vector<BoxNode*> BoxTree::findFloatingNodes() const {
    std::vector<BoxNode*> nodes;
    if (root_) {
        collectFloatingNodes(root_.get(), nodes);
    }
    return nodes;
}

std::vector<BoxNode*> BoxTree::findBlockLevelNodes() const {
    std::vector<BoxNode*> nodes;
    if (root_) {
        collectBlockLevelNodes(root_.get(), nodes);
    }
    return nodes;
}

std::vector<BoxNode*> BoxTree::findInlineLevelNodes() const {
    std::vector<BoxNode*> nodes;
    if (root_) {
        collectInlineLevelNodes(root_.get(), nodes);
    }
    return nodes;
}

std::vector<BoxNode*> BoxTree::findStackingContextNodes() const {
    std::vector<BoxNode*> nodes;
    if (root_) {
        collectStackingContextNodes(root_.get(), nodes);
    }
    return nodes;
}

bool BoxTree::isValid() const {
    return root_ ? validateNode(root_.get()) : true;
}

std::unique_ptr<BoxTree> BoxTree::clone() const {
    auto cloned = std::make_unique<BoxTree>();
    if (root_) {
        cloned->root_ = root_->clone();
    }
    return cloned;
}

void BoxTree::collectAllNodes(BoxNode* node, std::vector<BoxNode*>& nodes) const {
    for (auto* child : node->children()) {
        nodes.push_back(child);
        collectAllNodes(child, nodes);
    }
}

void BoxTree::collectLeafNodes(BoxNode* node, std::vector<BoxNode*>& nodes) const {
    if (node->children().empty()) {
        nodes.push_back(node);
    } else {
        for (auto* child : node->children()) {
            collectLeafNodes(child, nodes);
        }
    }
}

void BoxTree::collectNodesAtDepth(BoxNode* node, size_t depth, std::vector<BoxNode*>& nodes) const {
    if (depth == 0) {
        nodes.push_back(node);
        return;
    }
    
    for (auto* child : node->children()) {
        collectNodesAtDepth(child, depth - 1, nodes);
    }
}

void BoxTree::collectNodesByDisplay(BoxNode* node, Display display, std::vector<BoxNode*>& nodes) const {
    if (node->box() && node->box()->display() == display) {
        nodes.push_back(node);
    }
    
    for (auto* child : node->children()) {
        collectNodesByDisplay(child, display, nodes);
    }
}

void BoxTree::collectNodesByPosition(BoxNode* node, Position position, std::vector<BoxNode*>& nodes) const {
    if (node->box() && node->box()->position() == position) {
        nodes.push_back(node);
    }
    
    for (auto* child : node->children()) {
        collectNodesByPosition(child, position, nodes);
    }
}

void BoxTree::collectPositionedNodes(BoxNode* node, std::vector<BoxNode*>& nodes) const {
    if (node->box() && node->box()->isPositioned()) {
        nodes.push_back(node);
    }
    
    for (auto* child : node->children()) {
        collectPositionedNodes(child, nodes);
    }
}

void BoxTree::collectFloatingNodes(BoxNode* node, std::vector<BoxNode*>& nodes) const {
    if (node->box() && node->box()->isFloating()) {
        nodes.push_back(node);
    }
    
    for (auto* child : node->children()) {
        collectFloatingNodes(child, nodes);
    }
}

void BoxTree::collectBlockLevelNodes(BoxNode* node, std::vector<BoxNode*>& nodes) const {
    if (node->box() && node->box()->isBlockLevel()) {
        nodes.push_back(node);
    }
    
    for (auto* child : node->children()) {
        collectBlockLevelNodes(child, nodes);
    }
}

void BoxTree::collectInlineLevelNodes(BoxNode* node, std::vector<BoxNode*>& nodes) const {
    if (node->box() && node->box()->isInlineLevel()) {
        nodes.push_back(node);
    }
    
    for (auto* child : node->children()) {
        collectInlineLevelNodes(child, nodes);
    }
}

void BoxTree::collectStackingContextNodes(BoxNode* node, std::vector<BoxNode*>& nodes) const {
    if (node->box() && node->box()->isStackingContext()) {
        nodes.push_back(node);
    }
    
    for (auto* child : node->children()) {
        collectStackingContextNodes(child, nodes);
    }
}

bool BoxTree::validateNode(BoxNode* node) const {
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

BoxNode* BoxTree::findNodeRecursive(BoxNode* node, const LayoutBox* box) const {
    if (node->box() == box) {
        return node;
    }
    
    for (auto* child : node->children()) {
        BoxNode* found = findNodeRecursive(child, box);
        if (found) {
            return found;
        }
    }
    
    return nullptr;
}

} // namespace layout
