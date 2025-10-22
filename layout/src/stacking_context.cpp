#include "layout/stacking_context.h"

namespace layout {

StackingContext::StackingContext() : z_index(0) {}

StackingContext::~StackingContext() {}

void StackingContext::add_element(std::shared_ptr<LayoutNode> element) {
    elements.push_back(element);
}

void StackingContext::remove_element(std::shared_ptr<LayoutNode> element) {
    elements.erase(
        std::remove(elements.begin(), elements.end(), element),
        elements.end()
    );
}

std::vector<std::shared_ptr<LayoutNode>> StackingContext::get_elements() const {
    return elements;
}

void StackingContext::set_z_index(int z) {
    z_index = z;
}

int StackingContext::get_z_index() const {
    return z_index;
}

void StackingContext::sort_by_z_index() {
    std::sort(elements.begin(), elements.end(),
        [](const std::shared_ptr<LayoutNode>& a, const std::shared_ptr<LayoutNode>& b) {
            return a->get_z_index() < b->get_z_index();
        });
}

} // namespace layout
