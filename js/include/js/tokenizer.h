#pragma once

#include "types.h"
#include <string>
#include <vector>
#include <memory>

namespace js {

// Token types
enum class TokenType {
    // Literals
    StringLiteral,
    NumberLiteral,
    BooleanLiteral,
    NullLiteral,
    UndefinedLiteral,
    RegExpLiteral,
    TemplateLiteral,
    BigIntLiteral,

    // Identifiers
    Identifier,
    Keyword,
    ReservedWord,

    // Operators
    AssignmentOperator,
    ArithmeticOperator,
    ComparisonOperator,
    LogicalOperator,
    BitwiseOperator,
    UnaryOperator,
    TernaryOperator,
    CommaOperator,

    // Punctuation
    LeftParen,
    RightParen,
    LeftBracket,
    RightBracket,
    LeftBrace,
    RightBrace,
    Semicolon,
    Colon,
    Comma,
    Dot,
    QuestionMark,
    ExclamationMark,
    At,
    Hash,
    Dollar,
    Percent,
    Ampersand,
    Asterisk,
    Plus,
    Minus,
    Equals,
    LessThan,
    GreaterThan,
    Caret,
    Tilde,
    Pipe,
    Backslash,
    ForwardSlash,
    Backtick,
    Quote,
    DoubleQuote,
    SingleQuote,

    // Keywords
    Break,
    Case,
    Catch,
    Class,
    Const,
    Continue,
    Debugger,
    Default,
    Delete,
    Do,
    Else,
    Export,
    Extends,
    Finally,
    For,
    Function,
    If,
    Import,
    In,
    Instanceof,
    Let,
    New,
    Return,
    Super,
    Switch,
    This,
    Throw,
    Try,
    Typeof,
    Var,
    Void,
    While,
    With,
    Yield,
    Await,
    Async,
    Static,
    Public,
    Private,
    Protected,
    Abstract,
    Interface,
    Enum,
    Namespace,
    Module,
    Implements,
    Package,
    Declare,
    Global,
    Ambient,
    Readonly,
    Override,
    Virtual,
    Sealed,
    Final,
    Volatile,
    Transient,
    Native,
    Synchronized,
    Strictfp,
    Native,
    Abstract,
    Interface,
    Enum,
    Namespace,
    Module,
    Implements,
    Package,
    Declare,
    Global,
    Ambient,
    Readonly,
    Override,
    Virtual,
    Sealed,
    Final,
    Volatile,
    Transient,
    Native,
    Synchronized,
    Strictfp,

    // Reserved words
    Abstract,
    Arguments,
    Boolean,
    Byte,
    Char,
    Double,
    Eval,
    Float,
    Goto,
    Int,
    Long,
    Native,
    Short,
    Synchronized,
    Throws,
    Transient,
    Volatile,

    // Special tokens
    EndOfFile,
    Invalid,
    Whitespace,
    Comment,
    LineComment,
    BlockComment,
    DocComment,
    Shebang,
    BOM,
    LineBreak,
    CarriageReturn,
    Tab,
    Space
};

// Token class
class Token {
public:
    Token();
    Token(TokenType type, const String& value, const TokenPosition& position);
    ~Token();

    TokenType type() const { return type_; }
    void setType(TokenType type) { type_ = type; }

    const String& value() const { return value_; }
    void setValue(const String& value) { value_ = value; }

    const TokenPosition& position() const { return position_; }
    void setPosition(const TokenPosition& position) { position_ = position; }

    bool isKeyword() const;
    bool isReservedWord() const;
    bool isOperator() const;
    bool isPunctuation() const;
    bool isLiteral() const;
    bool isIdentifier() const;
    bool isWhitespace() const;
    bool isComment() const;
    bool isEndOfFile() const;
    bool isValid() const;

    String toString() const;

    bool operator==(const Token& other) const;
    bool operator!=(const Token& other) const { return !(*this == other); }

private:
    TokenType type_;
    String value_;
    TokenPosition position_;
};

// Tokenizer class
class Tokenizer {
public:
    Tokenizer();
    explicit Tokenizer(const String& source);
    ~Tokenizer();

    void setSource(const String& source);
    const String& source() const { return source_; }

    Vector<Token> tokenize();
    Token nextToken();
    Token peekToken();
    Token peekToken(size_t offset);

    bool hasMoreTokens() const;
    size_t position() const { return position_; }
    void setPosition(size_t position) { position_ = position; }

    void reset();
    void skipWhitespace();
    void skipComments();

    Token readString();
    Token readNumber();
    Token readIdentifier();
    Token readOperator();
    Token readPunctuation();
    Token readComment();
    Token readTemplateLiteral();
    Token readRegExp();

    char currentChar() const;
    char nextChar() const;
    char peekChar() const;
    char peekChar(size_t offset) const;

    void advance();
    void advance(size_t count);
    void retreat();
    void retreat(size_t count);

    bool isDigit(char c) const;
    bool isLetter(char c) const;
    bool isLetterOrDigit(char c) const;
    bool isWhitespace(char c) const;
    bool isNewline(char c) const;
    bool isOperator(char c) const;
    bool isPunctuation(char c) const;
    bool isQuote(char c) const;
    bool isEscape(char c) const;
    bool isHexDigit(char c) const;
    bool isOctalDigit(char c) const;
    bool isBinaryDigit(char c) const;

    TokenPosition getCurrentPosition() const;
    SourceLocation getCurrentLocation() const;

    bool isKeyword(const String& value) const;
    bool isReservedWord(const String& value) const;
    bool isOperator(const String& value) const;
    bool isPunctuation(const String& value) const;

    String getKeywordValue(const String& value) const;
    String getOperatorValue(const String& value) const;
    String getPunctuationValue(const String& value) const;

    void setFilename(const String& filename) { filename_ = filename; }
    const String& filename() const { return filename_; }

    void setStrictMode(bool strict) { strictMode_ = strict; }
    bool strictMode() const { return strictMode_; }

    void setModuleMode(bool module) { moduleMode_ = module; }
    bool moduleMode() const { return moduleMode_; }

    void setJSXMode(bool jsx) { jsxMode_ = jsx; }
    bool jsxMode() const { return jsxMode_; }

    void setTypeScriptMode(bool typescript) { typescriptMode_ = typescript; }
    bool typeScriptMode() const { return typescriptMode_; }

    void setFlowMode(bool flow) { flowMode_ = flow; }
    bool flowMode() const { return flowMode_; }

    void setDecoratorMode(bool decorator) { decoratorMode_ = decorator; }
    bool decoratorMode() const { return decoratorMode_; }

    void setAsyncIterationMode(bool asyncIteration) { asyncIterationMode_ = asyncIteration; }
    bool asyncIterationMode() const { return asyncIterationMode_; }

    void setOptionalChainingMode(bool optionalChaining) { optionalChainingMode_ = optionalChaining; }
    bool optionalChainingMode() const { return optionalChainingMode_; }

    void setNullishCoalescingMode(bool nullishCoalescing) { nullishCoalescingMode_ = nullishCoalescing; }
    bool nullishCoalescingMode() const { return nullishCoalescingMode_; }

    void setBigIntMode(bool bigInt) { bigIntMode_ = bigInt; }
    bool bigIntMode() const { return bigIntMode_; }

    void setDynamicImportMode(bool dynamicImport) { dynamicImportMode_ = dynamicImport; }
    bool dynamicImportMode() const { return dynamicImportMode_; }

    void setTopLevelAwaitMode(bool topLevelAwait) { topLevelAwaitMode_ = topLevelAwait; }
    bool topLevelAwaitMode() const { return topLevelAwaitMode_; }

    void setImportMetaMode(bool importMeta) { importMetaMode_ = importMeta; }
    bool importMetaMode() const { return importMetaMode_; }

    void setPrivateFieldsMode(bool privateFields) { privateFieldsMode_ = privateFields; }
    bool privateFieldsMode() const { return privateFieldsMode_; }

    void setClassFieldsMode(bool classFields) { classFieldsMode_ = classFields; }
    bool classFieldsMode() const { return classFieldsMode_; }

    void setStaticBlocksMode(bool staticBlocks) { staticBlocksMode_ = staticBlocks; }
    bool staticBlocksMode() const { return staticBlocksMode_; }

    void setErgonomicBrandChecksMode(bool ergonomicBrandChecks) { ergonomicBrandChecksMode_ = ergonomicBrandChecks; }
    bool ergonomicBrandChecksMode() const { return ergonomicBrandChecksMode_; }

    void setHashbangMode(bool hashbang) { hashbangMode_ = hashbang; }
    bool hashbangMode() const { return hashbangMode_; }

    void setUnicodeMode(bool unicode) { unicodeMode_ = unicode; }
    bool unicodeMode() const { return unicodeMode_; }

    void setRegexMode(bool regex) { regexMode_ = regex; }
    bool regexMode() const { return regexMode_; }

    void setTemplateMode(bool template) { templateMode_ = template; }
    bool templateMode() const { return templateMode_; }

    void setArrowFunctionMode(bool arrowFunction) { arrowFunctionMode_ = arrowFunction; }
    bool arrowFunctionMode() const { return arrowFunctionMode_; }

    void setAsyncFunctionMode(bool asyncFunction) { asyncFunctionMode_ = asyncFunction; }
    bool asyncFunctionMode() const { return asyncFunctionMode_; }

    void setGeneratorFunctionMode(bool generatorFunction) { generatorFunctionMode_ = generatorFunction; }
    bool generatorFunctionMode() const { return generatorFunctionMode_; }

    void setAsyncGeneratorFunctionMode(bool asyncGeneratorFunction) { asyncGeneratorFunctionMode_ = asyncGeneratorFunction; }
    bool asyncGeneratorFunctionMode() const { return asyncGeneratorFunctionMode_; }

    void setClassMode(bool class) { classMode_ = class; }
    bool classMode() const { return classMode_; }

    void setImportMode(bool import) { importMode_ = import; }
    bool importMode() const { return importMode_; }

    void setExportMode(bool export) { exportMode_ = export; }
    bool exportMode() const { return exportMode_; }

    void setDestructuringMode(bool destructuring) { destructuringMode_ = destructuring; }
    bool destructuringMode() const { return destructuringMode_; }

    void setSpreadMode(bool spread) { spreadMode_ = spread; }
    bool spreadMode() const { return spreadMode_; }

    void setRestMode(bool rest) { restMode_ = rest; }
    bool restMode() const { return restMode_; }

    void setDefaultParameterMode(bool defaultParameter) { defaultParameterMode_ = defaultParameter; }
    bool defaultParameterMode() const { return defaultParameterMode_; }

    void setRestParameterMode(bool restParameter) { restParameterMode_ = restParameter; }
    bool restParameterMode() const { return restParameterMode_; }

    void setArrowParameterMode(bool arrowParameter) { arrowParameterMode_ = arrowParameter; }
    bool arrowParameterMode() const { return arrowParameterMode_; }

    void setObjectLiteralMode(bool objectLiteral) { objectLiteralMode_ = objectLiteral; }
    bool objectLiteralMode() const { return objectLiteralMode_; }

    void setArrayLiteralMode(bool arrayLiteral) { arrayLiteralMode_ = arrayLiteral; }
    bool arrayLiteralMode() const { return arrayLiteralMode_; }

    void setFunctionLiteralMode(bool functionLiteral) { functionLiteralMode_ = functionLiteral; }
    bool functionLiteralMode() const { return functionLiteralMode_; }

    void setClassLiteralMode(bool classLiteral) { classLiteralMode_ = classLiteral; }
    bool classLiteralMode() const { return classLiteralMode_; }

    void setTemplateLiteralMode(bool templateLiteral) { templateLiteralMode_ = templateLiteral; }
    bool templateLiteralMode() const { return templateLiteralMode_; }

    void setRegExpLiteralMode(bool regExpLiteral) { regExpLiteralMode_ = regExpLiteral; }
    bool regExpLiteralMode() const { return regExpLiteralMode_; }

    void setBigIntLiteralMode(bool bigIntLiteral) { bigIntLiteralMode_ = bigIntLiteral; }
    bool bigIntLiteralMode() const { return bigIntLiteralMode_; }

    void setNumericLiteralMode(bool numericLiteral) { numericLiteralMode_ = numericLiteral; }
    bool numericLiteralMode() const { return numericLiteralMode_; }

    void setStringLiteralMode(bool stringLiteral) { stringLiteralMode_ = stringLiteral; }
    bool stringLiteralMode() const { return stringLiteralMode_; }

    void setBooleanLiteralMode(bool booleanLiteral) { booleanLiteralMode_ = booleanLiteral; }
    bool booleanLiteralMode() const { return booleanLiteralMode_; }

    void setNullLiteralMode(bool nullLiteral) { nullLiteralMode_ = nullLiteral; }
    bool nullLiteralMode() const { return nullLiteralMode_; }

    void setUndefinedLiteralMode(bool undefinedLiteral) { undefinedLiteralMode_ = undefinedLiteral; }
    bool undefinedLiteralMode() const { return undefinedLiteralMode_; }

    void setIdentifierMode(bool identifier) { identifierMode_ = identifier; }
    bool identifierMode() const { return identifierMode_; }

    void setKeywordMode(bool keyword) { keywordMode_ = keyword; }
    bool keywordMode() const { return keywordMode_; }

    void setReservedWordMode(bool reservedWord) { reservedWordMode_ = reservedWord; }
    bool reservedWordMode() const { return reservedWordMode_; }

    void setOperatorMode(bool operator) { operatorMode_ = operator; }
    bool operatorMode() const { return operatorMode_; }

    void setPunctuationMode(bool punctuation) { punctuationMode_ = punctuation; }
    bool punctuationMode() const { return punctuationMode_; }

    void setCommentMode(bool comment) { commentMode_ = comment; }
    bool commentMode() const { return commentMode_; }

    void setWhitespaceMode(bool whitespace) { whitespaceMode_ = whitespace; }
    bool whitespaceMode() const { return whitespaceMode_; }

    void setNewlineMode(bool newline) { newlineMode_ = newline; }
    bool newlineMode() const { return newlineMode_; }

    void setTabMode(bool tab) { tabMode_ = tab; }
    bool tabMode() const { return tabMode_; }

    void setSpaceMode(bool space) { spaceMode_ = space; }
    bool spaceMode() const { return spaceMode_; }

    void setCarriageReturnMode(bool carriageReturn) { carriageReturnMode_ = carriageReturn; }
    bool carriageReturnMode() const { return carriageReturnMode_; }

    void setLineBreakMode(bool lineBreak) { lineBreakMode_ = lineBreak; }
    bool lineBreakMode() const { return lineBreakMode_; }

    void setBOMMode(bool bom) { bomMode_ = bom; }
    bool bomMode() const { return bomMode_; }

    void setShebangMode(bool shebang) { shebangMode_ = shebang; }
    bool shebangMode() const { return shebangMode_; }

    void setDocCommentMode(bool docComment) { docCommentMode_ = docComment; }
    bool docCommentMode() const { return docCommentMode_; }

    void setBlockCommentMode(bool blockComment) { blockCommentMode_ = blockComment; }
    bool blockCommentMode() const { return blockCommentMode_; }

    void setLineCommentMode(bool lineComment) { lineCommentMode_ = lineComment; }
    bool lineCommentMode() const { return lineCommentMode_; }

    void setInvalidMode(bool invalid) { invalidMode_ = invalid; }
    bool invalidMode() const { return invalidMode_; }

    void setEndOfFileMode(bool endOfFile) { endOfFileMode_ = endOfFile; }
    bool endOfFileMode() const { return endOfFileMode_; }

private:
    String source_;
    size_t position_;
    String filename_;
    bool strictMode_;
    bool moduleMode_;
    bool jsxMode_;
    bool typescriptMode_;
    bool flowMode_;
    bool decoratorMode_;
    bool asyncIterationMode_;
    bool optionalChainingMode_;
    bool nullishCoalescingMode_;
    bool bigIntMode_;
    bool dynamicImportMode_;
    bool topLevelAwaitMode_;
    bool importMetaMode_;
    bool privateFieldsMode_;
    bool classFieldsMode_;
    bool staticBlocksMode_;
    bool ergonomicBrandChecksMode_;
    bool hashbangMode_;
    bool unicodeMode_;
    bool regexMode_;
    bool templateMode_;
    bool arrowFunctionMode_;
    bool asyncFunctionMode_;
    bool generatorFunctionMode_;
    bool asyncGeneratorFunctionMode_;
    bool classMode_;
    bool importMode_;
    bool exportMode_;
    bool destructuringMode_;
    bool spreadMode_;
    bool restMode_;
    bool defaultParameterMode_;
    bool restParameterMode_;
    bool arrowParameterMode_;
    bool objectLiteralMode_;
    bool arrayLiteralMode_;
    bool functionLiteralMode_;
    bool classLiteralMode_;
    bool templateLiteralMode_;
    bool regExpLiteralMode_;
    bool bigIntLiteralMode_;
    bool numericLiteralMode_;
    bool stringLiteralMode_;
    bool booleanLiteralMode_;
    bool nullLiteralMode_;
    bool undefinedLiteralMode_;
    bool identifierMode_;
    bool keywordMode_;
    bool reservedWordMode_;
    bool operatorMode_;
    bool punctuationMode_;
    bool commentMode_;
    bool whitespaceMode_;
    bool newlineMode_;
    bool tabMode_;
    bool spaceMode_;
    bool carriageReturnMode_;
    bool lineBreakMode_;
    bool bomMode_;
    bool shebangMode_;
    bool docCommentMode_;
    bool blockCommentMode_;
    bool lineCommentMode_;
    bool invalidMode_;
    bool endOfFileMode_;

    void initializeModes();
    void resetModes();
    void setDefaultModes();
    void setStrictModes();
    void setModuleModes();
    void setJSXModes();
    void setTypeScriptModes();
    void setFlowModes();
    void setDecoratorModes();
    void setAsyncIterationModes();
    void setOptionalChainingModes();
    void setNullishCoalescingModes();
    void setBigIntModes();
    void setDynamicImportModes();
    void setTopLevelAwaitModes();
    void setImportMetaModes();
    void setPrivateFieldsModes();
    void setClassFieldsModes();
    void setStaticBlocksModes();
    void setErgonomicBrandChecksModes();
    void setHashbangModes();
    void setUnicodeModes();
    void setRegexModes();
    void setTemplateModes();
    void setArrowFunctionModes();
    void setAsyncFunctionModes();
    void setGeneratorFunctionModes();
    void setAsyncGeneratorFunctionModes();
    void setClassModes();
    void setImportModes();
    void setExportModes();
    void setDestructuringModes();
    void setSpreadModes();
    void setRestModes();
    void setDefaultParameterModes();
    void setRestParameterModes();
    void setArrowParameterModes();
    void setObjectLiteralModes();
    void setArrayLiteralModes();
    void setFunctionLiteralModes();
    void setClassLiteralModes();
    void setTemplateLiteralModes();
    void setRegExpLiteralModes();
    void setBigIntLiteralModes();
    void setNumericLiteralModes();
    void setStringLiteralModes();
    void setBooleanLiteralModes();
    void setNullLiteralModes();
    void setUndefinedLiteralModes();
    void setIdentifierModes();
    void setKeywordModes();
    void setReservedWordModes();
    void setOperatorModes();
    void setPunctuationModes();
    void setCommentModes();
    void setWhitespaceModes();
    void setNewlineModes();
    void setTabModes();
    void setSpaceModes();
    void setCarriageReturnModes();
    void setLineBreakModes();
    void setBOMModes();
    void setShebangModes();
    void setDocCommentModes();
    void setBlockCommentModes();
    void setLineCommentModes();
    void setInvalidModes();
    void setEndOfFileModes();
};

} // namespace js
