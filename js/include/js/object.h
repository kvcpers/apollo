#pragma once

#include "js/value.h"
#include <map>
#include <vector>
#include <string>

namespace js {

class Object : public Value {
public:
    Object();
    ~Object();

    void set_property(const std::string& name, std::shared_ptr<Value> value);
    std::shared_ptr<Value> get_property(const std::string& name);
    bool has_property(const std::string& name) const;
    void remove_property(const std::string& name);
    std::vector<std::string> get_property_names() const;

private:
    std::map<std::string, std::shared_ptr<Value>> properties;
};

} // namespace js
