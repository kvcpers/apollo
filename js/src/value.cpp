#include "js/value.h"

namespace js {

Value::Value(ValueType type) : type_(type) {}

Value::~Value() {}

ValueType Value::get_type() const {
    return type_;
}

bool Value::is_undefined() const {
    return type_ == ValueType::Undefined;
}

bool Value::is_null() const {
    return type_ == ValueType::Null;
}

bool Value::is_boolean() const {
    return type_ == ValueType::Boolean;
}

bool Value::is_number() const {
    return type_ == ValueType::Number;
}

bool Value::is_string() const {
    return type_ == ValueType::String;
}

bool Value::is_object() const {
    return type_ == ValueType::Object;
}

bool Value::is_function() const {
    return type_ == ValueType::Function;
}

// String implementation
String::String(const std::string& value) : Value(ValueType::String), value_(value) {}

String::~String() {}

std::string String::get_value() const {
    return value_;
}

void String::set_value(const std::string& value) {
    value_ = value;
}

// Number implementation
Number::Number(double value) : Value(ValueType::Number), value_(value) {}

Number::~Number() {}

double Number::get_value() const {
    return value_;
}

void Number::set_value(double value) {
    value_ = value;
}

// Boolean implementation
Boolean::Boolean(bool value) : Value(ValueType::Boolean), value_(value) {}

Boolean::~Boolean() {}

bool Boolean::get_value() const {
    return value_;
}

void Boolean::set_value(bool value) {
    value_ = value;
}

} // namespace js
