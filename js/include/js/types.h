#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include <memory>
#include <unordered_map>
#include <functional>

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
class Context;
class Interpreter;
class Parser;
class Tokenizer;
class AST;
class Node;
class Statement;
class Expression;
class Declaration;
class Identifier;
class Literal;
class BinaryExpression;
class UnaryExpression;
class ConditionalExpression;
class CallExpression;
class MemberExpression;
class ArrayExpression;
class ObjectExpression;
class FunctionExpression;
class ArrowFunctionExpression;
class ClassExpression;
class TemplateLiteral;
class TaggedTemplateExpression;
class SequenceExpression;
class AssignmentExpression;
class UpdateExpression;
class LogicalExpression;
class ForStatement;
class WhileStatement;
class DoWhileStatement;
class ForInStatement;
class ForOfStatement;
class IfStatement;
class SwitchStatement;
class TryStatement;
class ThrowStatement;
class ReturnStatement;
class BreakStatement;
class ContinueStatement;
class LabeledStatement;
class WithStatement;
class BlockStatement;
class VariableDeclaration;
class FunctionDeclaration;
class ClassDeclaration;
class ImportDeclaration;
class ExportDeclaration;
class Program;
class Module;
class EventLoop;
class Promise;
class AsyncFunction;
class Generator;
class AsyncGenerator;
class Iterator;
class AsyncIterator;
class Symbol;
class Proxy;
class WeakMap;
class WeakSet;
class Map;
class Set;
class Date;
class RegExp;
class JSON;
class Math;
class Console;
class DOMBindings;
class GarbageCollector;
class Compiler;
class Optimizer;
class Debugger;
class Profiler;
class Engine;

// Basic types
using int8_t = std::int8_t;
using int16_t = std::int16_t;
using int32_t = std::int32_t;
using int64_t = std::int64_t;
using uint8_t = std::uint8_t;
using uint16_t = std::uint16_t;
using uint32_t = std::uint32_t;
using uint64_t = std::uint64_t;
using size_t = std::size_t;
using ptrdiff_t = std::ptrdiff_t;

// String types
using String = std::string;
using StringView = std::string_view;
using WString = std::wstring;
using WStringView = std::wstring_view;

// Container types
template<typename T>
using Vector = std::vector<T>;
template<typename K, typename V>
using HashMap = std::unordered_map<K, V>;
template<typename K, typename V>
using Map = std::map<K, V>;
template<typename T>
using Set = std::set<T>;
template<typename T>
using List = std::list<T>;
template<typename T>
using Deque = std::deque<T>;
template<typename T>
using Stack = std::stack<T>;
template<typename T>
using Queue = std::queue<T>;
template<typename T>
using PriorityQueue = std::priority_queue<T>;

// Smart pointer types
template<typename T>
using UniquePtr = std::unique_ptr<T>;
template<typename T>
using SharedPtr = std::shared_ptr<T>;
template<typename T>
using WeakPtr = std::weak_ptr<T>;

// Function types
template<typename... Args>
using Function = std::function<void(Args...)>;
template<typename R, typename... Args>
using FunctionR = std::function<R(Args...)>;

// Optional type
template<typename T>
using Optional = std::optional<T>;

// Variant type
template<typename... Types>
using Variant = std::variant<Types...>;

// Any type
using Any = std::any;

// Source location
struct SourceLocation {
    size_t line;
    size_t column;
    size_t offset;
    String filename;

    SourceLocation() : line(1), column(1), offset(0), filename() {}
    SourceLocation(size_t line, size_t column, size_t offset, const String& filename = "")
        : line(line), column(column), offset(offset), filename(filename) {}

    bool operator==(const SourceLocation& other) const {
        return line == other.line && column == other.column && offset == other.offset && filename == other.filename;
    }
    bool operator!=(const SourceLocation& other) const { return !(*this == other); }
};

// Token position
struct TokenPosition {
    SourceLocation start;
    SourceLocation end;

    TokenPosition() : start(), end() {}
    TokenPosition(const SourceLocation& start, const SourceLocation& end) : start(start), end(end) {}

    bool operator==(const TokenPosition& other) const {
        return start == other.start && end == other.end;
    }
    bool operator!=(const TokenPosition& other) const { return !(*this == other); }
};

// Error types
enum class ErrorType {
    None,
    SyntaxError,
    TypeError,
    ReferenceError,
    RangeError,
    EvalError,
    URIError,
    InternalError,
    AggregateError
};

// Value types
enum class ValueType {
    Undefined,
    Null,
    Boolean,
    Number,
    String,
    Symbol,
    BigInt,
    Object,
    Function,
    Array,
    Date,
    RegExp,
    Error,
    Promise,
    Proxy,
    WeakMap,
    WeakSet,
    Map,
    Set,
    Iterator,
    AsyncIterator,
    Generator,
    AsyncGenerator,
    Module,
    Class,
    Instance
};

// Operator types
enum class OperatorType {
    // Assignment operators
    Assign,
    AddAssign,
    SubtractAssign,
    MultiplyAssign,
    DivideAssign,
    ModuloAssign,
    ExponentAssign,
    LeftShiftAssign,
    RightShiftAssign,
    UnsignedRightShiftAssign,
    BitwiseAndAssign,
    BitwiseXorAssign,
    BitwiseOrAssign,
    LogicalAndAssign,
    LogicalOrAssign,
    NullishAssign,

    // Arithmetic operators
    Add,
    Subtract,
    Multiply,
    Divide,
    Modulo,
    Exponent,
    Increment,
    Decrement,

    // Comparison operators
    Equal,
    NotEqual,
    StrictEqual,
    StrictNotEqual,
    LessThan,
    LessThanOrEqual,
    GreaterThan,
    GreaterThanOrEqual,
    InstanceOf,
    In,

    // Logical operators
    LogicalAnd,
    LogicalOr,
    LogicalNot,
    NullishCoalescing,

    // Bitwise operators
    BitwiseAnd,
    BitwiseOr,
    BitwiseXor,
    BitwiseNot,
    LeftShift,
    RightShift,
    UnsignedRightShift,

    // Unary operators
    UnaryPlus,
    UnaryMinus,
    TypeOf,
    Void,
    Delete,

    // Ternary operator
    Conditional,

    // Comma operator
    Comma,

    // Member access
    MemberAccess,
    ComputedMemberAccess,
    OptionalMemberAccess,
    OptionalComputedMemberAccess,

    // Function call
    Call,
    OptionalCall,

    // Template literal
    TemplateLiteral,
    TaggedTemplateLiteral,

    // Spread operator
    Spread,

    // Rest operator
    Rest,

    // Destructuring
    Destructuring,

    // Import/Export
    Import,
    Export,

    // Await
    Await,

    // Yield
    Yield,

    // Super
    Super,

    // This
    This,

    // New
    New
};

// Statement types
enum class StatementType {
    Expression,
    Block,
    VariableDeclaration,
    FunctionDeclaration,
    ClassDeclaration,
    ImportDeclaration,
    ExportDeclaration,
    IfStatement,
    SwitchStatement,
    ForStatement,
    WhileStatement,
    DoWhileStatement,
    ForInStatement,
    ForOfStatement,
    TryStatement,
    ThrowStatement,
    ReturnStatement,
    BreakStatement,
    ContinueStatement,
    LabeledStatement,
    WithStatement,
    DebuggerStatement
};

// Expression types
enum class ExpressionType {
    Identifier,
    Literal,
    BinaryExpression,
    UnaryExpression,
    ConditionalExpression,
    CallExpression,
    MemberExpression,
    ArrayExpression,
    ObjectExpression,
    FunctionExpression,
    ArrowFunctionExpression,
    ClassExpression,
    TemplateLiteral,
    TaggedTemplateExpression,
    SequenceExpression,
    AssignmentExpression,
    UpdateExpression,
    LogicalExpression,
    NewExpression,
    SuperExpression,
    ThisExpression,
    YieldExpression,
    AwaitExpression,
    SpreadElement,
    RestElement,
    DestructuringPattern,
    ImportExpression,
    MetaProperty
};

// Declaration types
enum class DeclarationType {
    Variable,
    Function,
    Class,
    Import,
    Export
};

// Literal types
enum class LiteralType {
    String,
    Number,
    Boolean,
    Null,
    Undefined,
    RegExp,
    TemplateLiteral,
    BigInt
};

// Function types
enum class FunctionType {
    Function,
    ArrowFunction,
    AsyncFunction,
    GeneratorFunction,
    AsyncGeneratorFunction,
    Method,
    Constructor,
    Getter,
    Setter
};

// Class types
enum class ClassType {
    Class,
    Interface,
    Enum,
    Namespace,
    Module
};

// Module types
enum class ModuleType {
    Script,
    Module,
    CommonJS,
    AMD,
    UMD
};

// Binding types
enum class BindingType {
    Variable,
    Function,
    Class,
    Import,
    Export,
    Parameter,
    Catch,
    Loop
};

// Scope types
enum class ScopeType {
    Global,
    Function,
    Block,
    Module,
    Catch,
    With,
    Eval
};

// Environment types
enum class EnvironmentType {
    Global,
    Function,
    Block,
    Module,
    Catch,
    With,
    Eval
};

// Context types
enum class ContextType {
    Global,
    Function,
    Block,
    Module,
    Catch,
    With,
    Eval
};

// Garbage collection types
enum class GCType {
    MarkAndSweep,
    Generational,
    Incremental,
    Concurrent,
    Parallel
};

// Compilation types
enum class CompilationType {
    Interpretation,
    JIT,
    AOT,
    Hybrid
};

// Optimization types
enum class OptimizationType {
    None,
    Basic,
    Aggressive,
    Maximum
};

// Debug types
enum class DebugType {
    None,
    Source,
    Symbol,
    Line,
    Column,
    Full
};

// Profiling types
enum class ProfilingType {
    None,
    CPU,
    Memory,
    Time,
    All
};

// Engine types
enum class EngineType {
    Interpreter,
    JIT,
    AOT,
    Hybrid
};

// Result type
template<typename T>
struct Result {
    T value;
    ErrorType error;
    String message;

    Result() : value(), error(ErrorType::None), message() {}
    Result(const T& value) : value(value), error(ErrorType::None), message() {}
    Result(ErrorType error, const String& message = "") : value(), error(error), message(message) {}

    bool isOk() const { return error == ErrorType::None; }
    bool isError() const { return error != ErrorType::None; }

    operator bool() const { return isOk(); }
    T& operator*() { return value; }
    const T& operator*() const { return value; }
    T* operator->() { return &value; }
    const T* operator->() const { return &value; }
};

// Utility functions
template<typename T>
Result<T> Ok(const T& value) {
    return Result<T>(value);
}

template<typename T>
Result<T> Err(ErrorType error, const String& message = "") {
    return Result<T>(error, message);
}

} // namespace js
