#pragma once

#include "layout/layout_node.h"
#include <vector>
#include <memory>

namespace layout {

class StackingContext {
public:
    StackingContext();
    ~StackingContext();

    void add_element(std::shared_ptr<LayoutNode> element);
    void remove_element(std::shared_ptr<LayoutNode> element);
    std::vector<std::shared_ptr<LayoutNode>> get_elements() const;

    void set_z_index(int z);
    int get_z_index() const;

    void sort_by_z_index();

private:
    std::vector<std::shared_ptr<LayoutNode>> elements;
    int z_index;
};

} // namespace layout