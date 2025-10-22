#include "js/object.h"

namespace js {

Object::Object() : Value(ValueType::Object) {}

Object::~Object() {}

void Object::set_property(const std::string& name, std::shared_ptr<Value> value) {
    properties[name] = value;
}

std::shared_ptr<Value> Object::get_property(const std::string& name) {
    auto it = properties.find(name);
    if (it != properties.end()) {
        return it->second;
    }
    return nullptr;
}

bool Object::has_property(const std::string& name) const {
    return properties.find(name) != properties.end();
}

void Object::remove_property(const std::string& name) {
    properties.erase(name);
}

std::vector<std::string> Object::get_property_names() const {
    std::vector<std::string> names;
    for (const auto& pair : properties) {
        names.push_back(pair.first);
    }
    return names;
}

} // namespace js
