#pragma once

#include "ast.h"
#include "value.h"
#include "context.h"
#include "interpreter.h"
#include <memory>
#include <string>
#include <vector>
#include <unordered_map>

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
class GC;
class DOMBindings;
class Console;
class EventLoop;
class Promise;
class Async;
class Module;
class Loader;
class Compiler;
class Optimizer;
class Debugger;
class Profiler;

// JavaScript Engine
class JavaScriptEngine {
public:
    JavaScriptEngine();
    ~JavaScriptEngine();

    // Engine lifecycle
    void initialize();
    void shutdown();
    bool isInitialized() const { return initialized_; }

    // Execution
    std::unique_ptr<Value> execute(const std::string& source);
    std::unique_ptr<Value> execute(std::unique_ptr<AST> ast);
    std::unique_ptr<Value> execute(std::unique_ptr<Program> program);
    std::unique_ptr<Value> execute(std::unique_ptr<Module> module);

    // Context management
    std::unique_ptr<Context> createContext();
    void setGlobalContext(std::unique_ptr<Context> context);
    Context* getGlobalContext() const { return globalContext_.get(); }

    // Value creation
    std::unique_ptr<Value> createValue();
    std::unique_ptr<Object> createObject();
    std::unique_ptr<Function> createFunction();
    std::unique_ptr<Array> createArray();
    std::unique_ptr<String> createString(const std::string& value);
    std::unique_ptr<Number> createNumber(double value);
    std::unique_ptr<Boolean> createBoolean(bool value);
    std::unique_ptr<Null> createNull();
    std::unique_ptr<Undefined> createUndefined();
    std::unique_ptr<Error> createError(const std::string& message);

    // Built-in objects
    void initializeBuiltins();
    void initializeGlobalObject();
    void initializeConsole();
    void initializeDOM();
    void initializeMath();
    void initializeDate();
    void initializeJSON();
    void initializePromise();
    void initializeAsync();

    // Module system
    std::unique_ptr<Module> loadModule(const std::string& specifier);
    std::unique_ptr<Module> loadModule(const std::string& specifier, const std::string& referrer);
    void registerModule(const std::string& specifier, std::unique_ptr<Module> module);

    // Error handling
    void setErrorHandler(std::function<void(const Exception&)> handler);
    void handleError(const Exception& error);
    void handleUncaughtError(const Exception& error);

    // Performance
    void enableProfiling();
    void disableProfiling();
    void enableOptimization();
    void disableOptimization();
    void enableDebugging();
    void disableDebugging();

    // Memory management
    void enableGC();
    void disableGC();
    void runGC();
    size_t getHeapSize() const;
    size_t getHeapUsed() const;

    // Configuration
    void setStrictMode(bool strict);
    bool isStrictMode() const { return strictMode_; }
    void setModuleMode(bool module);
    bool isModuleMode() const { return moduleMode_; }
    void setAsyncMode(bool async);
    bool isAsyncMode() const { return asyncMode_; }

    // Statistics
    size_t getExecutionCount() const { return executionCount_; }
    size_t getErrorCount() const { return errorCount_; }
    double getAverageExecutionTime() const;
    double getTotalExecutionTime() const;

private:
    bool initialized_;
    bool strictMode_;
    bool moduleMode_;
    bool asyncMode_;
    bool profilingEnabled_;
    bool optimizationEnabled_;
    bool debuggingEnabled_;
    bool gcEnabled_;

    // Core components
    std::unique_ptr<Context> globalContext_;
    std::unique_ptr<Interpreter> interpreter_;
    std::unique_ptr<GC> gc_;
    std::unique_ptr<DOMBindings> domBindings_;
    std::unique_ptr<Console> console_;
    std::unique_ptr<EventLoop> eventLoop_;
    std::unique_ptr<Promise> promise_;
    std::unique_ptr<Async> async_;
    std::unique_ptr<Module> module_;
    std::unique_ptr<Loader> loader_;
    std::unique_ptr<Compiler> compiler_;
    std::unique_ptr<Optimizer> optimizer_;
    std::unique_ptr<Debugger> debugger_;
    std::unique_ptr<Profiler> profiler_;

    // Statistics
    size_t executionCount_;
    size_t errorCount_;
    double totalExecutionTime_;

    // Error handling
    std::function<void(const Exception&)> errorHandler_;

    // Helper methods
    void setupDefaultErrorHandler();
    void collectStatistics();
    void resetStatistics();
};

} // namespace js
