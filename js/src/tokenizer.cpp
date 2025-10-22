#include "js/tokenizer.h"
#include <algorithm>
#include <cctype>
#include <unordered_map>

namespace js {

// Token implementation
Token::Token() : type_(TokenType::Invalid), value_(), position_() {}

Token::Token(TokenType type, const String& value, const TokenPosition& position)
    : type_(type), value_(value), position_(position) {}

Token::~Token() = default;

bool Token::isKeyword() const {
    switch (type_) {
        case TokenType::Break:
        case TokenType::Case:
        case TokenType::Catch:
        case TokenType::Class:
        case TokenType::Const:
        case TokenType::Continue:
        case TokenType::Debugger:
        case TokenType::Default:
        case TokenType::Delete:
        case TokenType::Do:
        case TokenType::Else:
        case TokenType::Export:
        case TokenType::Extends:
        case TokenType::Finally:
        case TokenType::For:
        case TokenType::Function:
        case TokenType::If:
        case TokenType::Import:
        case TokenType::In:
        case TokenType::Instanceof:
        case TokenType::Let:
        case TokenType::New:
        case TokenType::Return:
        case TokenType::Super:
        case TokenType::Switch:
        case TokenType::This:
        case TokenType::Throw:
        case TokenType::Try:
        case TokenType::Typeof:
        case TokenType::Var:
        case TokenType::Void:
        case TokenType::While:
        case TokenType::With:
        case TokenType::Yield:
        case TokenType::Await:
        case TokenType::Async:
        case TokenType::Static:
        case TokenType::Public:
        case TokenType::Private:
        case TokenType::Protected:
        case TokenType::Abstract:
        case TokenType::Interface:
        case TokenType::Enum:
        case TokenType::Namespace:
        case TokenType::Module:
        case TokenType::Implements:
        case TokenType::Package:
        case TokenType::Declare:
        case TokenType::Global:
        case TokenType::Ambient:
        case TokenType::Readonly:
        case TokenType::Override:
        case TokenType::Virtual:
        case TokenType::Sealed:
        case TokenType::Final:
        case TokenType::Volatile:
        case TokenType::Transient:
        case TokenType::Native:
        case TokenType::Synchronized:
        case TokenType::Strictfp:
            return true;
        default:
            return false;
    }
}

bool Token::isReservedWord() const {
    switch (type_) {
        case TokenType::Abstract:
        case TokenType::Arguments:
        case TokenType::Boolean:
        case TokenType::Byte:
        case TokenType::Char:
        case TokenType::Double:
        case TokenType::Eval:
        case TokenType::Float:
        case TokenType::Goto:
        case TokenType::Int:
        case TokenType::Long:
        case TokenType::Native:
        case TokenType::Short:
        case TokenType::Synchronized:
        case TokenType::Throws:
        case TokenType::Transient:
        case TokenType::Volatile:
            return true;
        default:
            return false;
    }
}

bool Token::isOperator() const {
    switch (type_) {
        case TokenType::AssignmentOperator:
        case TokenType::ArithmeticOperator:
        case TokenType::ComparisonOperator:
        case TokenType::LogicalOperator:
        case TokenType::BitwiseOperator:
        case TokenType::UnaryOperator:
        case TokenType::TernaryOperator:
        case TokenType::CommaOperator:
            return true;
        default:
            return false;
    }
}

bool Token::isPunctuation() const {
    switch (type_) {
        case TokenType::LeftParen:
        case TokenType::RightParen:
        case TokenType::LeftBracket:
        case TokenType::RightBracket:
        case TokenType::LeftBrace:
        case TokenType::RightBrace:
        case TokenType::Semicolon:
        case TokenType::Colon:
        case TokenType::Comma:
        case TokenType::Dot:
        case TokenType::QuestionMark:
        case TokenType::ExclamationMark:
        case TokenType::At:
        case TokenType::Hash:
        case TokenType::Dollar:
        case TokenType::Percent:
        case TokenType::Ampersand:
        case TokenType::Asterisk:
        case TokenType::Plus:
        case TokenType::Minus:
        case TokenType::Equals:
        case TokenType::LessThan:
        case TokenType::GreaterThan:
        case TokenType::Caret:
        case TokenType::Tilde:
        case TokenType::Pipe:
        case TokenType::Backslash:
        case TokenType::ForwardSlash:
        case TokenType::Backtick:
        case TokenType::Quote:
        case TokenType::DoubleQuote:
        case TokenType::SingleQuote:
            return true;
        default:
            return false;
    }
}

bool Token::isLiteral() const {
    switch (type_) {
        case TokenType::StringLiteral:
        case TokenType::NumberLiteral:
        case TokenType::BooleanLiteral:
        case TokenType::NullLiteral:
        case TokenType::UndefinedLiteral:
        case TokenType::RegExpLiteral:
        case TokenType::TemplateLiteral:
        case TokenType::BigIntLiteral:
            return true;
        default:
            return false;
    }
}

bool Token::isIdentifier() const {
    return type_ == TokenType::Identifier;
}

bool Token::isWhitespace() const {
    switch (type_) {
        case TokenType::Whitespace:
        case TokenType::LineBreak:
        case TokenType::CarriageReturn:
        case TokenType::Tab:
        case TokenType::Space:
            return true;
        default:
            return false;
    }
}

bool Token::isComment() const {
    switch (type_) {
        case TokenType::Comment:
        case TokenType::LineComment:
        case TokenType::BlockComment:
        case TokenType::DocComment:
            return true;
        default:
            return false;
    }
}

bool Token::isEndOfFile() const {
    return type_ == TokenType::EndOfFile;
}

bool Token::isValid() const {
    return type_ != TokenType::Invalid;
}

String Token::toString() const {
    return "Token(" + std::to_string(static_cast<int>(type_)) + ", \"" + value_ + "\")";
}

bool Token::operator==(const Token& other) const {
    return type_ == other.type_ && value_ == other.value_ && position_ == other.position_;
}

// Tokenizer implementation
Tokenizer::Tokenizer() : source_(), position_(0), filename_(), strictMode_(false), moduleMode_(false) {
    initializeModes();
}

Tokenizer::Tokenizer(const String& source) : source_(source), position_(0), filename_(), strictMode_(false), moduleMode_(false) {
    initializeModes();
}

Tokenizer::~Tokenizer() = default;

void Tokenizer::setSource(const String& source) {
    source_ = source;
    position_ = 0;
}

Vector<Token> Tokenizer::tokenize() {
    Vector<Token> tokens;
    reset();
    
    while (hasMoreTokens()) {
        Token token = nextToken();
        tokens.push_back(token);
        
        if (token.isEndOfFile()) {
            break;
        }
    }
    
    return tokens;
}

Token Tokenizer::nextToken() {
    if (!hasMoreTokens()) {
        return Token(TokenType::EndOfFile, "", getCurrentPosition());
    }
    
    skipWhitespace();
    
    if (!hasMoreTokens()) {
        return Token(TokenType::EndOfFile, "", getCurrentPosition());
    }
    
    char c = currentChar();
    
    if (isDigit(c)) {
        return readNumber();
    } else if (isLetter(c) || c == '_' || c == '$') {
        return readIdentifier();
    } else if (isQuote(c)) {
        return readString();
    } else if (c == '/') {
        char next = nextChar();
        if (next == '/' || next == '*') {
            return readComment();
        } else {
            return readOperator();
        }
    } else if (c == '`') {
        return readTemplateLiteral();
    } else if (isOperator(c)) {
        return readOperator();
    } else if (isPunctuation(c)) {
        return readPunctuation();
    } else if (c == '\\') {
        return readRegExp();
    } else {
        return Token(TokenType::Invalid, String(1, c), getCurrentPosition());
    }
}

Token Tokenizer::peekToken() {
    return peekToken(1);
}

Token Tokenizer::peekToken(size_t offset) {
    size_t savedPosition = position_;
    Token token;
    
    for (size_t i = 0; i < offset; ++i) {
        token = nextToken();
    }
    
    position_ = savedPosition;
    return token;
}

bool Tokenizer::hasMoreTokens() const {
    return position_ < source_.length();
}

void Tokenizer::reset() {
    position_ = 0;
}

void Tokenizer::skipWhitespace() {
    while (hasMoreTokens() && isWhitespace(currentChar())) {
        advance();
    }
}

void Tokenizer::skipComments() {
    while (hasMoreTokens()) {
        char c = currentChar();
        if (c == '/') {
            char next = nextChar();
            if (next == '/' || next == '*') {
                readComment();
            } else {
                break;
            }
        } else if (!isWhitespace(c)) {
            break;
        } else {
            advance();
        }
    }
}

Token Tokenizer::readString() {
    SourceLocation start = getCurrentLocation();
    char quote = currentChar();
    String value;
    
    advance(); // Skip opening quote
    
    while (hasMoreTokens() && currentChar() != quote) {
        char c = currentChar();
        if (c == '\\') {
            advance();
            if (hasMoreTokens()) {
                char escaped = currentChar();
                switch (escaped) {
                    case 'n': value += '\n'; break;
                    case 't': value += '\t'; break;
                    case 'r': value += '\r'; break;
                    case '\\': value += '\\'; break;
                    case '"': value += '"'; break;
                    case '\'': value += '\''; break;
                    case '0': value += '\0'; break;
                    case 'b': value += '\b'; break;
                    case 'f': value += '\f'; break;
                    case 'v': value += '\v'; break;
                    default: value += escaped; break;
                }
                advance();
            }
        } else {
            value += c;
            advance();
        }
    }
    
    if (hasMoreTokens() && currentChar() == quote) {
        advance(); // Skip closing quote
    }
    
    SourceLocation end = getCurrentLocation();
    return Token(TokenType::StringLiteral, value, TokenPosition(start, end));
}

Token Tokenizer::readNumber() {
    SourceLocation start = getCurrentLocation();
    String value;
    
    while (hasMoreTokens() && (isDigit(currentChar()) || currentChar() == '.' || currentChar() == 'e' || currentChar() == 'E' || currentChar() == '+' || currentChar() == '-')) {
        value += currentChar();
        advance();
    }
    
    SourceLocation end = getCurrentLocation();
    return Token(TokenType::NumberLiteral, value, TokenPosition(start, end));
}

Token Tokenizer::readIdentifier() {
    SourceLocation start = getCurrentLocation();
    String value;
    
    while (hasMoreTokens() && (isLetterOrDigit(currentChar()) || currentChar() == '_' || currentChar() == '$')) {
        value += currentChar();
        advance();
    }
    
    SourceLocation end = getCurrentLocation();
    
    // Check if it's a keyword
    if (isKeyword(value)) {
        return Token(TokenType::Keyword, value, TokenPosition(start, end));
    }
    
    // Check if it's a reserved word
    if (isReservedWord(value)) {
        return Token(TokenType::ReservedWord, value, TokenPosition(start, end));
    }
    
    return Token(TokenType::Identifier, value, TokenPosition(start, end));
}

Token Tokenizer::readOperator() {
    SourceLocation start = getCurrentLocation();
    String value;
    
    while (hasMoreTokens() && isOperator(currentChar())) {
        value += currentChar();
        advance();
    }
    
    SourceLocation end = getCurrentLocation();
    return Token(TokenType::ArithmeticOperator, value, TokenPosition(start, end));
}

Token Tokenizer::readPunctuation() {
    SourceLocation start = getCurrentLocation();
    char c = currentChar();
    advance();
    
    SourceLocation end = getCurrentLocation();
    
    switch (c) {
        case '(': return Token(TokenType::LeftParen, "(", TokenPosition(start, end));
        case ')': return Token(TokenType::RightParen, ")", TokenPosition(start, end));
        case '[': return Token(TokenType::LeftBracket, "[", TokenPosition(start, end));
        case ']': return Token(TokenType::RightBracket, "]", TokenPosition(start, end));
        case '{': return Token(TokenType::LeftBrace, "{", TokenPosition(start, end));
        case '}': return Token(TokenType::RightBrace, "}", TokenPosition(start, end));
        case ';': return Token(TokenType::Semicolon, ";", TokenPosition(start, end));
        case ':': return Token(TokenType::Colon, ":", TokenPosition(start, end));
        case ',': return Token(TokenType::Comma, ",", TokenPosition(start, end));
        case '.': return Token(TokenType::Dot, ".", TokenPosition(start, end));
        case '?': return Token(TokenType::QuestionMark, "?", TokenPosition(start, end));
        case '!': return Token(TokenType::ExclamationMark, "!", TokenPosition(start, end));
        case '@': return Token(TokenType::At, "@", TokenPosition(start, end));
        case '#': return Token(TokenType::Hash, "#", TokenPosition(start, end));
        case '$': return Token(TokenType::Dollar, "$", TokenPosition(start, end));
        case '%': return Token(TokenType::Percent, "%", TokenPosition(start, end));
        case '&': return Token(TokenType::Ampersand, "&", TokenPosition(start, end));
        case '*': return Token(TokenType::Asterisk, "*", TokenPosition(start, end));
        case '+': return Token(TokenType::Plus, "+", TokenPosition(start, end));
        case '-': return Token(TokenType::Minus, "-", TokenPosition(start, end));
        case '=': return Token(TokenType::Equals, "=", TokenPosition(start, end));
        case '<': return Token(TokenType::LessThan, "<", TokenPosition(start, end));
        case '>': return Token(TokenType::GreaterThan, ">", TokenPosition(start, end));
        case '^': return Token(TokenType::Caret, "^", TokenPosition(start, end));
        case '~': return Token(TokenType::Tilde, "~", TokenPosition(start, end));
        case '|': return Token(TokenType::Pipe, "|", TokenPosition(start, end));
        case '\\': return Token(TokenType::Backslash, "\\", TokenPosition(start, end));
        case '/': return Token(TokenType::ForwardSlash, "/", TokenPosition(start, end));
        case '`': return Token(TokenType::Backtick, "`", TokenPosition(start, end));
        case '"': return Token(TokenType::DoubleQuote, "\"", TokenPosition(start, end));
        case '\'': return Token(TokenType::SingleQuote, "'", TokenPosition(start, end));
        default: return Token(TokenType::Invalid, String(1, c), TokenPosition(start, end));
    }
}

Token Tokenizer::readComment() {
    SourceLocation start = getCurrentLocation();
    String value;
    
    char c = currentChar();
    advance();
    
    if (hasMoreTokens() && currentChar() == '/') {
        // Line comment
        advance();
        while (hasMoreTokens() && !isNewline(currentChar())) {
            value += currentChar();
            advance();
        }
        SourceLocation end = getCurrentLocation();
        return Token(TokenType::LineComment, value, TokenPosition(start, end));
    } else if (hasMoreTokens() && currentChar() == '*') {
        // Block comment
        advance();
        while (hasMoreTokens()) {
            char c = currentChar();
            if (c == '*' && hasMoreTokens() && nextChar() == '/') {
                advance(); // Skip *
                advance(); // Skip /
                break;
            }
            value += c;
            advance();
        }
        SourceLocation end = getCurrentLocation();
        return Token(TokenType::BlockComment, value, TokenPosition(start, end));
    }
    
    SourceLocation end = getCurrentLocation();
    return Token(TokenType::Invalid, value, TokenPosition(start, end));
}

Token Tokenizer::readTemplateLiteral() {
    SourceLocation start = getCurrentLocation();
    String value;
    
    advance(); // Skip opening backtick
    
    while (hasMoreTokens() && currentChar() != '`') {
        char c = currentChar();
        if (c == '\\') {
            advance();
            if (hasMoreTokens()) {
                char escaped = currentChar();
                value += escaped;
                advance();
            }
        } else if (c == '$' && hasMoreTokens() && nextChar() == '{') {
            // Template literal expression
            value += "${";
            advance();
            advance();
        } else {
            value += c;
            advance();
        }
    }
    
    if (hasMoreTokens() && currentChar() == '`') {
        advance(); // Skip closing backtick
    }
    
    SourceLocation end = getCurrentLocation();
    return Token(TokenType::TemplateLiteral, value, TokenPosition(start, end));
}

Token Tokenizer::readRegExp() {
    SourceLocation start = getCurrentLocation();
    String value;
    
    advance(); // Skip opening slash
    
    while (hasMoreTokens() && currentChar() != '/') {
        char c = currentChar();
        if (c == '\\') {
            advance();
            if (hasMoreTokens()) {
                char escaped = currentChar();
                value += escaped;
                advance();
            }
        } else {
            value += c;
            advance();
        }
    }
    
    if (hasMoreTokens() && currentChar() == '/') {
        advance(); // Skip closing slash
    }
    
    // Read flags
    while (hasMoreTokens() && isLetter(currentChar())) {
        value += currentChar();
        advance();
    }
    
    SourceLocation end = getCurrentLocation();
    return Token(TokenType::RegExpLiteral, value, TokenPosition(start, end));
}

char Tokenizer::currentChar() const {
    return hasMoreTokens() ? source_[position_] : '\0';
}

char Tokenizer::nextChar() const {
    return (position_ + 1 < source_.length()) ? source_[position_ + 1] : '\0';
}

char Tokenizer::peekChar() const {
    return peekChar(1);
}

char Tokenizer::peekChar(size_t offset) const {
    return (position_ + offset < source_.length()) ? source_[position_ + offset] : '\0';
}

void Tokenizer::advance() {
    if (hasMoreTokens()) {
        position_++;
    }
}

void Tokenizer::advance(size_t count) {
    for (size_t i = 0; i < count && hasMoreTokens(); ++i) {
        advance();
    }
}

void Tokenizer::retreat() {
    if (position_ > 0) {
        position_--;
    }
}

void Tokenizer::retreat(size_t count) {
    for (size_t i = 0; i < count && position_ > 0; ++i) {
        retreat();
    }
}

bool Tokenizer::isDigit(char c) const {
    return std::isdigit(c);
}

bool Tokenizer::isLetter(char c) const {
    return std::isalpha(c);
}

bool Tokenizer::isLetterOrDigit(char c) const {
    return std::isalnum(c);
}

bool Tokenizer::isWhitespace(char c) const {
    return std::isspace(c);
}

bool Tokenizer::isNewline(char c) const {
    return c == '\n' || c == '\r';
}

bool Tokenizer::isOperator(char c) const {
    switch (c) {
        case '+':
        case '-':
        case '*':
        case '/':
        case '%':
        case '=':
        case '!':
        case '<':
        case '>':
        case '&':
        case '|':
        case '^':
        case '~':
        case '?':
        case ':':
            return true;
        default:
            return false;
    }
}

bool Tokenizer::isPunctuation(char c) const {
    switch (c) {
        case '(':
        case ')':
        case '[':
        case ']':
        case '{':
        case '}':
        case ';':
        case ':':
        case ',':
        case '.':
        case '?':
        case '!':
        case '@':
        case '#':
        case '$':
        case '%':
        case '&':
        case '*':
        case '+':
        case '-':
        case '=':
        case '<':
        case '>':
        case '^':
        case '~':
        case '|':
        case '\\':
        case '/':
        case '`':
        case '"':
        case '\'':
            return true;
        default:
            return false;
    }
}

bool Tokenizer::isQuote(char c) const {
    return c == '"' || c == '\'';
}

bool Tokenizer::isEscape(char c) const {
    return c == '\\';
}

bool Tokenizer::isHexDigit(char c) const {
    return std::isxdigit(c);
}

bool Tokenizer::isOctalDigit(char c) const {
    return c >= '0' && c <= '7';
}

bool Tokenizer::isBinaryDigit(char c) const {
    return c == '0' || c == '1';
}

TokenPosition Tokenizer::getCurrentPosition() const {
    SourceLocation start = getCurrentLocation();
    return TokenPosition(start, start);
}

SourceLocation Tokenizer::getCurrentLocation() const {
    size_t line = 1;
    size_t column = 1;
    
    for (size_t i = 0; i < position_; ++i) {
        if (source_[i] == '\n') {
            line++;
            column = 1;
        } else {
            column++;
        }
    }
    
    return SourceLocation(line, column, position_, filename_);
}

bool Tokenizer::isKeyword(const String& value) const {
    static const std::unordered_map<String, bool> keywords = {
        {"break", true}, {"case", true}, {"catch", true}, {"class", true}, {"const", true},
        {"continue", true}, {"debugger", true}, {"default", true}, {"delete", true}, {"do", true},
        {"else", true}, {"export", true}, {"extends", true}, {"finally", true}, {"for", true},
        {"function", true}, {"if", true}, {"import", true}, {"in", true}, {"instanceof", true},
        {"let", true}, {"new", true}, {"return", true}, {"super", true}, {"switch", true},
        {"this", true}, {"throw", true}, {"try", true}, {"typeof", true}, {"var", true},
        {"void", true}, {"while", true}, {"with", true}, {"yield", true}, {"await", true},
        {"async", true}, {"static", true}, {"public", true}, {"private", true}, {"protected", true},
        {"abstract", true}, {"interface", true}, {"enum", true}, {"namespace", true}, {"module", true},
        {"implements", true}, {"package", true}, {"declare", true}, {"global", true}, {"ambient", true},
        {"readonly", true}, {"override", true}, {"virtual", true}, {"sealed", true}, {"final", true},
        {"volatile", true}, {"transient", true}, {"native", true}, {"synchronized", true}, {"strictfp", true}
    };
    
    return keywords.find(value) != keywords.end();
}

bool Tokenizer::isReservedWord(const String& value) const {
    static const std::unordered_map<String, bool> reservedWords = {
        {"abstract", true}, {"arguments", true}, {"boolean", true}, {"byte", true}, {"char", true},
        {"double", true}, {"eval", true}, {"float", true}, {"goto", true}, {"int", true},
        {"long", true}, {"native", true}, {"short", true}, {"synchronized", true}, {"throws", true},
        {"transient", true}, {"volatile", true}
    };
    
    return reservedWords.find(value) != reservedWords.end();
}

bool Tokenizer::isOperator(const String& value) const {
    static const std::unordered_map<String, bool> operators = {
        {"+", true}, {"-", true}, {"*", true}, {"/", true}, {"%", true},
        {"=", true}, {"!", true}, {"<", true}, {">", true}, {"&", true},
        {"|", true}, {"^", true}, {"~", true}, {"?", true}, {":", true},
        {"==", true}, {"!=", true}, {"===", true}, {"!==", true}, {"<=", true},
        {">=", true}, {"&&", true}, {"||", true}, {"++", true}, {"--", true},
        {"+=", true}, {"-=", true}, {"*=", true}, {"/=", true}, {"%=", true},
        {"&=", true}, {"|=", true}, {"^=", true}, {"<<", true}, {">>", true},
        {">>>", true}, {"<<=", true}, {">>=", true}, {">>>=", true}
    };
    
    return operators.find(value) != operators.end();
}

bool Tokenizer::isPunctuation(const String& value) const {
    static const std::unordered_map<String, bool> punctuation = {
        {"(", true}, {")", true}, {"[", true}, {"]", true}, {"{", true},
        {"}", true}, {";", true}, {":", true}, {",", true}, {".", true},
        {"?", true}, {"!", true}, {"@", true}, {"#", true}, {"$", true},
        {"%", true}, {"&", true}, {"*", true}, {"+", true}, {"-", true},
        {"=", true}, {"<", true}, {">", true}, {"^", true}, {"~", true},
        {"|", true}, {"\\", true}, {"/", true}, {"`", true}, {"\"", true},
        {"'", true}
    };
    
    return punctuation.find(value) != punctuation.end();
}

String Tokenizer::getKeywordValue(const String& value) const {
    return value;
}

String Tokenizer::getOperatorValue(const String& value) const {
    return value;
}

String Tokenizer::getPunctuationValue(const String& value) const {
    return value;
}

void Tokenizer::initializeModes() {
    setDefaultModes();
}

void Tokenizer::resetModes() {
    setDefaultModes();
}

void Tokenizer::setDefaultModes() {
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

void Tokenizer::setStrictModes() {
    strictMode_ = true;
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

void Tokenizer::setModuleModes() {
    strictMode_ = true;
    moduleMode_ = true;
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

// Additional mode setters would be implemented here
// These are placeholder implementations for the various mode setters

void Tokenizer::setJSXModes() {
    // Set JSX modes
}

void Tokenizer::setTypeScriptModes() {
    // Set TypeScript modes
}

void Tokenizer::setFlowModes() {
    // Set Flow modes
}

void Tokenizer::setDecoratorModes() {
    // Set decorator modes
}

void Tokenizer::setAsyncIterationModes() {
    // Set async iteration modes
}

void Tokenizer::setOptionalChainingModes() {
    // Set optional chaining modes
}

void Tokenizer::setNullishCoalescingModes() {
    // Set nullish coalescing modes
}

void Tokenizer::setBigIntModes() {
    // Set BigInt modes
}

void Tokenizer::setDynamicImportModes() {
    // Set dynamic import modes
}

void Tokenizer::setTopLevelAwaitModes() {
    // Set top-level await modes
}

void Tokenizer::setImportMetaModes() {
    // Set import.meta modes
}

void Tokenizer::setPrivateFieldsModes() {
    // Set private fields modes
}

void Tokenizer::setClassFieldsModes() {
    // Set class fields modes
}

void Tokenizer::setStaticBlocksModes() {
    // Set static blocks modes
}

void Tokenizer::setErgonomicBrandChecksModes() {
    // Set ergonomic brand checks modes
}

void Tokenizer::setHashbangModes() {
    // Set hashbang modes
}

void Tokenizer::setUnicodeModes() {
    // Set Unicode modes
}

void Tokenizer::setRegexModes() {
    // Set regex modes
}

void Tokenizer::setTemplateModes() {
    // Set template modes
}

void Tokenizer::setArrowFunctionModes() {
    // Set arrow function modes
}

void Tokenizer::setAsyncFunctionModes() {
    // Set async function modes
}

void Tokenizer::setGeneratorFunctionModes() {
    // Set generator function modes
}

void Tokenizer::setAsyncGeneratorFunctionModes() {
    // Set async generator function modes
}

void Tokenizer::setClassModes() {
    // Set class modes
}

void Tokenizer::setImportModes() {
    // Set import modes
}

void Tokenizer::setExportModes() {
    // Set export modes
}

void Tokenizer::setDestructuringModes() {
    // Set destructuring modes
}

void Tokenizer::setSpreadModes() {
    // Set spread modes
}

void Tokenizer::setRestModes() {
    // Set rest modes
}

void Tokenizer::setDefaultParameterModes() {
    // Set default parameter modes
}

void Tokenizer::setRestParameterModes() {
    // Set rest parameter modes
}

void Tokenizer::setArrowParameterModes() {
    // Set arrow parameter modes
}

void Tokenizer::setObjectLiteralModes() {
    // Set object literal modes
}

void Tokenizer::setArrayLiteralModes() {
    // Set array literal modes
}

void Tokenizer::setFunctionLiteralModes() {
    // Set function literal modes
}

void Tokenizer::setClassLiteralModes() {
    // Set class literal modes
}

void Tokenizer::setTemplateLiteralModes() {
    // Set template literal modes
}

void Tokenizer::setRegExpLiteralModes() {
    // Set RegExp literal modes
}

void Tokenizer::setBigIntLiteralModes() {
    // Set BigInt literal modes
}

void Tokenizer::setNumericLiteralModes() {
    // Set numeric literal modes
}

void Tokenizer::setStringLiteralModes() {
    // Set string literal modes
}

void Tokenizer::setBooleanLiteralModes() {
    // Set boolean literal modes
}

void Tokenizer::setNullLiteralModes() {
    // Set null literal modes
}

void Tokenizer::setUndefinedLiteralModes() {
    // Set undefined literal modes
}

void Tokenizer::setIdentifierModes() {
    // Set identifier modes
}

void Tokenizer::setKeywordModes() {
    // Set keyword modes
}

void Tokenizer::setReservedWordModes() {
    // Set reserved word modes
}

void Tokenizer::setOperatorModes() {
    // Set operator modes
}

void Tokenizer::setPunctuationModes() {
    // Set punctuation modes
}

void Tokenizer::setCommentModes() {
    // Set comment modes
}

void Tokenizer::setWhitespaceModes() {
    // Set whitespace modes
}

void Tokenizer::setNewlineModes() {
    // Set newline modes
}

void Tokenizer::setTabModes() {
    // Set tab modes
}

void Tokenizer::setSpaceModes() {
    // Set space modes
}

void Tokenizer::setCarriageReturnModes() {
    // Set carriage return modes
}

void Tokenizer::setLineBreakModes() {
    // Set line break modes
}

void Tokenizer::setBOMModes() {
    // Set BOM modes
}

void Tokenizer::setShebangModes() {
    // Set shebang modes
}

void Tokenizer::setDocCommentModes() {
    // Set doc comment modes
}

void Tokenizer::setBlockCommentModes() {
    // Set block comment modes
}

void Tokenizer::setLineCommentModes() {
    // Set line comment modes
}

void Tokenizer::setInvalidModes() {
    // Set invalid modes
}

void Tokenizer::setEndOfFileModes() {
    // Set end of file modes
}

} // namespace js
