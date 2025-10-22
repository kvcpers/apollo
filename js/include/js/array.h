#pragma once

#include "js/value.h"
#include <vector>

namespace js {

class Array : public Value {
public:
    Array();
    ~Array();

    void push(std::shared_ptr<Value> value);
    std::shared_ptr<Value> get(size_t index) const;
    void set(size_t index, std::shared_ptr<Value> value);
    size_t length() const;
    void clear();

private:
    std::vector<std::shared_ptr<Value>> elements;
};

} // namespace js
