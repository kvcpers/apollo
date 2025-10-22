#pragma once

#include "types.h"
#include <memory>
#include <string>
#include <vector>
#include <unordered_map>
#include <functional>

namespace js {

// Forward declarations
class Object;
class Function;
class Array;
class String;
class Number;
class Boolean;
class Null;
class Undefined;
class Error;
class Exception;
class Scope;
class Environment;

// JavaScript Value
class Value {
public:
    Value(ValueType type);
    virtual ~Value() = default;

    // Type checking
    ValueType type() const { return type_; }
    bool isObject() const { return type_ == ValueType::Object; }
    bool isFunction() const { return type_ == ValueType::Function; }
    bool isArray() const { return type_ == ValueType::Array; }
    bool isString() const { return type_ == ValueType::String; }
    bool isNumber() const { return type_ == ValueType::Number; }
    bool isBoolean() const { return type_ == ValueType::Boolean; }
    bool isNull() const { return type_ == ValueType::Null; }
    bool isUndefined() const { return type_ == ValueType::Undefined; }
    bool isError() const { return type_ == ValueType::Error; }
    bool isException() const { return type_ == ValueType::Exception; }

    // Type conversion
    virtual std::string toString() const = 0;
    virtual double toNumber() const = 0;
    virtual bool toBoolean() const = 0;
    virtual std::unique_ptr<Object> toObject() const = 0;

    // Comparison
    virtual bool equals(const Value& other) const = 0;
    virtual bool strictEquals(const Value& other) const = 0;
    virtual int compare(const Value& other) const = 0;

    // Operations
    virtual std::unique_ptr<Value> add(const Value& other) const = 0;
    virtual std::unique_ptr<Value> subtract(const Value& other) const = 0;
    virtual std::unique_ptr<Value> multiply(const Value& other) const = 0;
    virtual std::unique_ptr<Value> divide(const Value& other) const = 0;
    virtual std::unique_ptr<Value> modulo(const Value& other) const = 0;
    virtual std::unique_ptr<Value> power(const Value& other) const = 0;

    // Bitwise operations
    virtual std::unique_ptr<Value> bitwiseAnd(const Value& other) const = 0;
    virtual std::unique_ptr<Value> bitwiseOr(const Value& other) const = 0;
    virtual std::unique_ptr<Value> bitwiseXor(const Value& other) const = 0;
    virtual std::unique_ptr<Value> leftShift(const Value& other) const = 0;
    virtual std::unique_ptr<Value> rightShift(const Value& other) const = 0;
    virtual std::unique_ptr<Value> unsignedRightShift(const Value& other) const = 0;

    // Logical operations
    virtual std::unique_ptr<Value> logicalAnd(const Value& other) const = 0;
    virtual std::unique_ptr<Value> logicalOr(const Value& other) const = 0;
    virtual std::unique_ptr<Value> logicalNot() const = 0;

    // Unary operations
    virtual std::unique_ptr<Value> unaryPlus() const = 0;
    virtual std::unique_ptr<Value> unaryMinus() const = 0;
    virtual std::unique_ptr<Value> bitwiseNot() const = 0;
    virtual std::unique_ptr<Value> increment() const = 0;
    virtual std::unique_ptr<Value> decrement() const = 0;

    // Property access
    virtual std::unique_ptr<Value> getProperty(const std::string& name) const = 0;
    virtual void setProperty(const std::string& name, std::unique_ptr<Value> value) = 0;
    virtual bool hasProperty(const std::string& name) const = 0;
    virtual void deleteProperty(const std::string& name) = 0;
    virtual std::vector<std::string> getPropertyNames() const = 0;

    // Array access
    virtual std::unique_ptr<Value> getElement(size_t index) const = 0;
    virtual void setElement(size_t index, std::unique_ptr<Value> value) = 0;
    virtual bool hasElement(size_t index) const = 0;
    virtual void deleteElement(size_t index) = 0;
    virtual size_t getLength() const = 0;

    // Function call
    virtual std::unique_ptr<Value> call(const std::vector<std::unique_ptr<Value>>& arguments) = 0;
    virtual std::unique_ptr<Value> call(std::unique_ptr<Value> thisValue, const std::vector<std::unique_ptr<Value>>& arguments) = 0;
    virtual std::unique_ptr<Value> construct(const std::vector<std::unique_ptr<Value>>& arguments) = 0;

    // Iterator
    virtual std::unique_ptr<Value> iterator() const = 0;
    virtual std::unique_ptr<Value> next() = 0;
    virtual bool isIterable() const = 0;

    // Serialization
    virtual std::string serialize() const = 0;
    virtual std::unique_ptr<Value> deserialize(const std::string& data) = 0;

    // Cloning
    virtual std::unique_ptr<Value> clone() const = 0;
    virtual std::unique_ptr<Value> deepClone() const = 0;

    // Memory management
    virtual void mark() = 0;
    virtual bool isMarked() const = 0;
    virtual void unmark() = 0;

    // Debugging
    virtual std::string debugString() const = 0;
    virtual void dump() const = 0;

protected:
    ValueType type_;
};

// Object Value
class Object : public Value {
public:
    Object();
    virtual ~Object() = default;

    // Property management
    void setProperty(const std::string& name, std::unique_ptr<Value> value) override;
    std::unique_ptr<Value> getProperty(const std::string& name) const override;
    bool hasProperty(const std::string& name) const override;
    void deleteProperty(const std::string& name) override;
    std::vector<std::string> getPropertyNames() const override;

    // Array access
    std::unique_ptr<Value> getElement(size_t index) const override;
    void setElement(size_t index, std::unique_ptr<Value> value) override;
    bool hasElement(size_t index) const override;
    void deleteElement(size_t index) override;
    size_t getLength() const override;

    // Function call
    std::unique_ptr<Value> call(const std::vector<std::unique_ptr<Value>>& arguments) override;
    std::unique_ptr<Value> call(std::unique_ptr<Value> thisValue, const std::vector<std::unique_ptr<Value>>& arguments) override;
    std::unique_ptr<Value> construct(const std::vector<std::unique_ptr<Value>>& arguments) override;

    // Iterator
    std::unique_ptr<Value> iterator() const override;
    std::unique_ptr<Value> next() override;
    bool isIterable() const override;

    // Type conversion
    std::string toString() const override;
    double toNumber() const override;
    bool toBoolean() const override;
    std::unique_ptr<Object> toObject() const override;

    // Comparison
    bool equals(const Value& other) const override;
    bool strictEquals(const Value& other) const override;
    int compare(const Value& other) const override;

    // Operations
    std::unique_ptr<Value> add(const Value& other) const override;
    std::unique_ptr<Value> subtract(const Value& other) const override;
    std::unique_ptr<Value> multiply(const Value& other) const override;
    std::unique_ptr<Value> divide(const Value& other) const override;
    std::unique_ptr<Value> modulo(const Value& other) const override;
    std::unique_ptr<Value> power(const Value& other) const override;

    // Bitwise operations
    std::unique_ptr<Value> bitwiseAnd(const Value& other) const override;
    std::unique_ptr<Value> bitwiseOr(const Value& other) const override;
    std::unique_ptr<Value> bitwiseXor(const Value& other) const override;
    std::unique_ptr<Value> leftShift(const Value& other) const override;
    std::unique_ptr<Value> rightShift(const Value& other) const override;
    std::unique_ptr<Value> unsignedRightShift(const Value& other) const override;

    // Logical operations
    std::unique_ptr<Value> logicalAnd(const Value& other) const override;
    std::unique_ptr<Value> logicalOr(const Value& other) const override;
    std::unique_ptr<Value> logicalNot() const override;

    // Unary operations
    std::unique_ptr<Value> unaryPlus() const override;
    std::unique_ptr<Value> unaryMinus() const override;
    std::unique_ptr<Value> bitwiseNot() const override;
    std::unique_ptr<Value> increment() const override;
    std::unique_ptr<Value> decrement() const override;

    // Serialization
    std::string serialize() const override;
    std::unique_ptr<Value> deserialize(const std::string& data) override;

    // Cloning
    std::unique_ptr<Value> clone() const override;
    std::unique_ptr<Value> deepClone() const override;

    // Memory management
    void mark() override;
    bool isMarked() const override;
    void unmark() override;

    // Debugging
    std::string debugString() const override;
    void dump() const override;

private:
    std::unordered_map<std::string, std::unique_ptr<Value>> properties_;
    std::unordered_map<size_t, std::unique_ptr<Value>> elements_;
    bool marked_;
};

// Function Value
class Function : public Object {
public:
    Function(std::unique_ptr<FunctionDeclaration> declaration);
    virtual ~Function() = default;

    // Function call
    std::unique_ptr<Value> call(const std::vector<std::unique_ptr<Value>>& arguments) override;
    std::unique_ptr<Value> call(std::unique_ptr<Value> thisValue, const std::vector<std::unique_ptr<Value>>& arguments) override;
    std::unique_ptr<Value> construct(const std::vector<std::unique_ptr<Value>>& arguments) override;

    // Function properties
    const std::string& name() const { return name_; }
    size_t arity() const { return arity_; }
    bool isConstructor() const { return isConstructor_; }
    bool isGenerator() const { return isGenerator_; }
    bool isAsync() const { return isAsync_; }

    // Type conversion
    std::string toString() const override;

    // Cloning
    std::unique_ptr<Value> clone() const override;
    std::unique_ptr<Value> deepClone() const override;

private:
    std::string name_;
    size_t arity_;
    bool isConstructor_;
    bool isGenerator_;
    bool isAsync_;
    std::unique_ptr<FunctionDeclaration> declaration_;
};

// Array Value
class Array : public Object {
public:
    Array();
    virtual ~Array() = default;

    // Array operations
    void push(std::unique_ptr<Value> value);
    std::unique_ptr<Value> pop();
    void unshift(std::unique_ptr<Value> value);
    std::unique_ptr<Value> shift();
    void splice(size_t start, size_t deleteCount, const std::vector<std::unique_ptr<Value>>& items);
    std::unique_ptr<Value> slice(size_t start, size_t end);
    void reverse();
    void sort(std::function<int(const Value&, const Value&)> compare = nullptr);

    // Array access
    std::unique_ptr<Value> getElement(size_t index) const override;
    void setElement(size_t index, std::unique_ptr<Value> value) override;
    bool hasElement(size_t index) const override;
    void deleteElement(size_t index) override;
    size_t getLength() const override;

    // Iterator
    std::unique_ptr<Value> iterator() const override;
    std::unique_ptr<Value> next() override;
    bool isIterable() const override { return true; }

    // Type conversion
    std::string toString() const override;

    // Cloning
    std::unique_ptr<Value> clone() const override;
    std::unique_ptr<Value> deepClone() const override;

private:
    std::vector<std::unique_ptr<Value>> elements_;
};

// String Value
class String : public Value {
public:
    String(const std::string& value);
    virtual ~String() = default;

    // String operations
    size_t length() const { return value_.length(); }
    char charAt(size_t index) const;
    std::string substring(size_t start, size_t end) const;
    std::string toUpperCase() const;
    std::string toLowerCase() const;
    std::string trim() const;
    std::vector<std::string> split(const std::string& delimiter) const;
    size_t indexOf(const std::string& substring) const;
    size_t lastIndexOf(const std::string& substring) const;
    bool startsWith(const std::string& prefix) const;
    bool endsWith(const std::string& suffix) const;
    bool includes(const std::string& substring) const;

    // Type conversion
    std::string toString() const override;
    double toNumber() const override;
    bool toBoolean() const override;
    std::unique_ptr<Object> toObject() const override;

    // Comparison
    bool equals(const Value& other) const override;
    bool strictEquals(const Value& other) const override;
    int compare(const Value& other) const override;

    // Operations
    std::unique_ptr<Value> add(const Value& other) const override;
    std::unique_ptr<Value> subtract(const Value& other) const override;
    std::unique_ptr<Value> multiply(const Value& other) const override;
    std::unique_ptr<Value> divide(const Value& other) const override;
    std::unique_ptr<Value> modulo(const Value& other) const override;
    std::unique_ptr<Value> power(const Value& other) const override;

    // Bitwise operations
    std::unique_ptr<Value> bitwiseAnd(const Value& other) const override;
    std::unique_ptr<Value> bitwiseOr(const Value& other) const override;
    std::unique_ptr<Value> bitwiseXor(const Value& other) const override;
    std::unique_ptr<Value> leftShift(const Value& other) const override;
    std::unique_ptr<Value> rightShift(const Value& other) const override;
    std::unique_ptr<Value> unsignedRightShift(const Value& other) const override;

    // Logical operations
    std::unique_ptr<Value> logicalAnd(const Value& other) const override;
    std::unique_ptr<Value> logicalOr(const Value& other) const override;
    std::unique_ptr<Value> logicalNot() const override;

    // Unary operations
    std::unique_ptr<Value> unaryPlus() const override;
    std::unique_ptr<Value> unaryMinus() const override;
    std::unique_ptr<Value> bitwiseNot() const override;
    std::unique_ptr<Value> increment() const override;
    std::unique_ptr<Value> decrement() const override;

    // Property access
    std::unique_ptr<Value> getProperty(const std::string& name) const override;
    void setProperty(const std::string& name, std::unique_ptr<Value> value) override;
    bool hasProperty(const std::string& name) const override;
    void deleteProperty(const std::string& name) override;
    std::vector<std::string> getPropertyNames() const override;

    // Array access
    std::unique_ptr<Value> getElement(size_t index) const override;
    void setElement(size_t index, std::unique_ptr<Value> value) override;
    bool hasElement(size_t index) const override;
    void deleteElement(size_t index) override;
    size_t getLength() const override;

    // Function call
    std::unique_ptr<Value> call(const std::vector<std::unique_ptr<Value>>& arguments) override;
    std::unique_ptr<Value> call(std::unique_ptr<Value> thisValue, const std::vector<std::unique_ptr<Value>>& arguments) override;
    std::unique_ptr<Value> construct(const std::vector<std::unique_ptr<Value>>& arguments) override;

    // Iterator
    std::unique_ptr<Value> iterator() const override;
    std::unique_ptr<Value> next() override;
    bool isIterable() const override { return true; }

    // Serialization
    std::string serialize() const override;
    std::unique_ptr<Value> deserialize(const std::string& data) override;

    // Cloning
    std::unique_ptr<Value> clone() const override;
    std::unique_ptr<Value> deepClone() const override;

    // Memory management
    void mark() override;
    bool isMarked() const override;
    void unmark() override;

    // Debugging
    std::string debugString() const override;
    void dump() const override;

private:
    std::string value_;
    bool marked_;
};

// Number Value
class Number : public Value {
public:
    Number(double value);
    virtual ~Number() = default;

    // Number operations
    double value() const { return value_; }
    bool isInteger() const;
    bool isNaN() const;
    bool isFinite() const;
    bool isInfinite() const;

    // Type conversion
    std::string toString() const override;
    double toNumber() const override;
    bool toBoolean() const override;
    std::unique_ptr<Object> toObject() const override;

    // Comparison
    bool equals(const Value& other) const override;
    bool strictEquals(const Value& other) const override;
    int compare(const Value& other) const override;

    // Operations
    std::unique_ptr<Value> add(const Value& other) const override;
    std::unique_ptr<Value> subtract(const Value& other) const override;
    std::unique_ptr<Value> multiply(const Value& other) const override;
    std::unique_ptr<Value> divide(const Value& other) const override;
    std::unique_ptr<Value> modulo(const Value& other) const override;
    std::unique_ptr<Value> power(const Value& other) const override;

    // Bitwise operations
    std::unique_ptr<Value> bitwiseAnd(const Value& other) const override;
    std::unique_ptr<Value> bitwiseOr(const Value& other) const override;
    std::unique_ptr<Value> bitwiseXor(const Value& other) const override;
    std::unique_ptr<Value> leftShift(const Value& other) const override;
    std::unique_ptr<Value> rightShift(const Value& other) const override;
    std::unique_ptr<Value> unsignedRightShift(const Value& other) const override;

    // Logical operations
    std::unique_ptr<Value> logicalAnd(const Value& other) const override;
    std::unique_ptr<Value> logicalOr(const Value& other) const override;
    std::unique_ptr<Value> logicalNot() const override;

    // Unary operations
    std::unique_ptr<Value> unaryPlus() const override;
    std::unique_ptr<Value> unaryMinus() const override;
    std::unique_ptr<Value> bitwiseNot() const override;
    std::unique_ptr<Value> increment() const override;
    std::unique_ptr<Value> decrement() const override;

    // Property access
    std::unique_ptr<Value> getProperty(const std::string& name) const override;
    void setProperty(const std::string& name, std::unique_ptr<Value> value) override;
    bool hasProperty(const std::string& name) const override;
    void deleteProperty(const std::string& name) override;
    std::vector<std::string> getPropertyNames() const override;

    // Array access
    std::unique_ptr<Value> getElement(size_t index) const override;
    void setElement(size_t index, std::unique_ptr<Value> value) override;
    bool hasElement(size_t index) const override;
    void deleteElement(size_t index) override;
    size_t getLength() const override;

    // Function call
    std::unique_ptr<Value> call(const std::vector<std::unique_ptr<Value>>& arguments) override;
    std::unique_ptr<Value> call(std::unique_ptr<Value> thisValue, const std::vector<std::unique_ptr<Value>>& arguments) override;
    std::unique_ptr<Value> construct(const std::vector<std::unique_ptr<Value>>& arguments) override;

    // Iterator
    std::unique_ptr<Value> iterator() const override;
    std::unique_ptr<Value> next() override;
    bool isIterable() const override { return false; }

    // Serialization
    std::string serialize() const override;
    std::unique_ptr<Value> deserialize(const std::string& data) override;

    // Cloning
    std::unique_ptr<Value> clone() const override;
    std::unique_ptr<Value> deepClone() const override;

    // Memory management
    void mark() override;
    bool isMarked() const override;
    void unmark() override;

    // Debugging
    std::string debugString() const override;
    void dump() const override;

private:
    double value_;
    bool marked_;
};

// Boolean Value
class Boolean : public Value {
public:
    Boolean(bool value);
    virtual ~Boolean() = default;

    // Boolean operations
    bool value() const { return value_; }

    // Type conversion
    std::string toString() const override;
    double toNumber() const override;
    bool toBoolean() const override;
    std::unique_ptr<Object> toObject() const override;

    // Comparison
    bool equals(const Value& other) const override;
    bool strictEquals(const Value& other) const override;
    int compare(const Value& other) const override;

    // Operations
    std::unique_ptr<Value> add(const Value& other) const override;
    std::unique_ptr<Value> subtract(const Value& other) const override;
    std::unique_ptr<Value> multiply(const Value& other) const override;
    std::unique_ptr<Value> divide(const Value& other) const override;
    std::unique_ptr<Value> modulo(const Value& other) const override;
    std::unique_ptr<Value> power(const Value& other) const override;

    // Bitwise operations
    std::unique_ptr<Value> bitwiseAnd(const Value& other) const override;
    std::unique_ptr<Value> bitwiseOr(const Value& other) const override;
    std::unique_ptr<Value> bitwiseXor(const Value& other) const override;
    std::unique_ptr<Value> leftShift(const Value& other) const override;
    std::unique_ptr<Value> rightShift(const Value& other) const override;
    std::unique_ptr<Value> unsignedRightShift(const Value& other) const override;

    // Logical operations
    std::unique_ptr<Value> logicalAnd(const Value& other) const override;
    std::unique_ptr<Value> logicalOr(const Value& other) const override;
    std::unique_ptr<Value> logicalNot() const override;

    // Unary operations
    std::unique_ptr<Value> unaryPlus() const override;
    std::unique_ptr<Value> unaryMinus() const override;
    std::unique_ptr<Value> bitwiseNot() const override;
    std::unique_ptr<Value> increment() const override;
    std::unique_ptr<Value> decrement() const override;

    // Property access
    std::unique_ptr<Value> getProperty(const std::string& name) const override;
    void setProperty(const std::string& name, std::unique_ptr<Value> value) override;
    bool hasProperty(const std::string& name) const override;
    void deleteProperty(const std::string& name) override;
    std::vector<std::string> getPropertyNames() const override;

    // Array access
    std::unique_ptr<Value> getElement(size_t index) const override;
    void setElement(size_t index, std::unique_ptr<Value> value) override;
    bool hasElement(size_t index) const override;
    void deleteElement(size_t index) override;
    size_t getLength() const override;

    // Function call
    std::unique_ptr<Value> call(const std::vector<std::unique_ptr<Value>>& arguments) override;
    std::unique_ptr<Value> call(std::unique_ptr<Value> thisValue, const std::vector<std::unique_ptr<Value>>& arguments) override;
    std::unique_ptr<Value> construct(const std::vector<std::unique_ptr<Value>>& arguments) override;

    // Iterator
    std::unique_ptr<Value> iterator() const override;
    std::unique_ptr<Value> next() override;
    bool isIterable() const override { return false; }

    // Serialization
    std::string serialize() const override;
    std::unique_ptr<Value> deserialize(const std::string& data) override;

    // Cloning
    std::unique_ptr<Value> clone() const override;
    std::unique_ptr<Value> deepClone() const override;

    // Memory management
    void mark() override;
    bool isMarked() const override;
    void unmark() override;

    // Debugging
    std::string debugString() const override;
    void dump() const override;

private:
    bool value_;
    bool marked_;
};

// Null Value
class Null : public Value {
public:
    Null();
    virtual ~Null() = default;

    // Type conversion
    std::string toString() const override;
    double toNumber() const override;
    bool toBoolean() const override;
    std::unique_ptr<Object> toObject() const override;

    // Comparison
    bool equals(const Value& other) const override;
    bool strictEquals(const Value& other) const override;
    int compare(const Value& other) const override;

    // Operations
    std::unique_ptr<Value> add(const Value& other) const override;
    std::unique_ptr<Value> subtract(const Value& other) const override;
    std::unique_ptr<Value> multiply(const Value& other) const override;
    std::unique_ptr<Value> divide(const Value& other) const override;
    std::unique_ptr<Value> modulo(const Value& other) const override;
    std::unique_ptr<Value> power(const Value& other) const override;

    // Bitwise operations
    std::unique_ptr<Value> bitwiseAnd(const Value& other) const override;
    std::unique_ptr<Value> bitwiseOr(const Value& other) const override;
    std::unique_ptr<Value> bitwiseXor(const Value& other) const override;
    std::unique_ptr<Value> leftShift(const Value& other) const override;
    std::unique_ptr<Value> rightShift(const Value& other) const override;
    std::unique_ptr<Value> unsignedRightShift(const Value& other) const override;

    // Logical operations
    std::unique_ptr<Value> logicalAnd(const Value& other) const override;
    std::unique_ptr<Value> logicalOr(const Value& other) const override;
    std::unique_ptr<Value> logicalNot() const override;

    // Unary operations
    std::unique_ptr<Value> unaryPlus() const override;
    std::unique_ptr<Value> unaryMinus() const override;
    std::unique_ptr<Value> bitwiseNot() const override;
    std::unique_ptr<Value> increment() const override;
    std::unique_ptr<Value> decrement() const override;

    // Property access
    std::unique_ptr<Value> getProperty(const std::string& name) const override;
    void setProperty(const std::string& name, std::unique_ptr<Value> value) override;
    bool hasProperty(const std::string& name) const override;
    void deleteProperty(const std::string& name) override;
    std::vector<std::string> getPropertyNames() const override;

    // Array access
    std::unique_ptr<Value> getElement(size_t index) const override;
    void setElement(size_t index, std::unique_ptr<Value> value) override;
    bool hasElement(size_t index) const override;
    void deleteElement(size_t index) override;
    size_t getLength() const override;

    // Function call
    std::unique_ptr<Value> call(const std::vector<std::unique_ptr<Value>>& arguments) override;
    std::unique_ptr<Value> call(std::unique_ptr<Value> thisValue, const std::vector<std::unique_ptr<Value>>& arguments) override;
    std::unique_ptr<Value> construct(const std::vector<std::unique_ptr<Value>>& arguments) override;

    // Iterator
    std::unique_ptr<Value> iterator() const override;
    std::unique_ptr<Value> next() override;
    bool isIterable() const override { return false; }

    // Serialization
    std::string serialize() const override;
    std::unique_ptr<Value> deserialize(const std::string& data) override;

    // Cloning
    std::unique_ptr<Value> clone() const override;
    std::unique_ptr<Value> deepClone() const override;

    // Memory management
    void mark() override;
    bool isMarked() const override;
    void unmark() override;

    // Debugging
    std::string debugString() const override;
    void dump() const override;
};

// Undefined Value
class Undefined : public Value {
public:
    Undefined();
    virtual ~Undefined() = default;

    // Type conversion
    std::string toString() const override;
    double toNumber() const override;
    bool toBoolean() const override;
    std::unique_ptr<Object> toObject() const override;

    // Comparison
    bool equals(const Value& other) const override;
    bool strictEquals(const Value& other) const override;
    int compare(const Value& other) const override;

    // Operations
    std::unique_ptr<Value> add(const Value& other) const override;
    std::unique_ptr<Value> subtract(const Value& other) const override;
    std::unique_ptr<Value> multiply(const Value& other) const override;
    std::unique_ptr<Value> divide(const Value& other) const override;
    std::unique_ptr<Value> modulo(const Value& other) const override;
    std::unique_ptr<Value> power(const Value& other) const override;

    // Bitwise operations
    std::unique_ptr<Value> bitwiseAnd(const Value& other) const override;
    std::unique_ptr<Value> bitwiseOr(const Value& other) const override;
    std::unique_ptr<Value> bitwiseXor(const Value& other) const override;
    std::unique_ptr<Value> leftShift(const Value& other) const override;
    std::unique_ptr<Value> rightShift(const Value& other) const override;
    std::unique_ptr<Value> unsignedRightShift(const Value& other) const override;

    // Logical operations
    std::unique_ptr<Value> logicalAnd(const Value& other) const override;
    std::unique_ptr<Value> logicalOr(const Value& other) const override;
    std::unique_ptr<Value> logicalNot() const override;

    // Unary operations
    std::unique_ptr<Value> unaryPlus() const override;
    std::unique_ptr<Value> unaryMinus() const override;
    std::unique_ptr<Value> bitwiseNot() const override;
    std::unique_ptr<Value> increment() const override;
    std::unique_ptr<Value> decrement() const override;

    // Property access
    std::unique_ptr<Value> getProperty(const std::string& name) const override;
    void setProperty(const std::string& name, std::unique_ptr<Value> value) override;
    bool hasProperty(const std::string& name) const override;
    void deleteProperty(const std::string& name) override;
    std::vector<std::string> getPropertyNames() const override;

    // Array access
    std::unique_ptr<Value> getElement(size_t index) const override;
    void setElement(size_t index, std::unique_ptr<Value> value) override;
    bool hasElement(size_t index) const override;
    void deleteElement(size_t index) override;
    size_t getLength() const override;

    // Function call
    std::unique_ptr<Value> call(const std::vector<std::unique_ptr<Value>>& arguments) override;
    std::unique_ptr<Value> call(std::unique_ptr<Value> thisValue, const std::vector<std::unique_ptr<Value>>& arguments) override;
    std::unique_ptr<Value> construct(const std::vector<std::unique_ptr<Value>>& arguments) override;

    // Iterator
    std::unique_ptr<Value> iterator() const override;
    std::unique_ptr<Value> next() override;
    bool isIterable() const override { return false; }

    // Serialization
    std::string serialize() const override;
    std::unique_ptr<Value> deserialize(const std::string& data) override;

    // Cloning
    std::unique_ptr<Value> clone() const override;
    std::unique_ptr<Value> deepClone() const override;

    // Memory management
    void mark() override;
    bool isMarked() const override;
    void unmark() override;

    // Debugging
    std::string debugString() const override;
    void dump() const override;
};

// Error Value
class Error : public Object {
public:
    Error(const std::string& message);
    virtual ~Error() = default;

    // Error properties
    const std::string& message() const { return message_; }
    const std::string& name() const { return name_; }
    const std::string& stack() const { return stack_; }

    // Type conversion
    std::string toString() const override;

    // Cloning
    std::unique_ptr<Value> clone() const override;
    std::unique_ptr<Value> deepClone() const override;

private:
    std::string name_;
    std::string message_;
    std::string stack_;
};

// Exception Value
class Exception : public Error {
public:
    Exception(const std::string& message);
    virtual ~Exception() = default;

    // Exception properties
    const std::string& type() const { return type_; }
    const std::vector<std::string>& trace() const { return trace_; }

    // Type conversion
    std::string toString() const override;

    // Cloning
    std::unique_ptr<Value> clone() const override;
    std::unique_ptr<Value> deepClone() const override;

private:
    std::string type_;
    std::vector<std::string> trace_;
};

} // namespace js
