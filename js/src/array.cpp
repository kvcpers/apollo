#include "js/array.h"

namespace js {

Array::Array() : Value(ValueType::Array) {}

Array::~Array() {}

void Array::push(std::shared_ptr<Value> value) {
    elements.push_back(value);
}

std::shared_ptr<Value> Array::get(size_t index) const {
    if (index < elements.size()) {
        return elements[index];
    }
    return nullptr;
}

void Array::set(size_t index, std::shared_ptr<Value> value) {
    if (index < elements.size()) {
        elements[index] = value;
    }
}

size_t Array::length() const {
    return elements.size();
}

void Array::clear() {
    elements.clear();
}

} // namespace js
