#pragma once

#include "layout_node.h"
#include "types.h"
#include <memory>
#include <vector>

namespace layout {

// Stacking context for z-index ordering
class StackingContext {
public:
    StackingContext();
    ~StackingContext();

    // Create stacking context
    void createStackingContext(LayoutNode* node);

    // Destroy stacking context
    void destroyStackingContext(LayoutNode* node);

    // Add node to stacking context
    void addNode(LayoutNode* node);

    // Remove node from stacking context
    void removeNode(LayoutNode* node);

    // Get stacking order
    std::vector<LayoutNode*> getStackingOrder() const;

    // Sort nodes by z-index
    void sortNodesByZIndex();

    // Get nodes at z-index
    std::vector<LayoutNode*> getNodesAtZIndex(ZIndex zIndex) const;

    // Get highest z-index
    ZIndex getHighestZIndex() const;

    // Get lowest z-index
    ZIndex getLowestZIndex() const;

    // Check if node is in stacking context
    bool containsNode(LayoutNode* node) const;

    // Get stacking context root
    LayoutNode* getRoot() const { return root_; }

    // Set stacking context root
    void setRoot(LayoutNode* root) { root_ = root; }

    // Clear stacking context
    void clear();

    // Get node count
    size_t nodeCount() const { return nodes_.size(); }

    // Is empty
    bool isEmpty() const { return nodes_.empty(); }

private:
    LayoutNode* root_;
    std::vector<LayoutNode*> nodes_;

    void sortNodesByZIndexRecursive(std::vector<LayoutNode*>& nodes);
    void collectNodesAtZIndex(LayoutNode* node, ZIndex zIndex, std::vector<LayoutNode*>& result) const;
    ZIndex getNodeZIndex(LayoutNode* node) const;
    bool isNodeInStackingContext(LayoutNode* node) const;
};

// Stacking context manager
class StackingContextManager {
public:
    StackingContextManager();
    ~StackingContextManager();

    // Create stacking context for node
    void createStackingContext(LayoutNode* node);

    // Destroy stacking context for node
    void destroyStackingContext(LayoutNode* node);

    // Get stacking context for node
    StackingContext* getStackingContext(LayoutNode* node);

    // Get all stacking contexts
    std::vector<StackingContext*> getAllStackingContexts() const;

    // Get stacking order for all contexts
    std::vector<LayoutNode*> getGlobalStackingOrder() const;

    // Clear all stacking contexts
    void clear();

    // Get stacking context count
    size_t stackingContextCount() const { return contexts_.size(); }

    // Is empty
    bool isEmpty() const { return contexts_.empty(); }

private:
    std::vector<std::unique_ptr<StackingContext>> contexts_;

    StackingContext* findStackingContext(LayoutNode* node) const;
    void sortStackingContexts();
    void collectGlobalStackingOrder(std::vector<LayoutNode*>& order) const;
    bool isStackingContextRoot(LayoutNode* node) const;
};

} // namespace layout
