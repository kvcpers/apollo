#include "js/engine.h"
#include "js/interpreter.h"
#include "js/context.h"
#include "js/value.h"
#include "js/object.h"
#include "js/function.h"
#include "js/array.h"
#include "js/string.h"
#include "js/number.h"
#include "js/boolean.h"
#include "js/null.h"
#include "js/undefined.h"
#include "js/error.h"
#include "js/exception.h"
#include "js/scope.h"
#include "js/environment.h"
#include "js/gc.h"
#include "js/dom_bindings.h"
#include "js/console.h"
#include "js/event_loop.h"
#include "js/promise.h"
#include "js/async.h"
#include "js/module.h"
#include "js/loader.h"
#include "js/compiler.h"
#include "js/optimizer.h"
#include "js/debugger.h"
#include "js/profiler.h"
#include <iostream>
#include <chrono>

namespace js {

// JavaScript Engine implementation
JavaScriptEngine::JavaScriptEngine() 
    : initialized_(false)
    , strictMode_(false)
    , moduleMode_(false)
    , asyncMode_(false)
    , profilingEnabled_(false)
    , optimizationEnabled_(false)
    , debuggingEnabled_(false)
    , gcEnabled_(true)
    , executionCount_(0)
    , errorCount_(0)
    , totalExecutionTime_(0.0)
{
    initialize();
}

JavaScriptEngine::~JavaScriptEngine() {
    shutdown();
}

void JavaScriptEngine::initialize() {
    if (initialized_) {
        return;
    }

    // Initialize core components
    interpreter_ = std::make_unique<Interpreter>();
    gc_ = std::make_unique<GC>();
    domBindings_ = std::make_unique<DOMBindings>();
    console_ = std::make_unique<Console>();
    eventLoop_ = std::make_unique<EventLoop>();
    promise_ = std::make_unique<Promise>();
    async_ = std::make_unique<Async>();
    module_ = std::make_unique<Module>();
    loader_ = std::make_unique<Loader>();
    compiler_ = std::make_unique<Compiler>();
    optimizer_ = std::make_unique<Optimizer>();
    debugger_ = std::make_unique<Debugger>();
    profiler_ = std::make_unique<Profiler>();

    // Create global context
    globalContext_ = std::make_unique<Context>();
    globalContext_->initialize();

    // Initialize built-in objects
    initializeBuiltins();

    initialized_ = true;
}

void JavaScriptEngine::shutdown() {
    if (!initialized_) {
        return;
    }

    // Clear global context
    globalContext_.reset();

    // Clear core components
    interpreter_.reset();
    gc_.reset();
    domBindings_.reset();
    console_.reset();
    eventLoop_.reset();
    promise_.reset();
    async_.reset();
    module_.reset();
    loader_.reset();
    compiler_.reset();
    optimizer_.reset();
    debugger_.reset();
    profiler_.reset();

    initialized_ = false;
}

std::unique_ptr<Value> JavaScriptEngine::execute(const std::string& source) {
    if (!initialized_) {
        return nullptr;
    }

    auto start = std::chrono::high_resolution_clock::now();

    try {
        // Parse the source code
        Parser parser(source);
        auto ast = parser.parse();
        
        // Execute the AST
        auto result = execute(std::move(ast));
        
        executionCount_++;
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        totalExecutionTime_ += duration.count() / 1000000.0;
        
        return result;
    } catch (const std::exception& e) {
        errorCount_++;
        std::cerr << "JavaScript execution error: " << e.what() << std::endl;
        return nullptr;
    }
}

std::unique_ptr<Value> JavaScriptEngine::execute(std::unique_ptr<AST> ast) {
    if (!initialized_ || !ast) {
        return nullptr;
    }

    auto start = std::chrono::high_resolution_clock::now();

    try {
        // Execute the AST
        auto result = interpreter_->execute(std::move(ast), globalContext_.get());
        
        executionCount_++;
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        totalExecutionTime_ += duration.count() / 1000000.0;
        
        return result;
    } catch (const std::exception& e) {
        errorCount_++;
        std::cerr << "JavaScript execution error: " << e.what() << std::endl;
        return nullptr;
    }
}

std::unique_ptr<Value> JavaScriptEngine::execute(std::unique_ptr<Program> program) {
    if (!initialized_ || !program) {
        return nullptr;
    }

    auto start = std::chrono::high_resolution_clock::now();

    try {
        // Execute the program
        auto result = interpreter_->execute(std::move(program), globalContext_.get());
        
        executionCount_++;
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        totalExecutionTime_ += duration.count() / 1000000.0;
        
        return result;
    } catch (const std::exception& e) {
        errorCount_++;
        std::cerr << "JavaScript execution error: " << e.what() << std::endl;
        return nullptr;
    }
}

std::unique_ptr<Value> JavaScriptEngine::execute(std::unique_ptr<Module> module) {
    if (!initialized_ || !module) {
        return nullptr;
    }

    auto start = std::chrono::high_resolution_clock::now();

    try {
        // Execute the module
        auto result = interpreter_->execute(std::move(module), globalContext_.get());
        
        executionCount_++;
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        totalExecutionTime_ += duration.count() / 1000000.0;
        
        return result;
    } catch (const std::exception& e) {
        errorCount_++;
        std::cerr << "JavaScript execution error: " << e.what() << std::endl;
        return nullptr;
    }
}

std::unique_ptr<Context> JavaScriptEngine::createContext() {
    if (!initialized_) {
        return nullptr;
    }

    auto context = std::make_unique<Context>();
    context->initialize();
    return context;
}

void JavaScriptEngine::setGlobalContext(std::unique_ptr<Context> context) {
    globalContext_ = std::move(context);
}

std::unique_ptr<Value> JavaScriptEngine::createValue() {
    return std::make_unique<Value>(ValueType::Undefined);
}

std::unique_ptr<Object> JavaScriptEngine::createObject() {
    return std::make_unique<Object>();
}

std::unique_ptr<Function> JavaScriptEngine::createFunction() {
    return std::make_unique<Function>();
}

std::unique_ptr<Array> JavaScriptEngine::createArray() {
    return std::make_unique<Array>();
}

std::unique_ptr<String> JavaScriptEngine::createString(const std::string& value) {
    return std::make_unique<String>(value);
}

std::unique_ptr<Number> JavaScriptEngine::createNumber(double value) {
    return std::make_unique<Number>(value);
}

std::unique_ptr<Boolean> JavaScriptEngine::createBoolean(bool value) {
    return std::make_unique<Boolean>(value);
}

std::unique_ptr<Null> JavaScriptEngine::createNull() {
    return std::make_unique<Null>();
}

std::unique_ptr<Undefined> JavaScriptEngine::createUndefined() {
    return std::make_unique<Undefined>();
}

std::unique_ptr<Error> JavaScriptEngine::createError(const std::string& message) {
    return std::make_unique<Error>(message);
}

void JavaScriptEngine::initializeBuiltins() {
    if (!initialized_) {
        return;
    }

    initializeGlobalObject();
    initializeConsole();
    initializeDOM();
    initializeMath();
    initializeDate();
    initializeJSON();
    initializePromise();
    initializeAsync();
}

void JavaScriptEngine::initializeGlobalObject() {
    if (!globalContext_) {
        return;
    }

    auto globalObject = globalContext_->getGlobalObject();
    if (!globalObject) {
        globalObject = std::make_unique<Object>();
        globalContext_->setGlobalObject(std::move(globalObject));
    }
}

void JavaScriptEngine::initializeConsole() {
    if (!globalContext_) {
        return;
    }

    auto globalObject = globalContext_->getGlobalObject();
    if (globalObject) {
        auto consoleObject = console_->createConsoleObject();
        globalObject->setProperty("console", std::move(consoleObject));
    }
}

void JavaScriptEngine::initializeDOM() {
    if (!globalContext_) {
        return;
    }

    auto globalObject = globalContext_->getGlobalObject();
    if (globalObject) {
        auto domObject = domBindings_->createDOMObject();
        globalObject->setProperty("document", std::move(domObject));
    }
}

void JavaScriptEngine::initializeMath() {
    if (!globalContext_) {
        return;
    }

    auto globalObject = globalContext_->getGlobalObject();
    if (globalObject) {
        auto mathObject = std::make_unique<Object>();
        
        // Math constants
        mathObject->setProperty("PI", createNumber(3.141592653589793));
        mathObject->setProperty("E", createNumber(2.718281828459045));
        mathObject->setProperty("LN2", createNumber(0.6931471805599453));
        mathObject->setProperty("LN10", createNumber(2.302585092994046));
        mathObject->setProperty("LOG2E", createNumber(1.4426950408889634));
        mathObject->setProperty("LOG10E", createNumber(0.4342944819032518));
        mathObject->setProperty("SQRT1_2", createNumber(0.7071067811865476));
        mathObject->setProperty("SQRT2", createNumber(1.4142135623730951));
        
        // Math methods
        // Note: In a real implementation, these would be proper function objects
        // For now, we'll just set them as placeholder values
        
        globalObject->setProperty("Math", std::move(mathObject));
    }
}

void JavaScriptEngine::initializeDate() {
    if (!globalContext_) {
        return;
    }

    auto globalObject = globalContext_->getGlobalObject();
    if (globalObject) {
        auto dateObject = std::make_unique<Object>();
        
        // Date constructor
        // Note: In a real implementation, this would be a proper constructor function
        
        globalObject->setProperty("Date", std::move(dateObject));
    }
}

void JavaScriptEngine::initializeJSON() {
    if (!globalContext_) {
        return;
    }

    auto globalObject = globalContext_->getGlobalObject();
    if (globalObject) {
        auto jsonObject = std::make_unique<Object>();
        
        // JSON methods
        // Note: In a real implementation, these would be proper function objects
        // For now, we'll just set them as placeholder values
        
        globalObject->setProperty("JSON", std::move(jsonObject));
    }
}

void JavaScriptEngine::initializePromise() {
    if (!globalContext_) {
        return;
    }

    auto globalObject = globalContext_->getGlobalObject();
    if (globalObject) {
        auto promiseObject = promise_->createPromiseObject();
        globalObject->setProperty("Promise", std::move(promiseObject));
    }
}

void JavaScriptEngine::initializeAsync() {
    if (!globalContext_) {
        return;
    }

    auto globalObject = globalContext_->getGlobalObject();
    if (globalObject) {
        auto asyncObject = async_->createAsyncObject();
        globalObject->setProperty("async", std::move(asyncObject));
    }
}

std::unique_ptr<Module> JavaScriptEngine::loadModule(const std::string& specifier) {
    if (!initialized_) {
        return nullptr;
    }

    return loader_->loadModule(specifier);
}

std::unique_ptr<Module> JavaScriptEngine::loadModule(const std::string& specifier, const std::string& referrer) {
    if (!initialized_) {
        return nullptr;
    }

    return loader_->loadModule(specifier, referrer);
}

void JavaScriptEngine::registerModule(const std::string& specifier, std::unique_ptr<Module> module) {
    if (!initialized_) {
        return;
    }

    loader_->registerModule(specifier, std::move(module));
}

void JavaScriptEngine::setErrorHandler(std::function<void(const Exception&)> handler) {
    if (globalContext_) {
        globalContext_->setErrorHandler(handler);
    }
}

void JavaScriptEngine::handleError(const Exception& error) {
    if (globalContext_) {
        globalContext_->handleError(error);
    }
}

void JavaScriptEngine::handleUncaughtError(const Exception& error) {
    if (globalContext_) {
        globalContext_->handleUncaughtError(error);
    }
}

void JavaScriptEngine::enableProfiling() {
    profilingEnabled_ = true;
    if (profiler_) {
        profiler_->startProfiling();
    }
}

void JavaScriptEngine::disableProfiling() {
    profilingEnabled_ = false;
    if (profiler_) {
        profiler_->stopProfiling();
    }
}

void JavaScriptEngine::enableOptimization() {
    optimizationEnabled_ = true;
    if (optimizer_) {
        optimizer_->enableOptimization();
    }
}

void JavaScriptEngine::disableOptimization() {
    optimizationEnabled_ = false;
    if (optimizer_) {
        optimizer_->disableOptimization();
    }
}

void JavaScriptEngine::enableDebugging() {
    debuggingEnabled_ = true;
    if (debugger_) {
        debugger_->enableDebugging();
    }
}

void JavaScriptEngine::disableDebugging() {
    debuggingEnabled_ = false;
    if (debugger_) {
        debugger_->disableDebugging();
    }
}

void JavaScriptEngine::enableGC() {
    gcEnabled_ = true;
    if (gc_) {
        gc_->enableGC();
    }
}

void JavaScriptEngine::disableGC() {
    gcEnabled_ = false;
    if (gc_) {
        gc_->disableGC();
    }
}

void JavaScriptEngine::runGC() {
    if (gcEnabled_ && gc_) {
        gc_->runGC();
    }
}

size_t JavaScriptEngine::getHeapSize() const {
    if (gc_) {
        return gc_->getHeapSize();
    }
    return 0;
}

size_t JavaScriptEngine::getHeapUsed() const {
    if (gc_) {
        return gc_->getHeapUsed();
    }
    return 0;
}

void JavaScriptEngine::setStrictMode(bool strict) {
    strictMode_ = strict;
}

void JavaScriptEngine::setModuleMode(bool module) {
    moduleMode_ = module;
}

void JavaScriptEngine::setAsyncMode(bool async) {
    asyncMode_ = async;
}

double JavaScriptEngine::getAverageExecutionTime() const {
    if (executionCount_ == 0) {
        return 0.0;
    }
    return totalExecutionTime_ / executionCount_;
}

double JavaScriptEngine::getTotalExecutionTime() const {
    return totalExecutionTime_;
}

} // namespace js
