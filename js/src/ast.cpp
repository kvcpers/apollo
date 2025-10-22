#include "js/ast.h"

namespace js {

// Base Node implementation
Node::Node(NodeType type) : node_type(type) {}

Node::~Node() {}

NodeType Node::get_type() const {
    return node_type;
}

// Statement implementations
Statement::Statement(NodeType type) : Node(type) {}

BlockStatement::BlockStatement() : Statement(NodeType::BlockStatement) {}

BlockStatement::~BlockStatement() {}

void BlockStatement::add_statement(std::shared_ptr<Statement> stmt) {
    statements.push_back(stmt);
}

std::vector<std::shared_ptr<Statement>> BlockStatement::get_statements() const {
    return statements;
}

// Expression implementations
Expression::Expression(NodeType type) : Node(type) {}

BinaryExpression::BinaryExpression(std::shared_ptr<Expression> left, 
                                   const std::string& op, 
                                   std::shared_ptr<Expression> right)
    : Expression(NodeType::BinaryExpression), left_operand(left), operator_(op), right_operand(right) {}

BinaryExpression::~BinaryExpression() {}

std::shared_ptr<Expression> BinaryExpression::get_left() const {
    return left_operand;
}

std::string BinaryExpression::get_operator() const {
    return operator_;
}

std::shared_ptr<Expression> BinaryExpression::get_right() const {
    return right_operand;
}

// Literal implementations
Literal::Literal(NodeType type) : Expression(type) {}

StringLiteral::StringLiteral(const std::string& value) 
    : Literal(NodeType::StringLiteral), value_(value) {}

StringLiteral::~StringLiteral() {}

std::string StringLiteral::get_value() const {
    return value_;
}

NumberLiteral::NumberLiteral(double value) 
    : Literal(NodeType::NumberLiteral), value_(value) {}

NumberLiteral::~NumberLiteral() {}

double NumberLiteral::get_value() const {
    return value_;
}

BooleanLiteral::BooleanLiteral(bool value) 
    : Literal(NodeType::BooleanLiteral), value_(value) {}

BooleanLiteral::~BooleanLiteral() {}

bool BooleanLiteral::get_value() const {
    return value_;
}

// Identifier implementation
Identifier::Identifier(const std::string& name) 
    : Expression(NodeType::Identifier), name_(name) {}

Identifier::~Identifier() {}

std::string Identifier::get_name() const {
    return name_;
}

} // namespace js
