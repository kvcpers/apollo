#pragma once

#include "types.h"
#include "tokenizer.h"
#include <memory>
#include <vector>
#include <string>
#include <unordered_map>

namespace js {

// Forward declarations
class Node;
class Statement;
class Expression;
class Declaration;
class Pattern;
class Parameter;
class Property;
class Element;
class CaseClause;
class CatchClause;
class ImportSpecifier;
class ExportSpecifier;
class TemplateElement;
class MetaProperty;
class SuperExpression;
class ThisExpression;
class YieldExpression;
class AwaitExpression;
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
class NewExpression;
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

// Base AST node
class Node {
public:
    Node(NodeType type, const TokenPosition& position);
    virtual ~Node() = default;

    NodeType type() const { return type_; }
    const TokenPosition& position() const { return position_; }
    void setPosition(const TokenPosition& position) { position_ = position; }

    virtual std::string toString() const = 0;
    virtual void accept(ASTVisitor& visitor) = 0;

    bool operator==(const Node& other) const;
    bool operator!=(const Node& other) const { return !(*this == other); }

protected:
    NodeType type_;
    TokenPosition position_;
};

// Statement node
class Statement : public Node {
public:
    Statement(StatementType type, const TokenPosition& position);
    virtual ~Statement() = default;

    StatementType statementType() const { return statementType_; }
    void setStatementType(StatementType type) { statementType_ = type; }

    virtual std::string toString() const override;
    virtual void accept(ASTVisitor& visitor) override;

protected:
    StatementType statementType_;
};

// Expression node
class Expression : public Node {
public:
    Expression(ExpressionType type, const TokenPosition& position);
    virtual ~Expression() = default;

    ExpressionType expressionType() const { return expressionType_; }
    void setExpressionType(ExpressionType type) { expressionType_ = type; }

    virtual std::string toString() const override;
    virtual void accept(ASTVisitor& visitor) override;

protected:
    ExpressionType expressionType_;
};

// Declaration node
class Declaration : public Node {
public:
    Declaration(DeclarationType type, const TokenPosition& position);
    virtual ~Declaration() = default;

    DeclarationType declarationType() const { return declarationType_; }
    void setDeclarationType(DeclarationType type) { declarationType_ = type; }

    virtual std::string toString() const override;
    virtual void accept(ASTVisitor& visitor) override;

protected:
    DeclarationType declarationType_;
};

// Pattern node
class Pattern : public Node {
public:
    Pattern(PatternType type, const TokenPosition& position);
    virtual ~Pattern() = default;

    PatternType patternType() const { return patternType_; }
    void setPatternType(PatternType type) { patternType_ = type; }

    virtual std::string toString() const override;
    virtual void accept(ASTVisitor& visitor) override;

protected:
    PatternType patternType_;
};

// Parameter node
class Parameter : public Node {
public:
    Parameter(const TokenPosition& position);
    virtual ~Parameter() = default;

    virtual std::string toString() const override;
    virtual void accept(ASTVisitor& visitor) override;
};

// Property node
class Property : public Node {
public:
    Property(PropertyType type, const TokenPosition& position);
    virtual ~Property() = default;

    PropertyType propertyType() const { return propertyType_; }
    void setPropertyType(PropertyType type) { propertyType_ = type; }

    virtual std::string toString() const override;
    virtual void accept(ASTVisitor& visitor) override;

protected:
    PropertyType propertyType_;
};

// Element node
class Element : public Node {
public:
    Element(const TokenPosition& position);
    virtual ~Element() = default;

    virtual std::string toString() const override;
    virtual void accept(ASTVisitor& visitor) override;
};

// Case clause node
class CaseClause : public Node {
public:
    CaseClause(const TokenPosition& position);
    virtual ~CaseClause() = default;

    virtual std::string toString() const override;
    virtual void accept(ASTVisitor& visitor) override;
};

// Catch clause node
class CatchClause : public Node {
public:
    CatchClause(const TokenPosition& position);
    virtual ~CatchClause() = default;

    virtual std::string toString() const override;
    virtual void accept(ASTVisitor& visitor) override;
};

// Import specifier node
class ImportSpecifier : public Node {
public:
    ImportSpecifier(const TokenPosition& position);
    virtual ~ImportSpecifier() = default;

    virtual std::string toString() const override;
    virtual void accept(ASTVisitor& visitor) override;
};

// Export specifier node
class ExportSpecifier : public Node {
public:
    ExportSpecifier(const TokenPosition& position);
    virtual ~ExportSpecifier() = default;

    virtual std::string toString() const override;
    virtual void accept(ASTVisitor& visitor) override;
};

// Template element node
class TemplateElement : public Node {
public:
    TemplateElement(const TokenPosition& position);
    virtual ~TemplateElement() = default;

    virtual std::string toString() const override;
    virtual void accept(ASTVisitor& visitor) override;
};

// Meta property node
class MetaProperty : public Node {
public:
    MetaProperty(const TokenPosition& position);
    virtual ~MetaProperty() = default;

    virtual std::string toString() const override;
    virtual void accept(ASTVisitor& visitor) override;
};

// Super expression node
class SuperExpression : public Node {
public:
    SuperExpression(const TokenPosition& position);
    virtual ~SuperExpression() = default;

    virtual std::string toString() const override;
    virtual void accept(ASTVisitor& visitor) override;
};

// This expression node
class ThisExpression : public Node {
public:
    ThisExpression(const TokenPosition& position);
    virtual ~ThisExpression() = default;

    virtual std::string toString() const override;
    virtual void accept(ASTVisitor& visitor) override;
};

// Yield expression node
class YieldExpression : public Node {
public:
    YieldExpression(const TokenPosition& position);
    virtual ~YieldExpression() = default;

    virtual std::string toString() const override;
    virtual void accept(ASTVisitor& visitor) override;
};

// Await expression node
class AwaitExpression : public Node {
public:
    AwaitExpression(const TokenPosition& position);
    virtual ~AwaitExpression() = default;

    virtual std::string toString() const override;
    virtual void accept(ASTVisitor& visitor) override;
};

// Identifier node
class Identifier : public Expression {
public:
    Identifier(const std::string& name, const TokenPosition& position);
    virtual ~Identifier() = default;

    const std::string& name() const { return name_; }
    void setName(const std::string& name) { name_ = name; }

    virtual std::string toString() const override;
    virtual void accept(ASTVisitor& visitor) override;

private:
    std::string name_;
};

// Literal node
class Literal : public Expression {
public:
    Literal(LiteralType type, const TokenPosition& position);
    virtual ~Literal() = default;

    LiteralType literalType() const { return literalType_; }
    void setLiteralType(LiteralType type) { literalType_ = type; }

    virtual std::string toString() const override;
    virtual void accept(ASTVisitor& visitor) override;

protected:
    LiteralType literalType_;
};

// Binary expression node
class BinaryExpression : public Expression {
public:
    BinaryExpression(OperatorType op, std::unique_ptr<Expression> left, std::unique_ptr<Expression> right, const TokenPosition& position);
    virtual ~BinaryExpression() = default;

    OperatorType operatorType() const { return operatorType_; }
    void setOperatorType(OperatorType op) { operatorType_ = op; }

    Expression* left() const { return left_.get(); }
    void setLeft(std::unique_ptr<Expression> left) { left_ = std::move(left); }

    Expression* right() const { return right_.get(); }
    void setRight(std::unique_ptr<Expression> right) { right_ = std::move(right); }

    virtual std::string toString() const override;
    virtual void accept(ASTVisitor& visitor) override;

private:
    OperatorType operatorType_;
    std::unique_ptr<Expression> left_;
    std::unique_ptr<Expression> right_;
};

// Unary expression node
class UnaryExpression : public Expression {
public:
    UnaryExpression(OperatorType op, std::unique_ptr<Expression> argument, const TokenPosition& position);
    virtual ~UnaryExpression() = default;

    OperatorType operatorType() const { return operatorType_; }
    void setOperatorType(OperatorType op) { operatorType_ = op; }

    Expression* argument() const { return argument_.get(); }
    void setArgument(std::unique_ptr<Expression> argument) { argument_ = std::move(argument); }

    virtual std::string toString() const override;
    virtual void accept(ASTVisitor& visitor) override;

private:
    OperatorType operatorType_;
    std::unique_ptr<Expression> argument_;
};

// Conditional expression node
class ConditionalExpression : public Expression {
public:
    ConditionalExpression(std::unique_ptr<Expression> test, std::unique_ptr<Expression> consequent, std::unique_ptr<Expression> alternate, const TokenPosition& position);
    virtual ~ConditionalExpression() = default;

    Expression* test() const { return test_.get(); }
    void setTest(std::unique_ptr<Expression> test) { test_ = std::move(test); }

    Expression* consequent() const { return consequent_.get(); }
    void setConsequent(std::unique_ptr<Expression> consequent) { consequent_ = std::move(consequent); }

    Expression* alternate() const { return alternate_.get(); }
    void setAlternate(std::unique_ptr<Expression> alternate) { alternate_ = std::move(alternate); }

    virtual std::string toString() const override;
    virtual void accept(ASTVisitor& visitor) override;

private:
    std::unique_ptr<Expression> test_;
    std::unique_ptr<Expression> consequent_;
    std::unique_ptr<Expression> alternate_;
};

// Call expression node
class CallExpression : public Expression {
public:
    CallExpression(std::unique_ptr<Expression> callee, std::vector<std::unique_ptr<Expression>> arguments, const TokenPosition& position);
    virtual ~CallExpression() = default;

    Expression* callee() const { return callee_.get(); }
    void setCallee(std::unique_ptr<Expression> callee) { callee_ = std::move(callee); }

    const std::vector<std::unique_ptr<Expression>>& arguments() const { return arguments_; }
    void setArguments(std::vector<std::unique_ptr<Expression>> arguments) { arguments_ = std::move(arguments); }

    virtual std::string toString() const override;
    virtual void accept(ASTVisitor& visitor) override;

private:
    std::unique_ptr<Expression> callee_;
    std::vector<std::unique_ptr<Expression>> arguments_;
};

// Member expression node
class MemberExpression : public Expression {
public:
    MemberExpression(std::unique_ptr<Expression> object, std::unique_ptr<Expression> property, bool computed, const TokenPosition& position);
    virtual ~MemberExpression() = default;

    Expression* object() const { return object_.get(); }
    void setObject(std::unique_ptr<Expression> object) { object_ = std::move(object); }

    Expression* property() const { return property_.get(); }
    void setProperty(std::unique_ptr<Expression> property) { property_ = std::move(property); }

    bool computed() const { return computed_; }
    void setComputed(bool computed) { computed_ = computed; }

    virtual std::string toString() const override;
    virtual void accept(ASTVisitor& visitor) override;

private:
    std::unique_ptr<Expression> object_;
    std::unique_ptr<Expression> property_;
    bool computed_;
};

// Array expression node
class ArrayExpression : public Expression {
public:
    ArrayExpression(std::vector<std::unique_ptr<Element>> elements, const TokenPosition& position);
    virtual ~ArrayExpression() = default;

    const std::vector<std::unique_ptr<Element>>& elements() const { return elements_; }
    void setElements(std::vector<std::unique_ptr<Element>> elements) { elements_ = std::move(elements); }

    virtual std::string toString() const override;
    virtual void accept(ASTVisitor& visitor) override;

private:
    std::vector<std::unique_ptr<Element>> elements_;
};

// Object expression node
class ObjectExpression : public Expression {
public:
    ObjectExpression(std::vector<std::unique_ptr<Property>> properties, const TokenPosition& position);
    virtual ~ObjectExpression() = default;

    const std::vector<std::unique_ptr<Property>>& properties() const { return properties_; }
    void setProperties(std::vector<std::unique_ptr<Property>> properties) { properties_ = std::move(properties); }

    virtual std::string toString() const override;
    virtual void accept(ASTVisitor& visitor) override;

private:
    std::vector<std::unique_ptr<Property>> properties_;
};

// Function expression node
class FunctionExpression : public Expression {
public:
    FunctionExpression(std::unique_ptr<Identifier> id, std::vector<std::unique_ptr<Parameter>> params, std::unique_ptr<BlockStatement> body, const TokenPosition& position);
    virtual ~FunctionExpression() = default;

    Identifier* id() const { return id_.get(); }
    void setId(std::unique_ptr<Identifier> id) { id_ = std::move(id); }

    const std::vector<std::unique_ptr<Parameter>>& params() const { return params_; }
    void setParams(std::vector<std::unique_ptr<Parameter>> params) { params_ = std::move(params); }

    BlockStatement* body() const { return body_.get(); }
    void setBody(std::unique_ptr<BlockStatement> body) { body_ = std::move(body); }

    virtual std::string toString() const override;
    virtual void accept(ASTVisitor& visitor) override;

private:
    std::unique_ptr<Identifier> id_;
    std::vector<std::unique_ptr<Parameter>> params_;
    std::unique_ptr<BlockStatement> body_;
};

// Arrow function expression node
class ArrowFunctionExpression : public Expression {
public:
    ArrowFunctionExpression(std::vector<std::unique_ptr<Parameter>> params, std::unique_ptr<Expression> body, const TokenPosition& position);
    virtual ~ArrowFunctionExpression() = default;

    const std::vector<std::unique_ptr<Parameter>>& params() const { return params_; }
    void setParams(std::vector<std::unique_ptr<Parameter>> params) { params_ = std::move(params); }

    Expression* body() const { return body_.get(); }
    void setBody(std::unique_ptr<Expression> body) { body_ = std::move(body); }

    virtual std::string toString() const override;
    virtual void accept(ASTVisitor& visitor) override;

private:
    std::vector<std::unique_ptr<Parameter>> params_;
    std::unique_ptr<Expression> body_;
};

// Class expression node
class ClassExpression : public Expression {
public:
    ClassExpression(std::unique_ptr<Identifier> id, std::unique_ptr<Expression> superClass, std::unique_ptr<BlockStatement> body, const TokenPosition& position);
    virtual ~ClassExpression() = default;

    Identifier* id() const { return id_.get(); }
    void setId(std::unique_ptr<Identifier> id) { id_ = std::move(id); }

    Expression* superClass() const { return superClass_.get(); }
    void setSuperClass(std::unique_ptr<Expression> superClass) { superClass_ = std::move(superClass); }

    BlockStatement* body() const { return body_.get(); }
    void setBody(std::unique_ptr<BlockStatement> body) { body_ = std::move(body); }

    virtual std::string toString() const override;
    virtual void accept(ASTVisitor& visitor) override;

private:
    std::unique_ptr<Identifier> id_;
    std::unique_ptr<Expression> superClass_;
    std::unique_ptr<BlockStatement> body_;
};

// Template literal node
class TemplateLiteral : public Expression => {
public:
    TemplateLiteral(std::vector<std::unique_ptr<TemplateElement>> quasis, std::vector<std::unique_ptr<Expression>> expressions, const TokenPosition& position);
    virtual ~TemplateLiteral() = default;

    const std::vector<std::unique_ptr<TemplateElement>>& quasis() const { return quasis_; }
    void setQuasis(std::vector<std::unique_ptr<TemplateElement>> quasis) { quasis_ = std::move(quasis); }

    const std::vector<std::unique_ptr<Expression>>& expressions() const { return expressions_; }
    void setExpressions(std::vector<std::unique_ptr<Expression>> expressions) { expressions_ = std::move(expressions); }

    virtual std::string toString() const override;
    virtual void accept(ASTVisitor& visitor) override;

private:
    std::vector<std::unique_ptr<TemplateElement>> quasis_;
    std::vector<std::unique_ptr<Expression>> expressions_;
};

// Tagged template literal node
class TaggedTemplateExpression : public Expression {
public:
    TaggedTemplateExpression(std::unique_ptr<Expression> tag, std::unique_ptr<TemplateLiteral> quasi, const TokenPosition& position);
    virtual ~TaggedTemplateExpression() = default;

    Expression* tag() const { return tag_.get(); }
    void setTag(std::unique_ptr<Expression> tag) { tag_ = std::move(tag); }

    TemplateLiteral* quasi() const { return quasi_.get(); }
    void setQuasi(std::unique_ptr<TemplateLiteral> quasi) { quasi_ = std::move(quasi); }

    virtual std::string toString() const override;
    virtual void accept(ASTVisitor& visitor) override;

private:
    std::unique_ptr<Expression> tag_;
    std::unique_ptr<TemplateLiteral> quasi_;
};

// Sequence expression node
class SequenceExpression : public Expression {
public:
    SequenceExpression(std::vector<std::unique_ptr<Expression>> expressions, const TokenPosition& position);
    virtual ~SequenceExpression() = default;

    const std::vector<std::unique_ptr<Expression>>& expressions() const { return expressions_; }
    void setExpressions(std::vector<std::unique_ptr<Expression>> expressions) { expressions_ = std::move(expressions); }

    virtual std::string toString() const override;
    virtual void accept(ASTVisitor& visitor) override;

private:
    std::vector<std::unique_ptr<Expression>> expressions_;
};

// Assignment expression node
class AssignmentExpression : public Expression {
public:
    AssignmentExpression(OperatorType op, std::unique_ptr<Expression> left, std::unique_ptr<Expression> right, const TokenPosition& position);
    virtual ~AssignmentExpression() = default;

    OperatorType operatorType() const { return operatorType_; }
    void setOperatorType(OperatorType op) { operatorType_ = op; }

    Expression* left() const { return left_.get(); }
    void setLeft(std::unique_ptr<Expression> left) { left_ = std::move(left); }

    Expression* right() const { return right_.get(); }
    void setRight(std::unique_ptr<Expression> right) { right_ = std::move(right); }

    virtual std::string toString() const override;
    virtual void accept(ASTVisitor& visitor) override;

private:
    OperatorType operatorType_;
    std::unique_ptr<Expression> left_;
    std::unique_ptr<Expression> right_;
};

// Update expression node
class UpdateExpression : public Expression {
public:
    UpdateExpression(OperatorType op, std::unique_ptr<Expression> argument, bool prefix, const TokenPosition& position);
    virtual ~UpdateExpression() = default;

    OperatorType operatorType() const { return operatorType_; }
    void setOperatorType(OperatorType op) { operatorType_ = op; }

    Expression* argument() const { return argument_.get(); }
    void setArgument(std::unique_ptr<Expression> argument) { argument_ = std::move(argument); }

    bool prefix() const { return prefix_; }
    void setPrefix(bool prefix) { prefix_ = prefix; }

    virtual std::string toString() const override;
    virtual void accept(ASTVisitor& visitor) override;

private:
    OperatorType operatorType_;
    std::unique_ptr<Expression> argument_;
    bool prefix_;
};

// Logical expression node
class LogicalExpression : public Expression {
public:
    LogicalExpression(OperatorType op, std::unique_ptr<Expression> left, std::unique_ptr<Expression> right, const TokenPosition& position);
    virtual ~LogicalExpression() = default;

    OperatorType operatorType() const { return operatorType_; }
    void setOperatorType(OperatorType op) { operatorType_ = op; }

    Expression* left() const { return left_.get(); }
    void setLeft(std::unique_ptr<Expression> left) { left_ = std::move(left); }

    Expression* right() const { return right_.get(); }
    void setRight(std::unique_ptr<Expression> right) { right_ = std::move(right); }

    virtual std::string toString() const override;
    virtual void accept(ASTVisitor& visitor) override;

private:
    OperatorType operatorType_;
    std::unique_ptr<Expression> left_;
    std::unique_ptr<Expression> right_;
};

// New expression node
class NewExpression : public Expression {
public:
    NewExpression(std::unique_ptr<Expression> callee, std::vector<std::unique_ptr<Expression>> arguments, const TokenPosition& position);
    virtual ~NewExpression() = default;

    Expression* callee() const { return callee_.get(); }
    void setCallee(std::unique_ptr<Expression> callee) { callee_ = std::move(callee); }

    const std::vector<std::unique_ptr<Expression>>& arguments() const { return arguments_; }
    void setArguments(std::vector<std::unique_ptr<Expression>> arguments) { arguments_ = std::move(arguments); }

    virtual std::string toString() const override;
    virtual void accept(ASTVisitor& visitor) override;

private:
    std::unique_ptr<Expression> callee_;
    std::vector<std::unique_ptr<Expression>> arguments_;
};

// For statement node
class ForStatement : public Statement {
public:
    ForStatement(std::unique_ptr<Expression> init, std::unique_ptr<Expression> test, std::unique_ptr<Expression> update, std::unique_ptr<Statement> body, const TokenPosition& position);
    virtual ~ForStatement() = default;

    Expression* init() const { return init_.get(); }
    void setInit(std::unique_ptr<Expression> init) { init_ = std::move(init); }

    Expression* test() const { return test_.get(); }
    void setTest(std::unique_ptr<Expression> test) { test_ = std::move(test); }

    Expression* update() const { return update_.get(); }
    void setUpdate(std::unique_ptr<Expression> update) { update_ = std::move(update); }

    Statement* body() const { return body_.get(); }
    void setBody(std::unique_ptr<Statement> body) { body_ = std::move(body); }

    virtual std::string toString() const override;
    virtual void accept(ASTVisitor& visitor) override;

private:
    std::unique_ptr<Expression> init_;
    std::unique_ptr<Expression> test_;
    std::unique_ptr<Expression> update_;
    std::unique_ptr<Statement> body_;
};

// While statement node
class WhileStatement : public Statement {
public:
    WhileStatement(std::unique_ptr<Expression> test, std::unique_ptr<Statement> body, const TokenPosition& position);
    virtual ~WhileStatement() = default;

    Expression* test() const { return test_.get(); }
    void setTest(std::unique_ptr<Expression> test) { test_ = std::move(test); }

    Statement* body() const { return body_.get(); }
    void setBody(std::unique_ptr<Statement> body) { body_ = std::move(body); }

    virtual std::string toString() const override;
    virtual void accept(ASTVisitor& visitor) override;

private:
    std::unique_ptr<Expression> test_;
    std::unique_ptr<Statement> body_;
};

// Do while statement node
class DoWhileStatement : public Statement {
public:
    DoWhileStatement(std::unique_ptr<Statement> body, std::unique_ptr<Expression> test, const TokenPosition& position);
    virtual ~DoWhileStatement() = default;

    Statement* body() const { return body_.get(); }
    void setBody(std::unique_ptr<Statement> body) { body_ = std::move(body); }

    Expression* test() const { return test_.get(); }
    void setTest(std::unique_ptr<Expression> test) { test_ = std::move(test); }

    virtual std::string toString() const override;
    virtual void accept(ASTVisitor& visitor) override;

private:
    std::unique_ptr<Statement> body_;
    std::unique_ptr<Expression> test_;
};

// For in statement node
class ForInStatement : public Statement {
public:
    ForInStatement(std::unique_ptr<Expression> left, std::unique_ptr<Expression> right, std::unique_ptr<Statement> body, const TokenPosition& position);
    virtual ~ForInStatement() = default;

    Expression* left() const { return left_.get(); }
    void setLeft(std::unique_ptr<Expression> left) { left_ = std::move(left); }

    Expression* right() const { return right_.get(); }
    void setRight(std::unique_ptr<Expression> right) { right_ = std::move(right); }

    Statement* body() const { return body_.get(); }
    void setBody(std::unique_ptr<Statement> body) { body_ = std::move(body); }

    virtual std::string toString() const override;
    virtual void accept(ASTVisitor& visitor) override;

private:
    std::unique_ptr<Expression> left_;
    std::unique_ptr<Expression> right_;
    std::unique_ptr<Statement> body_;
};

// For of statement node
class ForOfStatement : public Statement {
public:
    ForOfStatement(std::unique_ptr<Expression> left, std::unique_ptr<Expression> right, std::unique_ptr<Statement> body, const TokenPosition& position);
    virtual ~ForOfStatement() = default;

    Expression* left() const { return left_.get(); }
    void setLeft(std::unique_ptr<Expression> left) { left_ = std::move(left); }

    Expression* right() const { return right_.get(); }
    void setRight(std::unique_ptr<Expression> right) { right_ = std::move(right); }

    Statement* body() const { return body_.get(); }
    void setBody(std::unique_ptr<Statement> body) { body_ = std::move(body); }

    virtual std::string toString() const override;
    virtual void accept(ASTVisitor& visitor) override;

private:
    std::unique_ptr<Expression> left_;
    std::unique_ptr<Expression> right_;
    std::unique_ptr<Statement> body_;
};

// If statement node
class IfStatement : public Statement {
public:
    IfStatement(std::unique_ptr<Expression> test, std::unique_ptr<Statement> consequent, std::unique_ptr<Statement> alternate, const TokenPosition& position);
    virtual ~IfStatement() = default;

    Expression* test() const { return test_.get(); }
    void setTest(std::unique_ptr<Expression> test) { test_ = std::move(test); }

    Statement* consequent() const { return consequent_.get(); }
    void setConsequent(std::unique_ptr<Statement> consequent) { consequent_ = std::move(consequent); }

    Statement* alternate() const { return alternate_.get(); }
    void setAlternate(std::unique_ptr<Statement> alternate) { alternate_ = std::move(alternate); }

    virtual std::string toString() const override;
    virtual void accept(ASTVisitor& visitor) override;

private:
    std::unique_ptr<Expression> test_;
    std::unique_ptr<Statement> consequent_;
    std::unique_ptr<Statement> alternate_;
};

// Switch statement node
class SwitchStatement : public Statement {
public:
    SwitchStatement(std::unique_ptr<Expression> discriminant, std::vector<std::unique_ptr<CaseClause>> cases, const TokenPosition& position);
    virtual ~SwitchStatement() = default;

    Expression* discriminant() const { return discriminant_.get(); }
    void setDiscriminant(std::unique_ptr<Expression> discriminant) { discriminant_ = std::move(discriminant); }

    const std::vector<std::unique_ptr<CaseClause>>& cases() const { return cases_; }
    void setCases(std::vector<std::unique_ptr<CaseClause>> cases) { cases_ = std::move(cases); }

    virtual std::string toString() const override;
    virtual void accept(ASTVisitor& visitor) override;

private:
    std::unique_ptr<Expression> discriminant_;
    std::vector<std::unique_ptr<CaseClause>> cases_;
};

// Try statement node
class TryStatement : public Statement {
public:
    TryStatement(std::unique_ptr<BlockStatement> block, std::unique_ptr<CatchClause> handler, std::unique_ptr<BlockStatement> finalizer, const TokenPosition& position);
    virtual ~TryStatement() = default;

    BlockStatement* block() const { return block_.get(); }
    void setBlock(std::unique_ptr<BlockStatement> block) { block_ = std::move(block); }

    CatchClause* handler() const { return handler_.get(); }
    void setHandler(std::unique_ptr<CatchClause> handler) { handler_ = std::move(handler); }

    BlockStatement* finalizer() const { return finalizer_.get(); }
    void setFinalizer(std::unique_ptr<BlockStatement> finalizer) { finalizer_ = std::move(finalizer); }

    virtual std::string toString() const override;
    virtual void accept(ASTVisitor& visitor) override;

private:
    std::unique_ptr<BlockStatement> block_;
    std::unique_ptr<CatchClause> handler_;
    std::unique_ptr<BlockStatement> finalizer_;
};

// Throw statement node
class ThrowStatement : public Statement {
public:
    ThrowStatement(std::unique_ptr<Expression> argument, const TokenPosition& position);
    virtual ~ThrowStatement() = default;

    Expression* argument() const { return argument_.get(); }
    void setArgument(std::unique_ptr<Expression> argument) { argument_ = std::move(argument); }

    virtual std::string toString() const override;
    virtual void accept(ASTVisitor& visitor) override;

private:
    std::unique_ptr<Expression> argument_;
};

// Return statement node
class ReturnStatement : public Statement {
public:
    ReturnStatement(std::unique_ptr<Expression> argument, const TokenPosition& position);
    virtual ~ReturnStatement() = default;

    Expression* argument() const { return argument_.get(); }
    void setArgument(std::unique_ptr<Expression> argument) { argument_ = std::move(argument); }

    virtual std::string toString() const override;
    virtual void accept(ASTVisitor& visitor) override;

private:
    std::unique_ptr<Expression> argument_;
};

// Break statement node
class BreakStatement : public Statement {
public:
    BreakStatement(std::unique_ptr<Identifier> label, const TokenPosition& position);
    virtual ~BreakStatement() = default;

    Identifier* label() const { return label_.get(); }
    void setLabel(std::unique_ptr<Identifier> label) { label_ = std::move(label); }

    virtual std::string toString() const override;
    virtual void accept(ASTVisitor& visitor) override;

private:
    std::unique_ptr<Identifier> label_;
};

// Continue statement node
class ContinueStatement : public Statement {
public:
    ContinueStatement(std::unique_ptr<Identifier> label, const TokenPosition& position);
    virtual ~ContinueStatement() = default;

    Identifier* label() const { return label_.get(); }
    void setLabel(std::unique_ptr<Identifier> label) { label_ = std::move(label); }

    virtual std::string toString() const override;
    virtual void accept(ASTVisitor& visitor) override;

private:
    std::unique_ptr<Identifier> label_;
};

// Labeled statement node
class LabeledStatement : public Statement {
public:
    LabeledStatement(std::unique_ptr<Identifier> label, std::unique_ptr<Statement> body, const TokenPosition& position);
    virtual ~LabeledStatement() = default;

    Identifier* label() const { return label_.get(); }
    void setLabel(std::unique_ptr<Identifier> label) { label_ = std::move(label); }

    Statement* body() const { return body_.get(); }
    void setBody(std::unique_ptr<Statement> body) { body_ = std::move(body); }

    virtual std::string toString() const override;
    virtual void accept(ASTVisitor& visitor) override;

private:
    std::unique_ptr<Identifier> label_;
    std::unique_ptr<Statement> body_;
};

// With statement node
class WithStatement : public Statement {
public:
    WithStatement(std::unique_ptr<Expression> object, std::unique_ptr<Statement> body, const TokenPosition& position);
    virtual ~WithStatement() = default;

    Expression* object() const { return object_.get(); }
    void setObject(std::unique_ptr<Expression> object) { object_ = std::move(object); }

    Statement* body() const { return body_.get(); }
    void setBody(std::unique_ptr<Statement> body) { body_ = std::move(body); }

    virtual std::string toString() const override;
    virtual void accept(ASTVisitor& visitor) override;

private:
    std::unique_ptr<Expression> object_;
    std::unique_ptr<Statement> body_;
};

// Block statement node
class BlockStatement : public Statement {
public:
    BlockStatement(std::vector<std::unique_ptr<Statement>> body, const TokenPosition& position);
    virtual ~BlockStatement() = default;

    const std::vector<std::unique_ptr<Statement>>& body() const { return body_; }
    void setBody(std::vector<std::unique_ptr<Statement>> body) { body_ = std::move(body); }

    virtual std::string toString() const override;
    virtual void accept(ASTVisitor& visitor) override;

private:
    std::vector<std::unique_ptr<Statement>> body_;
};

// Variable declaration node
class VariableDeclaration : public Declaration {
public:
    VariableDeclaration(const std::string& kind, std::vector<std::unique_ptr<VariableDeclarator>> declarations, const TokenPosition& position);
    virtual ~VariableDeclaration() = default;

    const std::string& kind() const { return kind_; }
    void setKind(const std::string& kind) { kind_ = kind; }

    const std::vector<std::unique_ptr<VariableDeclarator>>& declarations() const { return declarations_; }
    void setDeclarations(std::vector<std::unique_ptr<VariableDeclarator>> declarations) { declarations_ = std::move(declarations); }

    virtual std::string toString() const override;
    virtual void accept(ASTVisitor& visitor) override;

private:
    std::string kind_;
    std::vector<std::unique_ptr<VariableDeclarator>> declarations_;
};

// Function declaration node
class FunctionDeclaration : public Declaration {
public:
    FunctionDeclaration(std::unique_ptr<Identifier> id, std::vector<std::unique_ptr<Parameter>> params, std::unique_ptr<BlockStatement> body, const TokenPosition& position);
    virtual ~FunctionDeclaration() = default;

    Identifier* id() const { return id_.get(); }
    void setId(std::unique_ptr<Identifier> id) { id_ = std::move(id); }

    const std::vector<std::unique_ptr<Parameter>>& params() const { return params_; }
    void setParams(std::vector<std::unique_ptr<Parameter>> params) { params_ = std::move(params); }

    BlockStatement* body() const { return body_.get(); }
    void setBody(std::unique_ptr<BlockStatement> body) { body_ = std::move(body); }

    virtual std::string toString() const override;
    virtual void accept(ASTVisitor& visitor) override;

private:
    std::unique_ptr<Identifier> id_;
    std::vector<std::unique_ptr<Parameter>> params_;
    std::unique_ptr<BlockStatement> body_;
};

// Class declaration node
class ClassDeclaration : public Declaration {
public:
    ClassDeclaration(std::unique_ptr<Identifier> id, std::unique_ptr<Expression> superClass, std::unique_ptr<BlockStatement> body, const TokenPosition& position);
    virtual ~ClassDeclaration() = default;

    Identifier* id() const { return id_.get(); }
    void setId(std::unique_ptr<Identifier> id) { id_ = std::move(id); }

    Expression* superClass() const { return superClass_.get(); }
    void setSuperClass(std::unique_ptr<Expression> superClass) { superClass_ = std::move(superClass); }

    BlockStatement* body() const { return body_.get(); }
    void setBody(std::unique_ptr<BlockStatement> body) { body_ = std::move(body); }

    virtual std::string toString() const override;
    virtual void accept(ASTVisitor& visitor) override;

private:
    std::unique_ptr<Identifier> id_;
    std::unique_ptr<Expression> superClass_;
    std::unique_ptr<BlockStatement> body_;
};

// Import declaration node
class ImportDeclaration : public Declaration {
public:
    ImportDeclaration(std::vector<std::unique_ptr<ImportSpecifier>> specifiers, std::unique_ptr<Literal> source, const TokenPosition& position);
    virtual ~ImportDeclaration() = default;

    const std::vector<std::unique_ptr<ImportSpecifier>>& specifiers() const { return specifiers_; }
    void setSpecifiers(std::vector<std::unique_ptr<ImportSpecifier>> specifiers) { specifiers_ = std::move(specifiers); }

    Literal* source() const { return source_.get(); }
    void setSource(std::unique_ptr<Literal> source) { source_ = std::move(source); }

    virtual std::string toString() const override;
    virtual void accept(ASTVisitor& visitor) override;

private:
    std::vector<std::unique_ptr<ImportSpecifier>> specifiers_;
    std::unique_ptr<Literal> source_;
};

// Export declaration node
class ExportDeclaration : public Declaration {
public:
    ExportDeclaration(std::vector<std::unique_ptr<ExportSpecifier>> specifiers, std::unique_ptr<Literal> source, const TokenPosition& position);
    virtual ~ExportDeclaration() = default;

    const std::vector<std::unique_ptr<ExportSpecifier>>& specifiers() const { return specifiers_; }
    void setSpecifiers(std::vector<std::unique_ptr<ExportSpecifier>> specifiers) { specifiers_ = std::move(specifiers); }

    Literal* source() const { return source_.get(); }
    void setSource(std::unique_ptr<Literal> source) { source_ = std::move(source); }

    virtual std::string toString() const override;
    virtual void accept(ASTVisitor& visitor) override;

private:
    std::vector<std::unique_ptr<ExportSpecifier>> specifiers_;
    std::unique_ptr<Literal> source_;
};

// Program node
class Program : public Node {
public:
    Program(std::vector<std::unique_ptr<Statement>> body, const TokenPosition& position);
    virtual ~Program() = default;

    const std::vector<std::unique_ptr<Statement>>& body() const { return body_; }
    void setBody(std::vector<std::unique_ptr<Statement>> body) { body_ = std::move(body); }

    virtual std::string toString() const override;
    virtual void accept(ASTVisitor& visitor) override;

private:
    std::vector<std::unique_ptr<Statement>> body_;
};

// Module node
class Module : public Node {
public:
    Module(std::vector<std::unique_ptr<Statement>> body, const TokenPosition& position);
    virtual ~Module() = default;

    const std::vector<std::unique_ptr<Statement>>& body() const { return body_; }
    void setBody(std::vector<std::unique_ptr<Statement>> body) { body_ = std::move(body); }

    virtual std::string toString() const override;
    virtual void accept(ASTVisitor& visitor) override;

private:
    std::vector<std::unique_ptr<Statement>> body_;
};

// AST visitor
class ASTVisitor {
public:
    virtual ~ASTVisitor() = default;

    virtual void visit(Node& node) = 0;
    virtual void visit(Statement& statement) = 0;
    virtual void visit(Expression& expression) = 0;
    virtual void visit(Declaration& declaration) = 0;
    virtual void visit(Pattern& pattern) = 0;
    virtual void visit(Parameter& parameter) = 0;
    virtual void visit(Property& property) = 0;
    virtual void visit(Element& element) = 0;
    virtual void visit(CaseClause& caseClause) = 0;
    virtual void visit(CatchClause& catchClause) = 0;
    virtual void visit(ImportSpecifier& importSpecifier) = 0;
    virtual void visit(ExportSpecifier& exportSpecifier) = 0;
    virtual void visit(TemplateElement& templateElement) = 0;
    virtual void visit(MetaProperty& metaProperty) = 0;
    virtual void visit(SuperExpression& superExpression) = 0;
    virtual void visit(ThisExpression& thisExpression) = 0;
    virtual void visit(YieldExpression& yieldExpression) = 0;
    virtual void visit(AwaitExpression& awaitExpression) = 0;
    virtual void visit(Identifier& identifier) = 0;
    virtual void visit(Literal& literal) = 0;
    virtual void visit(BinaryExpression& binaryExpression) = 0;
    virtual void visit(UnaryExpression& unaryExpression) = 0;
    virtual void visit(ConditionalExpression& conditionalExpression) = 0;
    virtual void visit(CallExpression& callExpression) = 0;
    virtual void visit(MemberExpression& memberExpression) = 0;
    virtual void visit(ArrayExpression& arrayExpression) = 0;
    virtual void visit(ObjectExpression& objectExpression) = 0;
    virtual void visit(FunctionExpression& functionExpression) = 0;
    virtual void visit(ArrowFunctionExpression& arrowFunctionExpression) = 0;
    virtual void visit(ClassExpression& classExpression) = 0;
    virtual void visit(TemplateLiteral& templateLiteral) = 0;
    virtual void visit(TaggedTemplateExpression& taggedTemplateExpression) = 0;
    virtual void visit(SequenceExpression& sequenceExpression) = 0;
    virtual void visit(AssignmentExpression& assignmentExpression) = 0;
    virtual void visit(UpdateExpression& updateExpression) = 0;
    virtual void visit(LogicalExpression& logicalExpression) = 0;
    virtual void visit(NewExpression& newExpression) = 0;
    virtual void visit(ForStatement& forStatement) = 0;
    virtual void visit(WhileStatement& whileStatement) = 0;
    virtual void visit(DoWhileStatement& doWhileStatement) = 0;
    virtual void visit(ForInStatement& forInStatement) = 0;
    virtual void visit(ForOfStatement& forOfStatement) = 0;
    virtual void visit(IfStatement& ifStatement) = 0;
    virtual void visit(SwitchStatement& switchStatement) = 0;
    virtual void visit(TryStatement& tryStatement) = 0;
    virtual void visit(ThrowStatement& throwStatement) = 0;
    virtual void visit(ReturnStatement& returnStatement) = 0;
    virtual void visit(BreakStatement& breakStatement) = 0;
    virtual void visit(ContinueStatement& continueStatement) = 0;
    virtual void visit(LabeledStatement& labeledStatement) = 0;
    virtual void visit(WithStatement& withStatement) = 0;
    virtual void visit(BlockStatement& blockStatement) = 0;
    virtual void visit(VariableDeclaration& variableDeclaration) = 0;
    virtual void visit(FunctionDeclaration& functionDeclaration) = 0;
    virtual void visit(ClassDeclaration& classDeclaration) = 0;
    virtual void visit(ImportDeclaration& importDeclaration) = 0;
    virtual void visit(ExportDeclaration& exportDeclaration) = 0;
    virtual void visit(Program& program) = 0;
    virtual void visit(Module& module) = 0;
};

// AST class
class AST {
public:
    AST(std::unique_ptr<Node> root);
    ~AST();

    Node* root() const { return root_.get(); }
    void setRoot(std::unique_ptr<Node> root) { root_ = std::move(root); }

    std::string toString() const;
    void accept(ASTVisitor& visitor);

    bool operator==(const AST& other) const;
    bool operator!=(const AST& other) const { return !(*this == other); }

private:
    std::unique_ptr<Node> root_;
};

} // namespace js
