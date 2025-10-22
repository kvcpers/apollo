#pragma once

#include "ast.h"
#include "value.h"
#include "context.h"
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
class Context;

// JavaScript Interpreter
class Interpreter {
public:
    Interpreter();
    ~Interpreter();

    // Execution
    std::unique_ptr<Value> execute(std::unique_ptr<Program> program, Context* context);
    std::unique_ptr<Value> execute(std::unique_ptr<Module> module, Context* context);
    std::unique_ptr<Value> execute(std::unique_ptr<Statement> statement, Context* context);
    std::unique_ptr<Value> execute(std::unique_ptr<Expression> expression, Context* context);
    std::unique_ptr<Value> execute(std::unique_ptr<Declaration> declaration, Context* context);

    // Statement execution
    std::unique_ptr<Value> executeBlockStatement(BlockStatement* statement, Context* context);
    std::unique_ptr<Value> executeVariableStatement(VariableStatement* statement, Context* context);
    std::unique_ptr<Value> executeFunctionStatement(FunctionStatement* statement, Context* context);
    std::unique_ptr<Value> executeClassStatement(ClassStatement* statement, Context* context);
    std::unique_ptr<Value> executeIfStatement(IfStatement* statement, Context* context);
    std::unique_ptr<Value> executeSwitchStatement(SwitchStatement* statement, Context* context);
    std::unique_ptr<Value> executeForStatement(ForStatement* statement, Context* context);
    std::unique_ptr<Value> executeWhileStatement(WhileStatement* statement, Context* context);
    std::unique_ptr<Value> executeDoWhileStatement(DoWhileStatement* statement, Context* context);
    std::unique_ptr<Value> executeForInStatement(ForInStatement* statement, Context* context);
    std::unique_ptr<Value> executeForOfStatement(ForOfStatement* statement, Context* context);
    std::unique_ptr<Value> executeTryStatement(TryStatement* statement, Context* context);
    std::unique_ptr<Value> executeThrowStatement(ThrowStatement* statement, Context* context);
    std::unique_ptr<Value> executeReturnStatement(ReturnStatement* statement, Context* context);
    std::unique_ptr<Value> executeBreakStatement(BreakStatement* statement, Context* context);
    std::unique_ptr<Value> executeContinueStatement(ContinueStatement* statement, Context* context);
    std::unique_ptr<Value> executeLabeledStatement(LabeledStatement* statement, Context* context);
    std::unique_ptr<Value> executeWithStatement(WithStatement* statement, Context* context);
    std::unique_ptr<Value> executeDebuggerStatement(DebuggerStatement* statement, Context* context);

    // Expression execution
    std::unique_ptr<Value> executeIdentifier(Identifier* expression, Context* context);
    std::unique_ptr<Value> executeLiteral(Literal* expression, Context* context);
    std::unique_ptr<Value> executeBinaryExpression(BinaryExpression* expression, Context* context);
    std::unique_ptr<Value> executeUnaryExpression(UnaryExpression* expression, Context* context);
    std::unique_ptr<Value> executeConditionalExpression(ConditionalExpression* expression, Context* context);
    std::unique_ptr<Value> executeCallExpression(CallExpression* expression, Context* context);
    std::unique_ptr<Value> executeMemberExpression(MemberExpression* expression, Context* context);
    std::unique_ptr<Value> executeArrayExpression(ArrayExpression* expression, Context* context);
    std::unique_ptr<Value> executeObjectExpression(ObjectExpression* expression, Context* context);
    std::unique_ptr<Value> executeFunctionExpression(FunctionExpression* expression, Context* context);
    std::unique_ptr<Value> executeArrowFunctionExpression(ArrowFunctionExpression* expression, Context* context);
    std::unique_ptr<Value> executeClassExpression(ClassExpression* expression, Context* context);
    std::unique_ptr<Value> executeTemplateLiteral(TemplateLiteral* expression, Context* context);
    std::unique_ptr<Value> executeTaggedTemplateExpression(TaggedTemplateExpression* expression, Context* context);
    std::unique_ptr<Value> executeSequenceExpression(SequenceExpression* expression, Context* context);
    std::unique_ptr<Value> executeAssignmentExpression(AssignmentExpression* expression, Context* context);
    std::unique_ptr<Value> executeUpdateExpression(UpdateExpression* expression, Context* context);
    std::unique_ptr<Value> executeLogicalExpression(LogicalExpression* expression, Context* context);
    std::unique_ptr<Value> executeNewExpression(NewExpression* expression, Context* context);
    std::unique_ptr<Value> executeSuperExpression(SuperExpression* expression, Context* context);
    std::unique_ptr<Value> executeThisExpression(ThisExpression* expression, Context* context);
    std::unique_ptr<Value> executeYieldExpression(YieldExpression* expression, Context* context);
    std::unique_ptr<Value> executeAwaitExpression(AwaitExpression* expression, Context* context);
    std::unique_ptr<Value> executeMetaProperty(MetaProperty* expression, Context* context);

    // Declaration execution
    std::unique_ptr<Value> executeVariableDeclaration(VariableDeclaration* declaration, Context* context);
    std::unique_ptr<Value> executeFunctionDeclaration(FunctionDeclaration* declaration, Context* context);
    std::unique_ptr<Value> executeClassDeclaration(ClassDeclaration* declaration, Context* context);
    std::unique_ptr<Value> executeImportDeclaration(ImportDeclaration* declaration, Context* context);
    std::unique_ptr<Value> executeExportDeclaration(ExportDeclaration* declaration, Context* context);

    // Pattern execution
    std::unique_ptr<Value> executeArrayPattern(ArrayPattern* pattern, std::unique_ptr<Value> value, Context* context);
    std::unique_ptr<Value> executeObjectPattern(ObjectPattern* pattern, std::unique_ptr<Value> value, Context* context);
    std::unique_ptr<Value> executeRestPattern(RestPattern* pattern, std::unique_ptr<Value> value, Context* context);
    std::unique_ptr<Value> executeAssignmentPattern(AssignmentPattern* pattern, std::unique_ptr<Value> value, Context* context);

    // Property execution
    std::unique_ptr<Value> executeProperty(Property* property, std::unique_ptr<Value> object, Context* context);

    // Element execution
    std::unique_ptr<Value> executeElement(Element* element, std::unique_ptr<Value> array, Context* context);

    // Case clause execution
    std::unique_ptr<Value> executeCaseClause(CaseClause* caseClause, std::unique_ptr<Value> discriminant, Context* context);

    // Catch clause execution
    std::unique_ptr<Value> executeCatchClause(CatchClause* catchClause, std::unique_ptr<Value> error, Context* context);

    // Import specifier execution
    std::unique_ptr<Value> executeImportSpecifier(ImportSpecifier* specifier, std::unique_ptr<Value> module, Context* context);

    // Export specifier execution
    std::unique_ptr<Value> executeExportSpecifier(ExportSpecifier* specifier, std::unique_ptr<Value> module, Context* context);

    // Template element execution
    std::unique_ptr<Value> executeTemplateElement(TemplateElement* element, std::unique_ptr<Value> template, Context* context);

    // Operator execution
    std::unique_ptr<Value> executeOperator(OperatorType op, std::unique_ptr<Value> left, std::unique_ptr<Value> right, Context* context);
    std::unique_ptr<Value> executeUnaryOperator(OperatorType op, std::unique_ptr<Value> operand, Context* context);
    std::unique_ptr<Value> executeBinaryOperator(OperatorType op, std::unique_ptr<Value> left, std::unique_ptr<Value> right, Context* context);
    std::unique_ptr<Value> executeLogicalOperator(OperatorType op, std::unique_ptr<Value> left, std::unique_ptr<Value> right, Context* context);
    std::unique_ptr<Value> executeBitwiseOperator(OperatorType op, std::unique_ptr<Value> left, std::unique_ptr<Value> right, Context* context);
    std::unique_ptr<Value> executeComparisonOperator(OperatorType op, std::unique_ptr<Value> left, std::unique_ptr<Value> right, Context* context);
    std::unique_ptr<Value> executeArithmeticOperator(OperatorType op, std::unique_ptr<Value> left, std::unique_ptr<Value> right, Context* context);
    std::unique_ptr<Value> executeAssignmentOperator(OperatorType op, std::unique_ptr<Value> left, std::unique_ptr<Value> right, Context* context);

    // Function execution
    std::unique_ptr<Value> executeFunction(Function* function, const std::vector<std::unique_ptr<Value>>& arguments, Context* context);
    std::unique_ptr<Value> executeArrowFunction(ArrowFunctionExpression* function, const std::vector<std::unique_ptr<Value>>& arguments, Context* context);
    std::unique_ptr<Value> executeClass(ClassExpression* classExpr, const std::vector<std::unique_ptr<Value>>& arguments, Context* context);

    // Scope management
    void enterScope(Scope* scope);
    void exitScope();
    Scope* currentScope() const;
    std::unique_ptr<Value> resolveIdentifier(const std::string& name, Context* context);
    void declareVariable(const std::string& name, std::unique_ptr<Value> value, Context* context);
    void assignVariable(const std::string& name, std::unique_ptr<Value> value, Context* context);

    // Environment management
    void enterEnvironment(Environment* environment);
    void exitEnvironment();
    Environment* currentEnvironment() const;
    std::unique_ptr<Value> resolveBinding(const std::string& name, Context* context);
    void createBinding(const std::string& name, std::unique_ptr<Value> value, Context* context);
    void setBinding(const std::string& name, std::unique_ptr<Value> value, Context* context);

    // Context management
    void pushContext(Context* context);
    void popContext();
    Context* currentContext() const;

    // Error handling
    void throwError(const std::string& message, Context* context);
    void throwError(std::unique_ptr<Error> error, Context* context);
    void throwException(const std::string& message, Context* context);
    void throwException(std::unique_ptr<Exception> exception, Context* context);
    bool hasError() const;
    std::unique_ptr<Error> getError();
    void clearError();

    // Control flow
    void breakLoop(const std::string& label = "");
    void continueLoop(const std::string& label = "");
    void returnValue(std::unique_ptr<Value> value);
    bool shouldBreak() const;
    bool shouldContinue() const;
    bool shouldReturn() const;
    std::unique_ptr<Value> getReturnValue();
    void clearControlFlow();

    // Async execution
    std::unique_ptr<Value> executeAsync(std::unique_ptr<Program> program, Context* context);
    std::unique_ptr<Value> executeAsync(std::unique_ptr<Module> module, Context* context);
    std::unique_ptr<Value> executeAsync(std::unique_ptr<Statement> statement, Context* context);
    std::unique_ptr<Value> executeAsync(std::unique_ptr<Expression> expression, Context* context);
    std::unique_ptr<Value> executeAsync(std::unique_ptr<Declaration> declaration, Context* context);

    // Generator execution
    std::unique_ptr<Value> executeGenerator(std::unique_ptr<Function> function, const std::vector<std::unique_ptr<Value>>& arguments, Context* context);
    std::unique_ptr<Value> executeGenerator(std::unique_ptr<ArrowFunctionExpression> function, const std::vector<std::unique_ptr<Value>>& arguments, Context* context);
    std::unique_ptr<Value> nextGenerator(std::unique_ptr<Value> generator, std::unique_ptr<Value> value, Context* context);
    std::unique_ptr<Value> throwGenerator(std::unique_ptr<Value> generator, std::unique_ptr<Value> error, Context* context);
    std::unique_ptr<Value> returnGenerator(std::unique_ptr<Value> generator, std::unique_ptr<Value> value, Context* context);

    // Promise execution
    std::unique_ptr<Value> executePromise(std::unique_ptr<Function> function, const std::vector<std::unique_ptr<Value>>& arguments, Context* context);
    std::unique_ptr<Value> executePromise(std::unique_ptr<ArrowFunctionExpression> function, const std::vector<std::unique_ptr<Value>>& arguments, Context* context);
    std::unique_ptr<Value> resolvePromise(std::unique_ptr<Value> promise, std::unique_ptr<Value> value, Context* context);
    std::unique_ptr<Value> rejectPromise(std::unique_ptr<Value> promise, std::unique_ptr<Value> error, Context* context);

    // Module execution
    std::unique_ptr<Value> executeModule(std::unique_ptr<Module> module, Context* context);
    std::unique_ptr<Value> loadModule(const std::string& specifier, Context* context);
    std::unique_ptr<Value> resolveModule(const std::string& specifier, const std::string& referrer, Context* context);

    // Debugging
    void setBreakpoint(const std::string& filename, size_t line, size_t column);
    void removeBreakpoint(const std::string& filename, size_t line, size_t column);
    void clearBreakpoints();
    void step();
    void stepOver();
    void stepInto();
    void stepOut();
    void continueExecution();
    void pauseExecution();
    bool isPaused() const;
    bool isStepping() const;
    bool isAtBreakpoint() const;
    std::vector<std::string> getCallStack() const;
    std::unordered_map<std::string, std::unique_ptr<Value>> getLocalVariables() const;
    std::unordered_map<std::string, std::unique_ptr<Value>> getGlobalVariables() const;

    // Profiling
    void startProfiling();
    void stopProfiling();
    void pauseProfiling();
    void resumeProfiling();
    bool isProfiling() const;
    std::unordered_map<std::string, double> getProfileData() const;
    void clearProfileData();

    // Optimization
    void enableOptimization();
    void disableOptimization();
    bool isOptimizationEnabled() const;
    void optimize(std::unique_ptr<Program> program);
    void optimize(std::unique_ptr<Module> module);
    void optimize(std::unique_ptr<Statement> statement);
    void optimize(std::unique_ptr<Expression> expression);
    void optimize(std::unique_ptr<Declaration> declaration);

    // Statistics
    size_t getExecutionCount() const;
    size_t getErrorCount() const;
    double getAverageExecutionTime() const;
    double getTotalExecutionTime() const;
    void resetStatistics();

private:
    // Execution state
    std::stack<Scope*> scopeStack_;
    std::stack<Environment*> environmentStack_;
    std::stack<Context*> contextStack_;
    
    // Control flow state
    bool shouldBreak_;
    bool shouldContinue_;
    bool shouldReturn_;
    std::string breakLabel_;
    std::string continueLabel_;
    std::unique_ptr<Value> returnValue_;
    
    // Error state
    bool hasError_;
    std::unique_ptr<Error> currentError_;
    
    // Async state
    bool isAsync_;
    bool isGenerator_;
    bool isPromise_;
    
    // Debugging state
    bool isPaused_;
    bool isStepping_;
    bool isAtBreakpoint_;
    std::vector<std::string> callStack_;
    std::unordered_map<std::string, std::vector<std::pair<size_t, size_t>>> breakpoints_;
    
    // Profiling state
    bool isProfiling_;
    std::unordered_map<std::string, double> profileData_;
    
    // Optimization state
    bool optimizationEnabled_;
    
    // Statistics
    size_t executionCount_;
    size_t errorCount_;
    double totalExecutionTime_;
    
    // Helper methods
    void setupDefaultBreakpoints();
    void setupDefaultProfiling();
    void setupDefaultOptimization();
    void collectExecutionStatistics();
    void collectErrorStatistics();
    void collectPerformanceStatistics();
    void resetExecutionState();
    void resetControlFlowState();
    void resetErrorState();
    void resetAsyncState();
    void resetDebuggingState();
    void resetProfilingState();
    void resetOptimizationState();
    void resetStatistics();
    
    // Internal execution methods
    std::unique_ptr<Value> executeInternal(std::unique_ptr<Program> program, Context* context);
    std::unique_ptr<Value> executeInternal(std::unique_ptr<Module> module, Context* context);
    std::unique_ptr<Value> executeInternal(std::unique_ptr<Statement> statement, Context* context);
    std::unique_ptr<Value> executeInternal(std::unique_ptr<Expression> expression, Context* context);
    std::unique_ptr<Value> executeInternal(std::unique_ptr<Declaration> declaration, Context* context);
    
    // Internal helper methods
    std::unique_ptr<Value> createValue(ValueType type);
    std::unique_ptr<Value> createValue(const std::string& value);
    std::unique_ptr<Value> createValue(double value);
    std::unique_ptr<Value> createValue(bool value);
    std::unique_ptr<Value> createValue(std::nullptr_t);
    std::unique_ptr<Value> createValue();
    
    // Internal operator methods
    std::unique_ptr<Value> executeInternalOperator(OperatorType op, std::unique_ptr<Value> left, std::unique_ptr<Value> right, Context* context);
    std::unique_ptr<Value> executeInternalUnaryOperator(OperatorType op, std::unique_ptr<Value> operand, Context* context);
    std::unique_ptr<Value> executeInternalBinaryOperator(OperatorType op, std::unique_ptr<Value> left, std::unique_ptr<Value> right, Context* context);
    std::unique_ptr<Value> executeInternalLogicalOperator(OperatorType op, std::unique_ptr<Value> left, std::unique_ptr<Value> right, Context* context);
    std::unique_ptr<Value> executeInternalBitwiseOperator(OperatorType op, std::unique_ptr<Value> left, std::unique_ptr<Value> right, Context* context);
    std::unique_ptr<Value> executeInternalComparisonOperator(OperatorType op, std::unique_ptr<Value> left, std::unique_ptr<Value> right, Context* context);
    std::unique_ptr<Value> executeInternalArithmeticOperator(OperatorType op, std::unique_ptr<Value> left, std::unique_ptr<Value> right, Context* context);
    std::unique_ptr<Value> executeInternalAssignmentOperator(OperatorType op, std::unique_ptr<Value> left, std::unique_ptr<Value> right, Context* context);
    
    // Internal function methods
    std::unique_ptr<Value> executeInternalFunction(Function* function, const std::vector<std::unique_ptr<Value>>& arguments, Context* context);
    std::unique_ptr<Value> executeInternalArrowFunction(ArrowFunctionExpression* function, const std::vector<std::unique_ptr<Value>>& arguments, Context* context);
    std::unique_ptr<Value> executeInternalClass(ClassExpression* classExpr, const std::vector<std::unique_ptr<Value>>& arguments, Context* context);
    
    // Internal scope methods
    void enterInternalScope(Scope* scope);
    void exitInternalScope();
    Scope* getCurrentInternalScope() const;
    std::unique_ptr<Value> resolveInternalIdentifier(const std::string& name, Context* context);
    void declareInternalVariable(const std::string& name, std::unique_ptr<Value> value, Context* context);
    void assignInternalVariable(const std::string& name, std::unique_ptr<Value> value, Context* context);
    
    // Internal environment methods
    void enterInternalEnvironment(Environment* environment);
    void exitInternalEnvironment();
    Environment* getCurrentInternalEnvironment() const;
    std::unique_ptr<Value> resolveInternalBinding(const std::string& name, Context* context);
    void createInternalBinding(const std::string& name, std::unique_ptr<Value> value, Context* context);
    void setInternalBinding(const std::string& name, std::unique_ptr<Value> value, Context* context);
    
    // Internal context methods
    void pushInternalContext(Context* context);
    void popInternalContext();
    Context* getCurrentInternalContext() const;
    
    // Internal error methods
    void throwInternalError(const std::string& message, Context* context);
    void throwInternalError(std::unique_ptr<Error> error, Context* context);
    void throwInternalException(const std::string& message, Context* context);
    void throwInternalException(std::unique_ptr<Exception> exception, Context* context);
    bool hasInternalError() const;
    std::unique_ptr<Error> getInternalError();
    void clearInternalError();
    
    // Internal control flow methods
    void breakInternalLoop(const std::string& label = "");
    void continueInternalLoop(const std::string& label = "");
    void returnInternalValue(std::unique_ptr<Value> value);
    bool shouldInternalBreak() const;
    bool shouldInternalContinue() const;
    bool shouldInternalReturn() const;
    std::unique_ptr<Value> getInternalReturnValue();
    void clearInternalControlFlow();
    
    // Internal async methods
    std::unique_ptr<Value> executeInternalAsync(std::unique_ptr<Program> program, Context* context);
    std::unique_ptr<Value> executeInternalAsync(std::unique_ptr<Module> module, Context* context);
    std::unique_ptr<Value> executeInternalAsync(std::unique_ptr<Statement> statement, Context* context);
    std::unique_ptr<Value> executeInternalAsync(std::unique_ptr<Expression> expression, Context* context);
    std::unique_ptr<Value> executeInternalAsync(std::unique_ptr<Declaration> declaration, Context* context);
    
    // Internal generator methods
    std::unique_ptr<Value> executeInternalGenerator(std::unique_ptr<Function> function, const std::vector<std::unique_ptr<Value>>& arguments, Context* context);
    std::unique_ptr<Value> executeInternalGenerator(std::unique_ptr<ArrowFunctionExpression> function, const std::vector<std::unique_ptr<Value>>& arguments, Context* context);
    std::unique_ptr<Value> nextInternalGenerator(std::unique_ptr<Value> generator, std::unique_ptr<Value> value, Context* context);
    std::unique_ptr<Value> throwInternalGenerator(std::unique_ptr<Value> generator, std::unique_ptr<Value> error, Context* context);
    std::unique_ptr<Value> returnInternalGenerator(std::unique_ptr<Value> generator, std::unique_ptr<Value> value, Context* context);
    
    // Internal promise methods
    std::unique_ptr<Value> executeInternalPromise(std::unique_ptr<Function> function, const std::vector<std::unique_ptr<Value>>& arguments, Context* context);
    std::unique_ptr<Value> executeInternalPromise(std::unique_ptr<ArrowFunctionExpression> function, const std::vector<std::unique_ptr<Value>>& arguments, Context* context);
    std::unique_ptr<Value> resolveInternalPromise(std::unique_ptr<Value> promise, std::unique_ptr<Value> value, Context* context);
    std::unique_ptr<Value> rejectInternalPromise(std::unique_ptr<Value> promise, std::unique_ptr<Value> error, Context* context);
    
    // Internal module methods
    std::unique_ptr<Value> executeInternalModule(std::unique_ptr<Module> module, Context* context);
    std::unique_ptr<Value> loadInternalModule(const std::string& specifier, Context* context);
    std::unique_ptr<Value> resolveInternalModule(const std::string& specifier, const std::string& referrer, Context* context);
    
    // Internal debugging methods
    void setInternalBreakpoint(const std::string& filename, size_t line, size_t column);
    void removeInternalBreakpoint(const std::string& filename, size_t line, size_t column);
    void clearInternalBreakpoints();
    void stepInternal();
    void stepOverInternal();
    void stepIntoInternal();
    void stepOutInternal();
    void continueInternalExecution();
    void pauseInternalExecution();
    bool isInternalPaused() const;
    bool isInternalStepping() const;
    bool isInternalAtBreakpoint() const;
    std::vector<std::string> getInternalCallStack() const;
    std::unordered_map<std::string, std::unique_ptr<Value>> getInternalLocalVariables() const;
    std::unordered_map<std::string, std::unique_ptr<Value>> getInternalGlobalVariables() const;
    
    // Internal profiling methods
    void startInternalProfiling();
    void stopInternalProfiling();
    void pauseInternalProfiling();
    void resumeInternalProfiling();
    bool isInternalProfiling() const;
    std::unordered_map<std::string, double> getInternalProfileData() const;
    void clearInternalProfileData();
    
    // Internal optimization methods
    void enableInternalOptimization();
    void disableInternalOptimization();
    bool isInternalOptimizationEnabled() const;
    void optimizeInternal(std::unique_ptr<Program> program);
    void optimizeInternal(std::unique_ptr<Module> module);
    void optimizeInternal(std::unique_ptr<Statement> statement);
    void optimizeInternal(std::unique_ptr<Expression> expression);
    void optimizeInternal(std::unique_ptr<Declaration> declaration);
    
    // Internal statistics methods
    size_t getInternalExecutionCount() const;
    size_t getInternalErrorCount() const;
    double getInternalAverageExecutionTime() const;
    double getInternalTotalExecutionTime() const;
    void resetInternalStatistics();
};

} // namespace js
