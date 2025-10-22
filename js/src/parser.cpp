#include "js/parser.h"
#include "js/ast.h"
#include <algorithm>
#include <stdexcept>

namespace js {

// Parser implementation
Parser::Parser() : source_(), tokenizer_(), tokens_(), position_(0), strictMode_(false), moduleMode_(false) {
    initialize();
}

Parser::Parser(const std::string& source) : source_(source), tokenizer_(source), tokens_(), position_(0), strictMode_(false), moduleMode_(false) {
    initialize();
}

Parser::~Parser() = default;

void Parser::setSource(const std::string& source) {
    source_ = source;
    tokenizer_.setSource(source);
    reset();
}

std::unique_ptr<AST> Parser::parse() {
    if (moduleMode_) {
        return parseModule();
    } else {
        return parseScript();
    }
}

std::unique_ptr<AST> Parser::parseScript() {
    auto program = parseProgram();
    return std::make_unique<AST>(std::move(program));
}

std::unique_ptr<AST> Parser::parseModule() {
    auto module = parseModule();
    return std::make_unique<AST>(std::move(module));
}

std::unique_ptr<AST> Parser::parseExpression() {
    auto expression = parseExpression();
    return std::make_unique<AST>(std::move(expression));
}

std::unique_ptr<AST> Parser::parseStatement() {
    auto statement = parseStatement();
    return std::make_unique<AST>(std::move(statement));
}

std::unique_ptr<AST> Parser::parseDeclaration() {
    auto declaration = parseDeclaration();
    return std::make_unique<AST>(std::move(declaration));
}

std::unique_ptr<Program> Parser::parseProgram() {
    TokenPosition start = getCurrentPosition();
    std::vector<std::unique_ptr<Statement>> body;
    
    while (hasMoreTokens() && !isToken(TokenType::EndOfFile)) {
        if (isToken(TokenType::Semicolon)) {
            advance(); // Skip empty statement
        } else {
            auto statement = parseStatement();
            body.push_back(std::move(statement));
        }
    }
    
    TokenPosition end = getCurrentPosition();
    return std::make_unique<Program>(std::move(body), TokenPosition(start, end));
}

std::unique_ptr<Module> Parser::parseModule() {
    TokenPosition start = getCurrentPosition();
    std::vector<std::unique_ptr<Statement>> body;
    
    while (hasMoreTokens() && !isToken(TokenType::EndOfFile)) {
        if (isToken(TokenType::Semicolon)) {
            advance(); // Skip empty statement
        } else {
            auto statement = parseStatement();
            body.push_back(std::move(statement));
        }
    }
    
    TokenPosition end = getCurrentPosition();
    return std::make_unique<Module>(std::move(body), TokenPosition(start, end));
}

std::unique_ptr<Statement> Parser::parseStatement() {
    TokenPosition start = getCurrentPosition();
    
    if (isToken(TokenType::LeftBrace)) {
        return parseBlockStatement();
    } else if (isKeyword("var") || isKeyword("let") || isKeyword("const")) {
        return parseVariableStatement();
    } else if (isKeyword("function")) {
        return parseFunctionStatement();
    } else if (isKeyword("class")) {
        return parseClassStatement();
    } else if (isKeyword("if")) {
        return parseIfStatement();
    } else if (isKeyword("switch")) {
        return parseSwitchStatement();
    } else if (isKeyword("for")) {
        return parseForStatement();
    } else if (isKeyword("while")) {
        return parseWhileStatement();
    } else if (isKeyword("do")) {
        return parseDoWhileStatement();
    } else if (isKeyword("try")) {
        return parseTryStatement();
    } else if (isKeyword("throw")) {
        return parseThrowStatement();
    } else if (isKeyword("return")) {
        return parseReturnStatement();
    } else if (isKeyword("break")) {
        return parseBreakStatement();
    } else if (isKeyword("continue")) {
        return parseContinueStatement();
    } else if (isKeyword("with")) {
        return parseWithStatement();
    } else if (isKeyword("debugger")) {
        return parseDebuggerStatement();
    } else {
        // Expression statement
        auto expression = parseExpression();
        skipSemicolon();
        return std::make_unique<ExpressionStatement>(std::move(expression), TokenPosition(start, getCurrentPosition()));
    }
}

std::unique_ptr<Statement> Parser::parseBlockStatement() {
    TokenPosition start = getCurrentPosition();
    expect(TokenType::LeftBrace);
    
    std::vector<std::unique_ptr<Statement>> body;
    while (!isToken(TokenType::RightBrace) && hasMoreTokens()) {
        if (isToken(TokenType::Semicolon)) {
            advance(); // Skip empty statement
        } else {
            auto statement = parseStatement();
            body.push_back(std::move(statement));
        }
    }
    
    expect(TokenType::RightBrace);
    TokenPosition end = getCurrentPosition();
    return std::make_unique<BlockStatement>(std::move(body), TokenPosition(start, end));
}

std::unique_ptr<Statement> Parser::parseVariableStatement() {
    TokenPosition start = getCurrentPosition();
    auto declaration = parseVariableDeclaration();
    skipSemicolon();
    TokenPosition end = getCurrentPosition();
    return std::make_unique<VariableStatement>(std::move(declaration), TokenPosition(start, end));
}

std::unique_ptr<Statement> Parser::parseFunctionStatement() {
    TokenPosition start = getCurrentPosition();
    auto declaration = parseFunctionDeclaration();
    skipSemicolon();
    TokenPosition end = getCurrentPosition();
    return std::make_unique<FunctionStatement>(std::move(declaration), TokenPosition(start, end));
}

std::unique_ptr<Statement> Parser::parseClassStatement() {
    TokenPosition start = getCurrentPosition();
    auto declaration = parseClassDeclaration();
    skipSemicolon();
    TokenPosition end = getCurrentPosition();
    return std::make_unique<ClassStatement>(std::move(declaration), TokenPosition(start, end));
}

std::unique_ptr<Statement> Parser::parseIfStatement() {
    TokenPosition start = getCurrentPosition();
    expectKeyword("if");
    expect(TokenType::LeftParen);
    auto test = parseExpression();
    expect(TokenType::RightParen);
    auto consequent = parseStatement();
    std::unique_ptr<Statement> alternate = nullptr;
    if (optionalKeyword("else")) {
        alternate = parseStatement();
    }
    TokenPosition end = getCurrentPosition();
    return std::make_unique<IfStatement>(std::move(test), std::move(consequent), std::move(alternate), TokenPosition(start, end));
}

std::unique_ptr<Statement> Parser::parseSwitchStatement() {
    TokenPosition start = getCurrentPosition();
    expectKeyword("switch");
    expect(TokenType::LeftParen);
    auto discriminant = parseExpression();
    expect(TokenType::RightParen);
    expect(TokenType::LeftBrace);
    
    std::vector<std::unique_ptr<CaseClause>> cases;
    while (!isToken(TokenType::RightBrace) && hasMoreTokens()) {
        auto caseClause = parseCaseClause();
        cases.push_back(std::move(caseClause));
    }
    
    expect(TokenType::RightBrace);
    TokenPosition end = getCurrentPosition();
    return std::make_unique<SwitchStatement>(std::move(discriminant), std::move(cases), TokenPosition(start, end));
}

std::unique_ptr<Statement> Parser::parseForStatement() {
    TokenPosition start = getCurrentPosition();
    expectKeyword("for");
    expect(TokenType::LeftParen);
    
    std::unique_ptr<Expression> init = nullptr;
    std::unique_ptr<Expression> test = nullptr;
    std::unique_ptr<Expression> update = nullptr;
    
    if (!isToken(TokenType::Semicolon)) {
        if (isKeyword("var") || isKeyword("let") || isKeyword("const")) {
            // Variable declaration
            init = parseVariableDeclaration();
        } else {
            // Expression
            init = parseExpression();
        }
    }
    expect(TokenType::Semicolon);
    
    if (!isToken(TokenType::Semicolon)) {
        test = parseExpression();
    }
    expect(TokenType::Semicolon);
    
    if (!isToken(TokenType::RightParen)) {
        update = parseExpression();
    }
    expect(TokenType::RightParen);
    
    auto body = parseStatement();
    TokenPosition end = getCurrentPosition();
    return std::make_unique<ForStatement>(std::move(init), std::move(test), std::move(update), std::move(body), TokenPosition(start, end));
}

std::unique_ptr<Statement> Parser::parseWhileStatement() {
    TokenPosition start = getCurrentPosition();
    expectKeyword("while");
    expect(TokenType::LeftParen);
    auto test = parseExpression();
    expect(TokenType::RightParen);
    auto body = parseStatement();
    TokenPosition end = getCurrentPosition();
    return std::make_unique<WhileStatement>(std::move(test), std::move(body), TokenPosition(start, end));
}

std::unique_ptr<Statement> Parser::parseDoWhileStatement() {
    TokenPosition start = getCurrentPosition();
    expectKeyword("do");
    auto body = parseStatement();
    expectKeyword("while");
    expect(TokenType::LeftParen);
    auto test = parseExpression();
    expect(TokenType::RightParen);
    skipSemicolon();
    TokenPosition end = getCurrentPosition();
    return std::make_unique<DoWhileStatement>(std::move(body), std::move(test), TokenPosition(start, end));
}

std::unique_ptr<Statement> Parser::parseForInStatement() {
    TokenPosition start = getCurrentPosition();
    expectKeyword("for");
    expect(TokenType::LeftParen);
    
    std::unique_ptr<Expression> left = nullptr;
    if (isKeyword("var") || isKeyword("let") || isKeyword("const")) {
        left = parseVariableDeclaration();
    } else {
        left = parseExpression();
    }
    
    expectKeyword("in");
    auto right = parseExpression();
    expect(TokenType::RightParen);
    auto body = parseStatement();
    TokenPosition end = getCurrentPosition();
    return std::make_unique<ForInStatement>(std::move(left), std::move(right), std::move(body), TokenPosition(start, end));
}

std::unique_ptr<Statement> Parser::parseForOfStatement() {
    TokenPosition start = getCurrentPosition();
    expectKeyword("for");
    expect(TokenType::LeftParen);
    
    std::unique_ptr<Expression> left = nullptr;
    if (isKeyword("var") || isKeyword("let") || isKeyword("const")) {
        left = parseVariableDeclaration();
    } else {
        left = parseExpression();
    }
    
    expectKeyword("of");
    auto right = parseExpression();
    expect(TokenType::RightParen);
    auto body = parseStatement();
    TokenPosition end = getCurrentPosition();
    return std::make_unique<ForOfStatement>(std::move(left), std::move(right), std::move(body), TokenPosition(start, end));
}

std::unique_ptr<Statement> Parser::parseTryStatement() {
    TokenPosition start = getCurrentPosition();
    expectKeyword("try");
    auto block = parseBlockStatement();
    
    std::unique_ptr<CatchClause> handler = nullptr;
    if (optionalKeyword("catch")) {
        handler = parseCatchClause();
    }
    
    std::unique_ptr<BlockStatement> finalizer = nullptr;
    if (optionalKeyword("finally")) {
        finalizer = parseBlockStatement();
    }
    
    TokenPosition end = getCurrentPosition();
    return std::make_unique<TryStatement>(std::move(block), std::move(handler), std::move(finalizer), TokenPosition(start, end));
}

std::unique_ptr<Statement> Parser::parseThrowStatement() {
    TokenPosition start = getCurrentPosition();
    expectKeyword("throw");
    auto argument = parseExpression();
    skipSemicolon();
    TokenPosition end = getCurrentPosition();
    return std::make_unique<ThrowStatement>(std::move(argument), TokenPosition(start, end));
}

std::unique_ptr<Statement> Parser::parseReturnStatement() {
    TokenPosition start = getCurrentPosition();
    expectKeyword("return");
    std::unique_ptr<Expression> argument = nullptr;
    if (!isToken(TokenType::Semicolon) && !isToken(TokenType::EndOfFile)) {
        argument = parseExpression();
    }
    skipSemicolon();
    TokenPosition end = getCurrentPosition();
    return std::make_unique<ReturnStatement>(std::move(argument), TokenPosition(start, end));
}

std::unique_ptr<Statement> Parser::parseBreakStatement() {
    TokenPosition start = getCurrentPosition();
    expectKeyword("break");
    std::unique_ptr<Identifier> label = nullptr;
    if (!isToken(TokenType::Semicolon) && !isToken(TokenType::EndOfFile)) {
        label = parseIdentifier();
    }
    skipSemicolon();
    TokenPosition end = getCurrentPosition();
    return std::make_unique<BreakStatement>(std::move(label), TokenPosition(start, end));
}

std::unique_ptr<Statement> Parser::parseContinueStatement() {
    TokenPosition start = getCurrentPosition();
    expectKeyword("continue");
    std::unique_ptr<Identifier> label = nullptr;
    if (!isToken(TokenType::Semicolon) && !isToken(TokenType::EndOfFile)) {
        label = parseIdentifier();
    }
    skipSemicolon();
    TokenPosition end = getCurrentPosition();
    return std::make_unique<ContinueStatement>(std::move(label), TokenPosition(start, end));
}

std::unique_ptr<Statement> Parser::parseLabeledStatement() {
    TokenPosition start = getCurrentPosition();
    auto label = parseIdentifier();
    expect(TokenType::Colon);
    auto body = parseStatement();
    TokenPosition end = getCurrentPosition();
    return std::make_unique<LabeledStatement>(std::move(label), std::move(body), TokenPosition(start, end));
}

std::unique_ptr<Statement> Parser::parseWithStatement() {
    TokenPosition start = getCurrentPosition();
    expectKeyword("with");
    expect(TokenType::LeftParen);
    auto object = parseExpression();
    expect(TokenType::RightParen);
    auto body = parseStatement();
    TokenPosition end = getCurrentPosition();
    return std::make_unique<WithStatement>(std::move(object), std::move(body), TokenPosition(start, end));
}

std::unique_ptr<Statement> Parser::parseDebuggerStatement() {
    TokenPosition start = getCurrentPosition();
    expectKeyword("debugger");
    skipSemicolon();
    TokenPosition end = getCurrentPosition();
    return std::make_unique<DebuggerStatement>(TokenPosition(start, end));
}

std::unique_ptr<Expression> Parser::parseExpression() {
    return parseAssignmentExpression();
}

std::unique_ptr<Expression> Parser::parseAssignmentExpression() {
    TokenPosition start = getCurrentPosition();
    auto left = parseConditionalExpression();
    
    if (isAssignmentOperator(currentToken())) {
        auto op = currentToken();
        advance();
        auto right = parseAssignmentExpression();
        return std::make_unique<AssignmentExpression>(getOperatorType(op.value()), std::move(left), std::move(right), TokenPosition(start, getCurrentPosition()));
    }
    
    return left;
}

std::unique_ptr<Expression> Parser::parseConditionalExpression() {
    TokenPosition start = getCurrentPosition();
    auto test = parseLogicalExpression();
    
    if (isToken(TokenType::QuestionMark)) {
        advance();
        auto consequent = parseExpression();
        expect(TokenType::Colon);
        auto alternate = parseExpression();
        return std::make_unique<ConditionalExpression>(std::move(test), std::move(consequent), std::move(alternate), TokenPosition(start, getCurrentPosition()));
    }
    
    return test;
}

std::unique_ptr<Expression> Parser::parseLogicalExpression() {
    TokenPosition start = getCurrentPosition();
    auto left = parseBitwiseExpression();
    
    while (isLogicalOperator(currentToken())) {
        auto op = currentToken();
        advance();
        auto right = parseBitwiseExpression();
        left = std::make_unique<LogicalExpression>(getOperatorType(op.value()), std::move(left), std::move(right), TokenPosition(start, getCurrentPosition()));
    }
    
    return left;
}

std::unique_ptr<Expression> Parser::parseBitwiseExpression() {
    TokenPosition start = getCurrentPosition();
    auto left = parseEqualityExpression();
    
    while (isBitwiseOperator(currentToken())) {
        auto op = currentToken();
        advance();
        auto right = parseEqualityExpression();
        left = std::make_unique<BinaryExpression>(getOperatorType(op.value()), std::move(left), std::move(right), TokenPosition(start, getCurrentPosition()));
    }
    
    return left;
}

std::unique_ptr<Expression> Parser::parseEqualityExpression() {
    TokenPosition start = getCurrentPosition();
    auto left = parseRelationalExpression();
    
    while (isEqualityOperator(currentToken())) {
        auto op = currentToken();
        advance();
        auto right = parseRelationalExpression();
        left = std::make_unique<BinaryExpression>(getOperatorType(op.value()), std::move(left), std::move(right), TokenPosition(start, getCurrentPosition()));
    }
    
    return left;
}

std::unique_ptr<Expression> Parser::parseRelationalExpression() {
    TokenPosition start = getCurrentPosition();
    auto left = parseShiftExpression();
    
    while (isRelationalOperator(currentToken())) {
        auto op = currentToken();
        advance();
        auto right = parseShiftExpression();
        left = std::make_unique<BinaryExpression>(getOperatorType(op.value()), std::move(left), std::move(right), TokenPosition(start, getCurrentPosition()));
    }
    
    return left;
}

std::unique_ptr<Expression> Parser::parseShiftExpression() {
    TokenPosition start = getCurrentPosition();
    auto left = parseAdditiveExpression();
    
    while (isShiftOperator(currentToken())) {
        auto op = currentToken();
        advance();
        auto right = parseAdditiveExpression();
        left = std::make_unique<BinaryExpression>(getOperatorType(op.value()), std::move(left), std::move(right), TokenPosition(start, getCurrentPosition()));
    }
    
    return left;
}

std::unique_ptr<Expression> Parser::parseAdditiveExpression() {
    TokenPosition start = getCurrentPosition();
    auto left = parseMultiplicativeExpression();
    
    while (isAdditiveOperator(currentToken())) {
        auto op = currentToken();
        advance();
        auto right = parseMultiplicativeExpression();
        left = std::make_unique<BinaryExpression>(getOperatorType(op.value()), std::move(left), std::move(right), TokenPosition(start, getCurrentPosition()));
    }
    
    return left;
}

std::unique_ptr<Expression> Parser::parseMultiplicativeExpression() {
    TokenPosition start = getCurrentPosition();
    auto left = parseUnaryExpression();
    
    while (isMultiplicativeOperator(currentToken())) {
        auto op = currentToken();
        advance();
        auto right = parseUnaryExpression();
        left = std::make_unique<BinaryExpression>(getOperatorType(op.value()), std::move(left), std::move(right), TokenPosition(start, getCurrentPosition()));
    }
    
    return left;
}

std::unique_ptr<Expression> Parser::parseUnaryExpression() {
    TokenPosition start = getCurrentPosition();
    
    if (isUnaryOperator(currentToken())) {
        auto op = currentToken();
        advance();
        auto argument = parseUnaryExpression();
        return std::make_unique<UnaryExpression>(getOperatorType(op.value()), std::move(argument), TokenPosition(start, getCurrentPosition()));
    }
    
    if (isUpdateOperator(currentToken())) {
        auto op = currentToken();
        advance();
        auto argument = parseUnaryExpression();
        return std::make_unique<UpdateExpression>(getOperatorType(op.value()), std::move(argument), true, TokenPosition(start, getCurrentPosition()));
    }
    
    return parseUpdateExpression();
}

std::unique_ptr<Expression> Parser::parseUpdateExpression() {
    TokenPosition start = getCurrentPosition();
    
    if (isUpdateOperator(currentToken())) {
        auto op = currentToken();
        advance();
        auto argument = parseUpdateExpression();
        return std::make_unique<UpdateExpression>(getOperatorType(op.value()), std::move(argument), false, TokenPosition(start, getCurrentPosition()));
    }
    
    return parseNewExpression();
}

std::unique_ptr<Expression> Parser::parseNewExpression() {
    TokenPosition start = getCurrentPosition();
    
    if (isKeyword("new")) {
        advance();
        auto callee = parseNewExpression();
        std::vector<std::unique_ptr<Expression>> arguments;
        
        if (isToken(TokenType::LeftParen)) {
            advance();
            if (!isToken(TokenType::RightParen)) {
                arguments = parseArguments();
            }
            expect(TokenType::RightParen);
        }
        
        return std::make_unique<NewExpression>(std::move(callee), std::move(arguments), TokenPosition(start, getCurrentPosition()));
    }
    
    return parseCallExpression();
}

std::unique_ptr<Expression> Parser::parseCallExpression() {
    TokenPosition start = getCurrentPosition();
    auto callee = parseMemberExpression();
    
    while (isToken(TokenType::LeftParen) || isToken(TokenType::LeftBracket) || isToken(TokenType::Dot)) {
        if (isToken(TokenType::LeftParen)) {
            advance();
            std::vector<std::unique_ptr<Expression>> arguments;
            if (!isToken(TokenType::RightParen)) {
                arguments = parseArguments();
            }
            expect(TokenType::RightParen);
            callee = std::make_unique<CallExpression>(std::move(callee), std::move(arguments), TokenPosition(start, getCurrentPosition()));
        } else if (isToken(TokenType::LeftBracket)) {
            advance();
            auto property = parseExpression();
            expect(TokenType::RightBracket);
            callee = std::make_unique<MemberExpression>(std::move(callee), std::move(property), true, TokenPosition(start, getCurrentPosition()));
        } else if (isToken(TokenType::Dot)) {
            advance();
            auto property = parseIdentifier();
            callee = std::make_unique<MemberExpression>(std::move(callee), std::move(property), false, TokenPosition(start, getCurrentPosition()));
        }
    }
    
    return callee;
}

std::unique_ptr<Expression> Parser::parseMemberExpression() {
    TokenPosition start = getCurrentPosition();
    
    if (isKeyword("super")) {
        advance();
        return std::make_unique<SuperExpression>(TokenPosition(start, getCurrentPosition()));
    }
    
    if (isKeyword("this")) {
        advance();
        return std::make_unique<ThisExpression>(TokenPosition(start, getCurrentPosition()));
    }
    
    return parsePrimaryExpression();
}

std::unique_ptr<Expression> Parser::parsePrimaryExpression() {
    TokenPosition start = getCurrentPosition();
    
    if (isToken(TokenType::LeftParen)) {
        advance();
        auto expression = parseExpression();
        expect(TokenType::RightParen);
        return expression;
    }
    
    if (isToken(TokenType::LeftBracket)) {
        return parseArrayExpression();
    }
    
    if (isToken(TokenType::LeftBrace)) {
        return parseObjectExpression();
    }
    
    if (isToken(TokenType::StringLiteral)) {
        return parseStringLiteral();
    }
    
    if (isToken(TokenType::NumberLiteral)) {
        return parseNumericLiteral();
    }
    
    if (isToken(TokenType::BooleanLiteral)) {
        return parseBooleanLiteral();
    }
    
    if (isToken(TokenType::NullLiteral)) {
        return parseNullLiteral();
    }
    
    if (isToken(TokenType::UndefinedLiteral)) {
        return parseUndefinedLiteral();
    }
    
    if (isToken(TokenType::RegExpLiteral)) {
        return parseRegExpLiteral();
    }
    
    if (isToken(TokenType::TemplateLiteral)) {
        return parseTemplateLiteral();
    }
    
    if (isToken(TokenType::BigIntLiteral)) {
        return parseBigIntLiteral();
    }
    
    if (isToken(TokenType::Identifier)) {
        return parseIdentifier();
    }
    
    if (isKeyword("function")) {
        return parseFunctionExpression();
    }
    
    if (isKeyword("class")) {
        return parseClassExpression();
    }
    
    if (isKeyword("yield")) {
        return parseYieldExpression();
    }
    
    if (isKeyword("await")) {
        return parseAwaitExpression();
    }
    
    error("Unexpected token: " + currentToken().toString());
    return nullptr;
}

std::unique_ptr<Expression> Parser::parseArrayExpression() {
    TokenPosition start = getCurrentPosition();
    expect(TokenType::LeftBracket);
    
    std::vector<std::unique_ptr<Element>> elements;
    while (!isToken(TokenType::RightBracket) && hasMoreTokens()) {
        if (isToken(TokenType::Comma)) {
            advance(); // Skip empty element
        } else {
            auto element = parseElement();
            elements.push_back(std::move(element));
        }
        
        if (isToken(TokenType::Comma)) {
            advance();
        }
    }
    
    expect(TokenType::RightBracket);
    TokenPosition end = getCurrentPosition();
    return std::make_unique<ArrayExpression>(std::move(elements), TokenPosition(start, end));
}

std::unique_ptr<Expression> Parser::parseObjectExpression() {
    TokenPosition start = getCurrentPosition();
    expect(TokenType::LeftBrace);
    
    std::vector<std::unique_ptr<Property>> properties;
    while (!isToken(TokenType::RightBrace) && hasMoreTokens()) {
        if (isToken(TokenType::Comma)) {
            advance(); // Skip empty property
        } else {
            auto property = parseProperty();
            properties.push_back(std::move(property));
        }
        
        if (isToken(TokenType::Comma)) {
            advance();
        }
    }
    
    expect(TokenType::RightBrace);
    TokenPosition end = getCurrentPosition();
    return std::make_unique<ObjectExpression>(std::move(properties), TokenPosition(start, end));
}

std::unique_ptr<Expression> Parser::parseFunctionExpression() {
    TokenPosition start = getCurrentPosition();
    expectKeyword("function");
    
    std::unique_ptr<Identifier> id = nullptr;
    if (isToken(TokenType::Identifier)) {
        id = parseIdentifier();
    }
    
    expect(TokenType::LeftParen);
    auto params = parseParameters();
    expect(TokenType::RightParen);
    auto body = parseBlockStatement();
    
    TokenPosition end = getCurrentPosition();
    return std::make_unique<FunctionExpression>(std::move(id), std::move(params), std::move(body), TokenPosition(start, end));
}

std::unique_ptr<Expression> Parser::parseArrowFunctionExpression() {
    TokenPosition start = getCurrentPosition();
    
    std::vector<std::unique_ptr<Parameter>> params;
    if (isToken(TokenType::LeftParen)) {
        advance();
        if (!isToken(TokenType::RightParen)) {
            params = parseParameters();
        }
        expect(TokenType::RightParen);
    } else {
        auto param = parseParameter();
        params.push_back(std::move(param));
    }
    
    expect(TokenType::Arrow);
    auto body = parseExpression();
    
    TokenPosition end = getCurrentPosition();
    return std::make_unique<ArrowFunctionExpression>(std::move(params), std::move(body), TokenPosition(start, end));
}

std::unique_ptr<Expression> Parser::parseClassExpression() {
    TokenPosition start = getCurrentPosition();
    expectKeyword("class");
    
    std::unique_ptr<Identifier> id = nullptr;
    if (isToken(TokenType::Identifier)) {
        id = parseIdentifier();
    }
    
    std::unique_ptr<Expression> superClass = nullptr;
    if (optionalKeyword("extends")) {
        superClass = parseExpression();
    }
    
    auto body = parseBlockStatement();
    
    TokenPosition end = getCurrentPosition();
    return std::make_unique<ClassExpression>(std::move(id), std::move(superClass), std::move(body), TokenPosition(start, end));
}

std::unique_ptr<Expression> Parser::parseTemplateLiteral() {
    TokenPosition start = getCurrentPosition();
    expect(TokenType::TemplateLiteral);
    
    std::vector<std::unique_ptr<TemplateElement>> quasis;
    std::vector<std::unique_ptr<Expression>> expressions;
    
    // This is a simplified implementation
    // In a real implementation, we would parse template elements and expressions
    
    TokenPosition end = getCurrentPosition();
    return std::make_unique<TemplateLiteral>(std::move(quasis), std::move(expressions), TokenPosition(start, end));
}

std::unique_ptr<Expression> Parser::parseTaggedTemplateExpression() {
    TokenPosition start = getCurrentPosition();
    auto tag = parseExpression();
    auto quasi = parseTemplateLiteral();
    
    TokenPosition end = getCurrentPosition();
    return std::make_unique<TaggedTemplateExpression>(std::move(tag), std::move(quasi), TokenPosition(start, end));
}

std::unique_ptr<Expression> Parser::parseSequenceExpression() {
    TokenPosition start = getCurrentPosition();
    std::vector<std::unique_ptr<Expression>> expressions;
    
    auto expression = parseExpression();
    expressions.push_back(std::move(expression));
    
    while (isToken(TokenType::Comma)) {
        advance();
        expression = parseExpression();
        expressions.push_back(std::move(expression));
    }
    
    TokenPosition end = getCurrentPosition();
    return std::make_unique<SequenceExpression>(std::move(expressions), TokenPosition(start, end));
}

std::unique_ptr<Declaration> Parser::parseDeclaration() {
    if (isKeyword("var") || isKeyword("let") || isKeyword("const")) {
        return parseVariableDeclaration();
    } else if (isKeyword("function")) {
        return parseFunctionDeclaration();
    } else if (isKeyword("class")) {
        return parseClassDeclaration();
    } else if (isKeyword("import")) {
        return parseImportDeclaration();
    } else if (isKeyword("export")) {
        return parseExportDeclaration();
    }
    
    error("Expected declaration");
    return nullptr;
}

std::unique_ptr<Declaration> Parser::parseVariableDeclaration() {
    TokenPosition start = getCurrentPosition();
    std::string kind = currentToken().value();
    advance();
    
    std::vector<std::unique_ptr<VariableDeclarator>> declarations;
    do {
        auto declarator = parseVariableDeclarator();
        declarations.push_back(std::move(declarator));
        
        if (isToken(TokenType::Comma)) {
            advance();
        }
    } while (isToken(TokenType::Identifier));
    
    TokenPosition end = getCurrentPosition();
    return std::make_unique<VariableDeclaration>(kind, std::move(declarations), TokenPosition(start, end));
}

std::unique_ptr<Declaration> Parser::parseFunctionDeclaration() {
    TokenPosition start = getCurrentPosition();
    expectKeyword("function");
    auto id = parseIdentifier();
    expect(TokenType::LeftParen);
    auto params = parseParameters();
    expect(TokenType::RightParen);
    auto body = parseBlockStatement();
    
    TokenPosition end = getCurrentPosition();
    return std::make_unique<FunctionDeclaration>(std::move(id), std::move(params), std::move(body), TokenPosition(start, end));
}

std::unique_ptr<Declaration> Parser::parseClassDeclaration() {
    TokenPosition start = getCurrentPosition();
    expectKeyword("class");
    auto id = parseIdentifier();
    
    std::unique_ptr<Expression> superClass = nullptr;
    if (optionalKeyword("extends")) {
        superClass = parseExpression();
    }
    
    auto body = parseBlockStatement();
    
    TokenPosition end = getCurrentPosition();
    return std::make_unique<ClassDeclaration>(std::move(id), std::move(superClass), std::move(body), TokenPosition(start, end));
}

std::unique_ptr<Declaration> Parser::parseImportDeclaration() {
    TokenPosition start = getCurrentPosition();
    expectKeyword("import");
    
    std::vector<std::unique_ptr<ImportSpecifier>> specifiers;
    if (isToken(TokenType::LeftBrace)) {
        advance();
        if (!isToken(TokenType::RightBrace)) {
            specifiers = parseImportSpecifiers();
        }
        expect(TokenType::RightBrace);
    }
    
    expectKeyword("from");
    auto source = parseStringLiteral();
    
    TokenPosition end = getCurrentPosition();
    return std::make_unique<ImportDeclaration>(std::move(specifiers), std::move(source), TokenPosition(start, end));
}

std::unique_ptr<Declaration> Parser::parseExportDeclaration() {
    TokenPosition start = getCurrentPosition();
    expectKeyword("export");
    
    std::vector<std::unique_ptr<ExportSpecifier>> specifiers;
    if (isToken(TokenType::LeftBrace)) {
        advance();
        if (!isToken(TokenType::RightBrace)) {
            specifiers = parseExportSpecifiers();
        }
        expect(TokenType::RightBrace);
    }
    
    expectKeyword("from");
    auto source = parseStringLiteral();
    
    TokenPosition end = getCurrentPosition();
    return std::make_unique<ExportDeclaration>(std::move(specifiers), std::move(source), TokenPosition(start, end));
}

std::unique_ptr<Identifier> Parser::parseIdentifier() {
    TokenPosition start = getCurrentPosition();
    auto token = expect(TokenType::Identifier);
    TokenPosition end = getCurrentPosition();
    return std::make_unique<Identifier>(token.value(), TokenPosition(start, end));
}

std::unique_ptr<Literal> Parser::parseStringLiteral() {
    TokenPosition start = getCurrentPosition();
    auto token = expect(TokenType::StringLiteral);
    TokenPosition end = getCurrentPosition();
    return std::make_unique<StringLiteral>(token.value(), TokenPosition(start, end));
}

std::unique_ptr<Literal> Parser::parseNumericLiteral() {
    TokenPosition start = getCurrentPosition();
    auto token = expect(TokenType::NumberLiteral);
    TokenPosition end = getCurrentPosition();
    return std::make_unique<NumericLiteral>(token.value(), TokenPosition(start, end));
}

std::unique_ptr<Literal> Parser::parseBooleanLiteral() {
    TokenPosition start = getCurrentPosition();
    auto token = expect(TokenType::BooleanLiteral);
    TokenPosition end = getCurrentPosition();
    return std::make_unique<BooleanLiteral>(token.value(), TokenPosition(start, end));
}

std::unique_ptr<Literal> Parser::parseNullLiteral() {
    TokenPosition start = getCurrentPosition();
    auto token = expect(TokenType::NullLiteral);
    TokenPosition end = getCurrentPosition();
    return std::make_unique<NullLiteral>(TokenPosition(start, end));
}

std::unique_ptr<Literal> Parser::parseUndefinedLiteral() {
    TokenPosition start = getCurrentPosition();
    auto token = expect(TokenType::UndefinedLiteral);
    TokenPosition end = getCurrentPosition();
    return std::make_unique<UndefinedLiteral>(TokenPosition(start, end));
}

std::unique_ptr<Literal> Parser::parseRegExpLiteral() {
    TokenPosition start = getCurrentPosition();
    auto token = expect(TokenType::RegExpLiteral);
    TokenPosition end = getCurrentPosition();
    return std::make_unique<RegExpLiteral>(token.value(), TokenPosition(start, end));
}

std::unique_ptr<Literal> Parser::parseBigIntLiteral() {
    TokenPosition start = getCurrentPosition();
    auto token = expect(TokenType::BigIntLiteral);
    TokenPosition end = getCurrentPosition();
    return std::make_unique<BigIntLiteral>(token.value(), TokenPosition(start, end));
}

std::unique_ptr<YieldExpression> Parser::parseYieldExpression() {
    TokenPosition start = getCurrentPosition();
    expectKeyword("yield");
    
    std::unique_ptr<Expression> argument = nullptr;
    if (!isToken(TokenType::Semicolon) && !isToken(TokenType::EndOfFile)) {
        argument = parseExpression();
    }
    
    TokenPosition end = getCurrentPosition();
    return std::make_unique<YieldExpression>(std::move(argument), TokenPosition(start, end));
}

std::unique_ptr<AwaitExpression> Parser::parseAwaitExpression() {
    TokenPosition start = getCurrentPosition();
    expectKeyword("await");
    auto argument = parseExpression();
    
    TokenPosition end = getCurrentPosition();
    return std::make_unique<AwaitExpression>(std::move(argument), TokenPosition(start, end));
}

Token Parser::currentToken() const {
    if (position_ < tokens_.size()) {
        return tokens_[position_];
    }
    return Token(TokenType::EndOfFile, "", TokenPosition());
}

Token Parser::peekToken() const {
    return peekToken(1);
}

Token Parser::peekToken(size_t offset) const {
    if (position_ + offset < tokens_.size()) {
        return tokens_[position_ + offset];
    }
    return Token(TokenType::EndOfFile, "", TokenPosition());
}

void Parser::advance() {
    if (position_ < tokens_.size()) {
        position_++;
    }
}

void Parser::retreat() {
    if (position_ > 0) {
        position_--;
    }
}

bool Parser::hasMoreTokens() const {
    return position_ < tokens_.size();
}

bool Parser::isToken(TokenType type) const {
    return currentToken().type() == type;
}

bool Parser::isToken(const std::string& value) const {
    return currentToken().value() == value;
}

bool Parser::isToken(TokenType type, const std::string& value) const {
    return isToken(type) && isToken(value);
}

bool Parser::isKeyword(const std::string& keyword) const {
    return currentToken().type() == TokenType::Keyword && currentToken().value() == keyword;
}

bool Parser::isOperator(const std::string& op) const {
    return currentToken().type() == TokenType::ArithmeticOperator && currentToken().value() == op;
}

bool Parser::isPunctuation(const std::string& punct) const {
    return currentToken().type() == TokenType::LeftParen && currentToken().value() == punct;
}

Token Parser::expect(TokenType type) {
    if (!isToken(type)) {
        error("Expected token type: " + std::to_string(static_cast<int>(type)));
    }
    auto token = currentToken();
    advance();
    return token;
}

Token Parser::expect(const std::string& value) {
    if (!isToken(value)) {
        error("Expected token value: " + value);
    }
    auto token = currentToken();
    advance();
    return token;
}

Token Parser::expect(TokenType type, const std::string& value) {
    if (!isToken(type, value)) {
        error("Expected token type: " + std::to_string(static_cast<int>(type)) + ", value: " + value);
    }
    auto token = currentToken();
    advance();
    return token;
}

Token Parser::expectKeyword(const std::string& keyword) {
    if (!isKeyword(keyword)) {
        error("Expected keyword: " + keyword);
    }
    auto token = currentToken();
    advance();
    return token;
}

Token Parser::expectOperator(const std::string& op) {
    if (!isOperator(op)) {
        error("Expected operator: " + op);
    }
    auto token = currentToken();
    advance();
    return token;
}

Token Parser::expectPunctuation(const std::string& punct) {
    if (!isPunctuation(punct)) {
        error("Expected punctuation: " + punct);
    }
    auto token = currentToken();
    advance();
    return token;
}

bool Parser::optional(TokenType type) {
    if (isToken(type)) {
        advance();
        return true;
    }
    return false;
}

bool Parser::optional(const std::string& value) {
    if (isToken(value)) {
        advance();
        return true;
    }
    return false;
}

bool Parser::optional(TokenType type, const std::string& value) {
    if (isToken(type, value)) {
        advance();
        return true;
    }
    return false;
}

bool Parser::optionalKeyword(const std::string& keyword) {
    if (isKeyword(keyword)) {
        advance();
        return true;
    }
    return false;
}

bool Parser::optionalOperator(const std::string& op) {
    if (isOperator(op)) {
        advance();
        return true;
    }
    return false;
}

bool Parser::optionalPunctuation(const std::string& punct) {
    if (isPunctuation(punct)) {
        advance();
        return true;
    }
    return false;
}

void Parser::skipSemicolon() {
    if (isToken(TokenType::Semicolon)) {
        advance();
    } else if (needsAutomaticSemicolon()) {
        insertAutomaticSemicolon();
    }
}

void Parser::skipWhitespace() {
    while (hasMoreTokens() && currentToken().isWhitespace()) {
        advance();
    }
}

void Parser::skipComments() {
    while (hasMoreTokens() && currentToken().isComment()) {
        advance();
    }
}

void Parser::error(const std::string& message) {
    errors_.push_back(ParseError(message, currentToken().position()));
}

void Parser::error(const std::string& message, const Token& token) {
    errors_.push_back(ParseError(message, token.position()));
}

void Parser::warning(const std::string& message) {
    warnings_.push_back(ParseWarning(message, currentToken().position()));
}

void Parser::warning(const std::string& message, const Token& token) {
    warnings_.push_back(ParseWarning(message, token.position()));
}

void Parser::reset() {
    position_ = 0;
    tokens_ = tokenizer_.tokenize();
    errors_.clear();
    warnings_.clear();
}

void Parser::initialize() {
    setDefaultModes();
    reset();
}

void Parser::setDefaultModes() {
    strictMode_ = false;
    moduleMode_ = false;
    jsxMode_ = false;
    typescriptMode_ = false;
    flowMode_ = false;
    decoratorMode_ = false;
    asyncIterationMode_ = false;
    optionalChainingMode_ = false;
    nullishCoalescingMode_ = false;
    bigIntMode_ = false;
    dynamicImportMode_ = false;
    topLevelAwaitMode_ = false;
    importMetaMode_ = false;
    privateFieldsMode_ = false;
    classFieldsMode_ = false;
    staticBlocksMode_ = false;
    ergonomicBrandChecksMode_ = false;
    hashbangMode_ = false;
    unicodeMode_ = false;
    regexMode_ = false;
    templateMode_ = false;
    arrowFunctionMode_ = false;
    asyncFunctionMode_ = false;
    generatorFunctionMode_ = false;
    asyncGeneratorFunctionMode_ = false;
    classMode_ = false;
    importMode_ = false;
    exportMode_ = false;
    destructuringMode_ = false;
    spreadMode_ = false;
    restMode_ = false;
    defaultParameterMode_ = false;
    restParameterMode_ = false;
    arrowParameterMode_ = false;
    objectLiteralMode_ = false;
    arrayLiteralMode_ = false;
    functionLiteralMode_ = false;
    classLiteralMode_ = false;
    templateLiteralMode_ = false;
    regExpLiteralMode_ = false;
    bigIntLiteralMode_ = false;
    numericLiteralMode_ = false;
    stringLiteralMode_ = false;
    booleanLiteralMode_ = false;
    nullLiteralMode_ = false;
    undefinedLiteralMode_ = false;
    identifierMode_ = false;
    keywordMode_ = false;
    reservedWordMode_ = false;
    operatorMode_ = false;
    punctuationMode_ = false;
    commentMode_ = false;
    whitespaceMode_ = false;
    newlineMode_ = false;
    tabMode_ = false;
    spaceMode_ = false;
    carriageReturnMode_ = false;
    lineBreakMode_ = false;
    bomMode_ = false;
    shebangMode_ = false;
    docCommentMode_ = false;
    blockCommentMode_ = false;
    lineCommentMode_ = false;
    invalidMode_ = false;
    endOfFileMode_ = false;
}

// Helper methods for operator checking
bool Parser::isAssignmentOperator(const Token& token) const {
    return token.type() == TokenType::AssignmentOperator;
}

bool Parser::isLogicalOperator(const Token& token) const {
    return token.type() == TokenType::LogicalOperator;
}

bool Parser::isBitwiseOperator(const Token& token) const {
    return token.type() == TokenType::BitwiseOperator;
}

bool Parser::isEqualityOperator(const Token& token) const {
    return token.type() == TokenType::ComparisonOperator && 
           (token.value() == "==" || token.value() == "!=" || token.value() == "===" || token.value() == "!==");
}

bool Parser::isRelationalOperator(const Token& token) const {
    return token.type() == TokenType::ComparisonOperator && 
           (token.value() == "<" || token.value() == ">" || token.value() == "<=" || token.value() == ">=");
}

bool Parser::isShiftOperator(const Token& token) const {
    return token.type() == TokenType::BitwiseOperator && 
           (token.value() == "<<" || token.value() == ">>" || token.value() == ">>>");
}

bool Parser::isAdditiveOperator(const Token& token) const {
    return token.type() == TokenType::ArithmeticOperator && 
           (token.value() == "+" || token.value() == "-");
}

bool Parser::isMultiplicativeOperator(const Token& token) const {
    return token.type() == TokenType::ArithmeticOperator && 
           (token.value() == "*" || token.value() == "/" || token.value() == "%");
}

bool Parser::isUnaryOperator(const Token& token) const {
    return token.type() == TokenType::UnaryOperator;
}

bool Parser::isUpdateOperator(const Token& token) const {
    return token.type() == TokenType::ArithmeticOperator && 
           (token.value() == "++" || token.value() == "--");
}

OperatorType Parser::getOperatorType(const std::string& op) const {
    // This is a simplified implementation
    // In a real implementation, we would map all operators to their types
    if (op == "+") return OperatorType::Add;
    if (op == "-") return OperatorType::Subtract;
    if (op == "*") return OperatorType::Multiply;
    if (op == "/") return OperatorType::Divide;
    if (op == "%") return OperatorType::Modulo;
    if (op == "=") return OperatorType::Assign;
    if (op == "==") return OperatorType::Equal;
    if (op == "!=") return OperatorType::NotEqual;
    if (op == "===") return OperatorType::StrictEqual;
    if (op == "!==") return OperatorType::StrictNotEqual;
    if (op == "<") return OperatorType::LessThan;
    if (op == ">") return OperatorType::GreaterThan;
    if (op == "<=") return OperatorType::LessThanOrEqual;
    if (op == ">=") return OperatorType::GreaterThanOrEqual;
    if (op == "&&") return OperatorType::LogicalAnd;
    if (op == "||") return OperatorType::LogicalOr;
    if (op == "!") return OperatorType::LogicalNot;
    if (op == "++") return OperatorType::Increment;
    if (op == "--") return OperatorType::Decrement;
    return OperatorType::Add; // Default fallback
}

TokenPosition Parser::getCurrentPosition() const {
    return currentToken().position();
}

bool Parser::needsAutomaticSemicolon() const {
    // This is a simplified implementation
    // In a real implementation, we would check for ASI rules
    return false;
}

void Parser::insertAutomaticSemicolon() {
    // This is a simplified implementation
    // In a real implementation, we would insert a semicolon token
}

std::vector<std::unique_ptr<Expression>> Parser::parseArguments() {
    std::vector<std::unique_ptr<Expression>> arguments;
    
    auto argument = parseExpression();
    arguments.push_back(std::move(argument));
    
    while (isToken(TokenType::Comma)) {
        advance();
        argument = parseExpression();
        arguments.push_back(std::move(argument));
    }
    
    return arguments;
}

std::vector<std::unique_ptr<Parameter>> Parser::parseParameters() {
    std::vector<std::unique_ptr<Parameter>> parameters;
    
    if (isToken(TokenType::Identifier)) {
        auto parameter = parseParameter();
        parameters.push_back(std::move(parameter));
        
        while (isToken(TokenType::Comma)) {
            advance();
            parameter = parseParameter();
            parameters.push_back(std::move(parameter));
        }
    }
    
    return parameters;
}

std::unique_ptr<Parameter> Parser::parseParameter() {
    TokenPosition start = getCurrentPosition();
    auto identifier = parseIdentifier();
    TokenPosition end = getCurrentPosition();
    return std::make_unique<auto>(std::move(identifier), TokenPosition(start, end));
}

std::vector<std::unique_ptr<ImportSpecifier>> Parser::parseImportSpecifiers() {
    std::vector<std::unique_ptr<ImportSpecifier>> specifiers;
    
    auto specifier = parseImportSpecifier();
    specifiers.push_back(std::move(specifier));
    
    while (isToken(TokenType::Comma)) {
        advance();
        specifier = parseImportSpecifier();
        specifiers.push_back(std::move(specifier));
    }
    
    return specifiers;
}

std::unique_ptr<ImportSpecifier> Parser::parseImportSpecifier() {
    TokenPosition start = getCurrentPosition();
    auto imported = parseIdentifier();
    std::unique_ptr<Identifier> local = nullptr;
    
    if (optionalKeyword("as")) {
        local = parseIdentifier();
    } else {
        local = std::make_unique<Identifier>(imported->name(), imported->position());
    }
    
    TokenPosition end = getCurrentPosition();
    return std::make_unique<ImportSpecifier>(std::move(imported), std::move(local), TokenPosition(start, end));
}

std::vector<std::unique_ptr<ExportSpecifier>> Parser::parseExportSpecifiers() {
    std::vector<std::unique_ptr<ExportSpecifier>> specifiers;
    
    auto specifier = parseExportSpecifier();
    specifiers.push_back(std::move(specifier));
    
    while (isToken(TokenType::Comma)) {
        advance();
        specifier = parseExportSpecifier();
        specifiers.push_back(std::move(specifier));
    }
    
    return specifiers;
}

std::unique_ptr<ExportSpecifier> Parser::parseExportSpecifier() {
    TokenPosition start = getCurrentPosition();
    auto local = parseIdentifier();
    std::unique_ptr<Identifier> exported = nullptr;
    
    if (optionalKeyword("as")) {
        exported = parseIdentifier();
    } else {
        exported = std::make_unique<Identifier>(local->name(), local->position());
    }
    
    TokenPosition end = getCurrentPosition();
    return std::make_unique<ExportSpecifier>(std::move(local), std::move(exported), TokenPosition(start, end));
}

std::unique_ptr<CaseClause> Parser::parseCaseClause() {
    TokenPosition start = getCurrentPosition();
    
    if (isKeyword("case")) {
        advance();
        auto test = parseExpression();
        expect(TokenType::Colon);
        
        std::vector<std::unique_ptr<Statement>> consequent;
        while (!isKeyword("case") && !isKeyword("default") && !isToken(TokenType::RightBrace) && hasMoreTokens()) {
            auto statement = parseStatement();
            consequent.push_back(std::move(statement));
        }
        
        TokenPosition end = getCurrentPosition();
        return std::make_unique<CaseClause>(std::move(test), std::move(consequent), TokenPosition(start, end));
    } else if (isKeyword("default")) {
        advance();
        expect(TokenType::Colon);
        
        std::vector<std::unique_ptr<Statement>> consequent;
        while (!isKeyword("case") && !isKeyword("default") && !isToken(TokenType::RightBrace) && hasMoreTokens()) {
            auto statement = parseStatement();
            consequent.push_back(std::move(statement));
        }
        
        TokenPosition end = getCurrentPosition();
        return std::make_unique<CaseClause>(nullptr, std::move(consequent), TokenPosition(start, end));
    }
    
    error("Expected case or default clause");
    return nullptr;
}

std::unique_ptr<CatchClause> Parser::parseCatchClause() {
    TokenPosition start = getCurrentPosition();
    expect(TokenType::LeftParen);
    
    std::unique_ptr<Identifier> param = nullptr;
    if (isToken(TokenType::Identifier)) {
        param = parseIdentifier();
    }
    
    expect(TokenType::RightParen);
    auto body = parseBlockStatement();
    
    TokenPosition end = getCurrentPosition();
    return std::make_unique<CatchClause>(std::move(param), std::move(body), TokenPosition(start, end));
}

std::unique_ptr<Property> Parser::parseProperty() {
    TokenPosition start = getCurrentPosition();
    
    if (isToken(TokenType::Identifier)) {
        auto key = parseIdentifier();
        std::unique_ptr<Expression> value = nullptr;
        
        if (isToken(TokenType::Colon)) {
            advance();
            value = parseExpression();
        } else {
            value = std::make_unique<Identifier>(key->name(), key->position());
        }
        
        TokenPosition end = getCurrentPosition();
        return std::make_unique<Property>(std::move(key), std::move(value), false, TokenPosition(start, end));
    } else if (isToken(TokenType::StringLiteral)) {
        auto key = parseStringLiteral();
        expect(TokenType::Colon);
        auto value = parseExpression();
        
        TokenPosition end = getCurrentPosition();
        return std::make_unique<Property>(std::move(key), std::move(value), false, TokenPosition(start, end));
    } else if (isToken(TokenType::NumberLiteral)) {
        auto key = parseNumericLiteral();
        expect(TokenType::Colon);
        auto value = parseExpression();
        
        TokenPosition end = getCurrentPosition();
        return std::make_unique<Property>(std::move(key), std::move(value), false, TokenPosition(start, end));
    } else if (isToken(TokenType::LeftBracket)) {
        advance();
        auto key = parseExpression();
        expect(TokenType::RightBracket);
        expect(TokenType::Colon);
        auto value = parseExpression();
        
        TokenPosition end = getCurrentPosition();
        return std::make_unique<Property>(std::move(key), std::move(value), true, TokenPosition(start, end));
    }
    
    error("Expected property");
    return nullptr;
}

std::unique_ptr<Element> Parser::parseElement() {
    TokenPosition start = getCurrentPosition();
    auto expression = parseExpression();
    TokenPosition end = getCurrentPosition();
    return std::make_unique<Element>(std::move(expression), TokenPosition(start, end));
}

} // namespace js
