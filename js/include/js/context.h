#pragma once

#include "value.h"
#include "scope.h"
#include "environment.h"
#include <memory>
#include <string>
#include <vector>
#include <unordered_map>
#include <stack>

namespace js {

// Forward declarations
class Value;
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
class Interpreter;

// JavaScript Execution Context
class Context {
public:
    Context();
    virtual ~Context() = default;

    // Context lifecycle
    void initialize();
    void shutdown();
    bool isInitialized() const { return initialized_; }

    // Global object
    void setGlobalObject(std::unique_ptr<Object> global);
    Object* getGlobalObject() const { return globalObject_.get(); }

    // Variable environment
    void setVariableEnvironment(std::unique_ptr<Environment> environment);
    Environment* getVariableEnvironment() const { return variableEnvironment_.get(); }

    // Lexical environment
    void setLexicalEnvironment(std::unique_ptr<Environment> environment);
    Environment* getLexicalEnvironment() const { return lexicalEnvironment_.get(); }

    // This binding
    void setThisBinding(std::unique_ptr<Value> thisValue);
    Value* getThisBinding() const { return thisBinding_.get(); }

    // Scope chain
    void setScopeChain(std::vector<std::unique_ptr<Scope>> chain);
    const std::vector<std::unique_ptr<Scope>>& getScopeChain() const { return scopeChain_; }

    // Execution context
    void setExecutionContext(std::unique_ptr<Context> context);
    Context* getExecutionContext() const { return executionContext_.get(); }

    // Function context
    void setFunctionContext(std::unique_ptr<Function> function);
    Function* getFunctionContext() const { return functionContext_.get(); }

    // Module context
    void setModuleContext(std::unique_ptr<Module> module);
    Module* getModuleContext() const { return moduleContext_.get(); }

    // Class context
    void setClassContext(std::unique_ptr<Class> classObj);
    Class* getClassContext() const { return classContext_.get(); }

    // Generator context
    void setGeneratorContext(std::unique_ptr<Generator> generator);
    Generator* getGeneratorContext() const { return generatorContext_.get(); }

    // Async context
    void setAsyncContext(std::unique_ptr<Async> async);
    Async* getAsyncContext() const { return asyncContext_.get(); }

    // Promise context
    void setPromiseContext(std::unique_ptr<Promise> promise);
    Promise* getPromiseContext() const { return promiseContext_.get(); }

    // Error context
    void setErrorContext(std::unique_ptr<Error> error);
    Error* getErrorContext() const { return errorContext_.get(); }

    // Exception context
    void setExceptionContext(std::unique_ptr<Exception> exception);
    Exception* getExceptionContext() const { return exceptionContext_.get(); }

    // Variable resolution
    std::unique_ptr<Value> resolveVariable(const std::string& name);
    void declareVariable(const std::string& name, std::unique_ptr<Value> value);
    void assignVariable(const std::string& name, std::unique_ptr<Value> value);
    bool hasVariable(const std::string& name) const;
    void deleteVariable(const std::string& name);

    // Property access
    std::unique_ptr<Value> getProperty(const std::string& name);
    void setProperty(const std::string& name, std::unique_ptr<Value> value);
    bool hasProperty(const std::string& name) const;
    void deleteProperty(const std::string& name);

    // Function call
    std::unique_ptr<Value> callFunction(const std::string& name, const std::vector<std::unique_ptr<Value>>& arguments);
    std::unique_ptr<Value> callFunction(std::unique_ptr<Function> function, const std::vector<std::unique_ptr<Value>>& arguments);
    std::unique_ptr<Value> callFunction(std::unique_ptr<Function> function, std::unique_ptr<Value> thisValue, const std::vector<std::unique_ptr<Value>>& arguments);

    // Constructor call
    std::unique_ptr<Value> constructFunction(const std::string& name, const std::vector<std::unique_ptr<Value>>& arguments);
    std::unique_ptr<Value> constructFunction(std::unique_ptr<Function> function, const std::vector<std::unique_ptr<Value>>& arguments);

    // Method call
    std::unique_ptr<Value> callMethod(std::unique_ptr<Value> object, const std::string& name, const std::vector<std::unique_ptr<Value>>& arguments);
    std::unique_ptr<Value> callMethod(std::unique_ptr<Value> object, const std::string& name, std::unique_ptr<Value> thisValue, const std::vector<std::unique_ptr<Value>>& arguments);

    // Property descriptor
    void defineProperty(const std::string& name, std::unique_ptr<PropertyDescriptor> descriptor);
    std::unique_ptr<PropertyDescriptor> getPropertyDescriptor(const std::string& name);
    bool hasPropertyDescriptor(const std::string& name) const;
    void deletePropertyDescriptor(const std::string& name);

    // Object creation
    std::unique_ptr<Object> createObject();
    std::unique_ptr<Object> createObject(std::unique_ptr<Object> prototype);
    std::unique_ptr<Object> createObject(std::unique_ptr<Object> prototype, std::unique_ptr<PropertyDescriptorMap> descriptors);

    // Function creation
    std::unique_ptr<Function> createFunction(const std::string& name, const std::vector<std::string>& parameters, std::unique_ptr<BlockStatement> body);
    std::unique_ptr<Function> createFunction(const std::string& name, const std::vector<std::string>& parameters, std::unique_ptr<BlockStatement> body, std::unique_ptr<Object> prototype);

    // Array creation
    std::unique_ptr<Array> createArray();
    std::unique_ptr<Array> createArray(size_t length);
    std::unique_ptr<Array> createArray(const std::vector<std::unique_ptr<Value>>& elements);

    // String creation
    std::unique_ptr<String> createString(const std::string& value);
    std::unique_ptr<String> createString(const char* value);
    std::unique_ptr<String> createString(char value);

    // Number creation
    std::unique_ptr<Number> createNumber(double value);
    std::unique_ptr<Number> createNumber(int value);
    std::unique_ptr<Number> createNumber(long value);
    std::unique_ptr<Number> createNumber(float value);

    // Boolean creation
    std::unique_ptr<Boolean> createBoolean(bool value);
    std::unique_ptr<Boolean> createBoolean(int value);
    std::unique_ptr<Boolean> createBoolean(double value);

    // Null creation
    std::unique_ptr<Null> createNull();

    // Undefined creation
    std::unique_ptr<Undefined> createUndefined();

    // Error creation
    std::unique_ptr<Error> createError(const std::string& message);
    std::unique_ptr<Error> createError(const std::string& name, const std::string& message);
    std::unique_ptr<Error> createError(const std::string& name, const std::string& message, const std::string& stack);

    // Exception creation
    std::unique_ptr<Exception> createException(const std::string& message);
    std::unique_ptr<Exception> createException(const std::string& type, const std::string& message);
    std::unique_ptr<Exception> createException(const std::string& type, const std::string& message, const std::vector<std::string>& trace);

    // Type conversion
    std::unique_ptr<Value> toValue(const std::string& value);
    std::unique_ptr<Value> toValue(double value);
    std::unique_ptr<Value> toValue(bool value);
    std::unique_ptr<Value> toValue(std::nullptr_t);
    std::unique_ptr<Value> toValue();

    // Type checking
    bool isObject(std::unique_ptr<Value> value) const;
    bool isFunction(std::unique_ptr<Value> value) const;
    bool isArray(std::unique_ptr<Value> value) const;
    bool isString(std::unique_ptr<Value> value) const;
    bool isNumber(std::unique_ptr<Value> value) const;
    bool isBoolean(std::unique_ptr<Value> value) const;
    bool isNull(std::unique_ptr<Value> value) const;
    bool isUndefined(std::unique_ptr<Value> value) const;
    bool isError(std::unique_ptr<Value> value) const;
    bool isException(std::unique_ptr<Value> value) const;

    // Type coercion
    std::string toString(std::unique_ptr<Value> value);
    double toNumber(std::unique_ptr<Value> value);
    bool toBoolean(std::unique_ptr<Value> value);
    std::unique_ptr<Object> toObject(std::unique_ptr<Value> value);

    // Comparison
    bool equals(std::unique_ptr<Value> left, std::unique_ptr<Value> right);
    bool strictEquals(std::unique_ptr<Value> left, std::unique_ptr<Value> right);
    int compare(std::unique_ptr<Value> left, std::unique_ptr<Value> right);

    // Operations
    std::unique_ptr<Value> add(std::unique_ptr<Value> left, std::unique_ptr<Value> right);
    std::unique_ptr<Value> subtract(std::unique_ptr<Value> left, std::unique_ptr<Value> right);
    std::unique_ptr<Value> multiply(std::unique_ptr<Value> left, std::unique_ptr<Value> right);
    std::unique_ptr<Value> divide(std::unique_ptr<Value> left, std::unique_ptr<Value> right);
    std::unique_ptr<Value> modulo(std::unique_ptr<Value> left, std::unique_ptr<Value> right);
    std::unique_ptr<Value> power(std::unique_ptr<Value> left, std::unique_ptr<Value> right);

    // Bitwise operations
    std::unique_ptr<Value> bitwiseAnd(std::unique_ptr<Value> left, std::unique_ptr<Value> right);
    std::unique_ptr<Value> bitwiseOr(std::unique_ptr<Value> left, std::unique_ptr<Value> right);
    std::unique_ptr<Value> bitwiseXor(std::unique_ptr<Value> left, std::unique_ptr<Value> right);
    std::unique_ptr<Value> leftShift(std::unique_ptr<Value> left, std::unique_ptr<Value> right);
    std::unique_ptr<Value> rightShift(std::unique_ptr<Value> left, std::unique_ptr<Value> right);
    std::unique_ptr<Value> unsignedRightShift(std::unique_ptr<Value> left, std::unique_ptr<Value> right);

    // Logical operations
    std::unique_ptr<Value> logicalAnd(std::unique_ptr<Value> left, std::unique_ptr<Value> right);
    std::unique_ptr<Value> logicalOr(std::unique_ptr<Value> left, std::unique_ptr<Value> right);
    std::unique_ptr<Value> logicalNot(std::unique_ptr<Value> operand);

    // Unary operations
    std::unique_ptr<Value> unaryPlus(std::unique_ptr<Value> operand);
    std::unique_ptr<Value> unaryMinus(std::unique_ptr<Value> operand);
    std::unique_ptr<Value> bitwiseNot(std::unique_ptr<Value> operand);
    std::unique_ptr<Value> increment(std::unique_ptr<Value> operand);
    std::unique_ptr<Value> decrement(std::unique_ptr<Value> operand);

    // Assignment operations
    std::unique_ptr<Value> assign(std::unique_ptr<Value> left, std::unique_ptr<Value> right);
    std::unique_ptr<Value> addAssign(std::unique_ptr<Value> left, std::unique_ptr<Value> right);
    std::unique_ptr<Value> subtractAssign(std::unique_ptr<Value> left, std::unique_ptr<Value> right);
    std::unique_ptr<Value> multiplyAssign(std::unique_ptr<Value> left, std::unique_ptr<Value> right);
    std::unique_ptr<Value> divideAssign(std::unique_ptr<Value> left, std::unique_ptr<Value> right);
    std::unique_ptr<Value> moduloAssign(std::unique_ptr<Value> left, std::unique_ptr<Value> right);
    std::unique_ptr<Value> powerAssign(std::unique_ptr<Value> left, std::unique_ptr<Value> right);
    std::unique_ptr<Value> bitwiseAndAssign(std::unique_ptr<Value> left, std::unique_ptr<Value> right);
    std::unique_ptr<Value> bitwiseOrAssign(std::unique_ptr<Value> left, std::unique_ptr<Value> right);
    std::unique_ptr<Value> bitwiseXorAssign(std::unique_ptr<Value> left, std::unique_ptr<Value> right);
    std::unique_ptr<Value> leftShiftAssign(std::unique_ptr<Value> left, std::unique_ptr<Value> right);
    std::unique_ptr<Value> rightShiftAssign(std::unique_ptr<Value> left, std::unique_ptr<Value> right);
    std::unique_ptr<Value> unsignedRightShiftAssign(std::unique_ptr<Value> left, std::unique_ptr<Value> right);

    // Property access
    std::unique_ptr<Value> getProperty(std::unique_ptr<Value> object, const std::string& name);
    void setProperty(std::unique_ptr<Value> object, const std::string& name, std::unique_ptr<Value> value);
    bool hasProperty(std::unique_ptr<Value> object, const std::string& name);
    void deleteProperty(std::unique_ptr<Value> object, const std::string& name);
    std::vector<std::string> getPropertyNames(std::unique_ptr<Value> object);

    // Array access
    std::unique_ptr<Value> getElement(std::unique_ptr<Value> array, size_t index);
    void setElement(std::unique_ptr<Value> array, size_t index, std::unique_ptr<Value> value);
    bool hasElement(std::unique_ptr<Value> array, size_t index);
    void deleteElement(std::unique_ptr<Value> array, size_t index);
    size_t getLength(std::unique_ptr<Value> array);

    // Iterator
    std::unique_ptr<Value> iterator(std::unique_ptr<Value> iterable);
    std::unique_ptr<Value> next(std::unique_ptr<Value> iterator);
    bool isIterable(std::unique_ptr<Value> value);

    // Serialization
    std::string serialize(std::unique_ptr<Value> value);
    std::unique_ptr<Value> deserialize(const std::string& data);

    // Cloning
    std::unique_ptr<Value> clone(std::unique_ptr<Value> value);
    std::unique_ptr<Value> deepClone(std::unique_ptr<Value> value);

    // Memory management
    void mark(std::unique_ptr<Value> value);
    bool isMarked(std::unique_ptr<Value> value);
    void unmark(std::unique_ptr<Value> value);

    // Debugging
    std::string debugString(std::unique_ptr<Value> value);
    void dump(std::unique_ptr<Value> value);

    // Context management
    void pushContext(std::unique_ptr<Context> context);
    void popContext();
    Context* getParentContext() const { return parentContext_; }
    void setParentContext(Context* context) { parentContext_ = context; }

    // Scope management
    void pushScope(std::unique_ptr<Scope> scope);
    void popScope();
    Scope* getCurrentScope() const;
    const std::vector<std::unique_ptr<Scope>>& getScopeStack() const { return scopeStack_; }

    // Environment management
    void pushEnvironment(std::unique_ptr<Environment> environment);
    void popEnvironment();
    Environment* getCurrentEnvironment() const;
    const std::vector<std::unique_ptr<Environment>>& getEnvironmentStack() const { return environmentStack_; }

    // Error handling
    void setErrorHandler(std::function<void(const Exception&)> handler);
    void handleError(const Exception& error);
    void handleUncaughtError(const Exception& error);

    // Performance
    void enableProfiling();
    void disableProfiling();
    bool isProfilingEnabled() const { return profilingEnabled_; }

    // Statistics
    size_t getExecutionCount() const { return executionCount_; }
    size_t getErrorCount() const { return errorCount_; }
    double getAverageExecutionTime() const;
    double getTotalExecutionTime() const;
    void resetStatistics();

private:
    bool initialized_;
    bool profilingEnabled_;

    // Core components
    std::unique_ptr<Object> globalObject_;
    std::unique_ptr<Environment> variableEnvironment_;
    std::unique_ptr<Environment> lexicalEnvironment_;
    std::unique_ptr<Value> thisBinding_;
    std::vector<std::unique_ptr<Scope>> scopeChain_;

    // Context hierarchy
    std::unique_ptr<Context> executionContext_;
    std::unique_ptr<Function> functionContext_;
    std::unique_ptr<Module> moduleContext_;
    std::unique_ptr<Class> classContext_;
    std::unique_ptr<Generator> generatorContext_;
    std::unique_ptr<Async> asyncContext_;
    std::unique_ptr<Promise> promiseContext_;
    std::unique_ptr<Error> errorContext_;
    std::unique_ptr<Exception> exceptionContext_;

    // Context stack
    std::stack<std::unique_ptr<Context>> contextStack_;
    std::stack<std::unique_ptr<Scope>> scopeStack_;
    std::stack<std::unique_ptr<Environment>> environmentStack_;

    // Parent context
    Context* parentContext_;

    // Error handling
    std::function<void(const Exception&)> errorHandler_;

    // Statistics
    size_t executionCount_;
    size_t errorCount_;
    double totalExecutionTime_;

    // Helper methods
    void setupDefaultErrorHandler();
    void collectStatistics();
    void resetStatistics();
};

} // namespace js
