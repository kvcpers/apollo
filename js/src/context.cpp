#include "js/context.h"

namespace js {

Context::Context() {}

Context::~Context() {}

void Context::set_variable(const std::string& name, std::shared_ptr<Value> value) {
    variables[name] = value;
}

std::shared_ptr<Value> Context::get_variable(const std::string& name) {
    auto it = variables.find(name);
    if (it != variables.end()) {
        return it->second;
    }
    return nullptr;
}

bool Context::has_variable(const std::string& name) const {
    return variables.find(name) != variables.end();
}

void Context::remove_variable(const std::string& name) {
    variables.erase(name);
}

void Context::clear() {
    variables.clear();
}

} // namespace js
