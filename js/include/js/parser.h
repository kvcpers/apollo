#pragma once

#include "tokenizer.h"
#include "ast.h"
#include <memory>
#include <vector>
#include <unordered_map>

namespace js {

// Forward declarations
class AST;
class Node;
class Statement;
class Expression;
class Declaration;
class Program;
class Module;

// Parser class for building AST from tokens
class Parser {
public:
    Parser();
    explicit Parser(const std::string& source);
    ~Parser();

    // Source management
    void setSource(const std::string& source);
    const std::string& source() const { return source_; }

    // Parsing methods
    std::unique_ptr<AST> parse();
    std::unique_ptr<AST> parseScript();
    std::unique_ptr<AST> parseModule();
    std::unique_ptr<AST> parseExpression();
    std::unique_ptr<AST> parseStatement();
    std::unique_ptr<AST> parseDeclaration();

    // Program parsing
    std::unique_ptr<Program> parseProgram();
    std::unique_ptr<Module> parseModule();

    // Statement parsing
    std::unique_ptr<Statement> parseStatement();
    std::unique_ptr<Statement> parseBlockStatement();
    std::unique_ptr<Statement> parseVariableStatement();
    std::unique_ptr<Statement> parseFunctionStatement();
    std::unique_ptr<Statement> parseClassStatement();
    std::unique_ptr<Statement> parseIfStatement();
    std::unique_ptr<Statement> parseSwitchStatement();
    std::unique_ptr<Statement> parseForStatement();
    std::unique_ptr<Statement> parseWhileStatement();
    std::unique_ptr<Statement> parseDoWhileStatement();
    std::unique_ptr<Statement> parseForInStatement();
    std::unique_ptr<Statement> parseForOfStatement();
    std::unique_ptr<Statement> parseTryStatement();
    std::unique_ptr<Statement> parseThrowStatement();
    std::unique_ptr<Statement> parseReturnStatement();
    std::unique_ptr<Statement> parseBreakStatement();
    std::unique_ptr<Statement> parseContinueStatement();
    std::unique_ptr<Statement> parseLabeledStatement();
    std::unique_ptr<Statement> parseWithStatement();
    std::unique_ptr<Statement> parseDebuggerStatement();

    // Expression parsing
    std::unique_ptr<Expression> parseExpression();
    std::unique_ptr<Expression> parseAssignmentExpression();
    std::unique_ptr<Expression> parseConditionalExpression();
    std::unique_ptr<Expression> parseLogicalExpression();
    std::unique_ptr<Expression> parseBitwiseExpression();
    std::unique_ptr<Expression> parseEqualityExpression();
    std::unique_ptr<Expression> parseRelationalExpression();
    std::unique_ptr<Expression> parseShiftExpression();
    std::unique_ptr<Expression> parseAdditiveExpression();
    std::unique_ptr<Expression> parseMultiplicativeExpression();
    std::unique_ptr<Expression> parseUnaryExpression();
    std::unique_ptr<Expression> parseUpdateExpression();
    std::unique_ptr<Expression> parseNewExpression();
    std::unique_ptr<Expression> parseCallExpression();
    std::unique_ptr<Expression> parseMemberExpression();
    std::unique_ptr<Expression> parsePrimaryExpression();
    std::unique_ptr<Expression> parseArrayExpression();
    std::unique_ptr<Expression> parseObjectExpression();
    std::unique_ptr<Expression> parseFunctionExpression();
    std::unique_ptr<Expression> parseArrowFunctionExpression();
    std::unique_ptr<Expression> parseClassExpression();
    std::unique_ptr<Expression> parseTemplateLiteral();
    std::unique_ptr<Expression> parseTaggedTemplateExpression();
    std::unique_ptr<Expression> parseSequenceExpression();

    // Declaration parsing
    std::unique_ptr<Declaration> parseDeclaration();
    std::unique_ptr<Declaration> parseVariableDeclaration();
    std::unique_ptr<Declaration> parseFunctionDeclaration();
    std::unique_ptr<Declaration> parseClassDeclaration();
    std::unique_ptr<Declaration> parseImportDeclaration();
    std::unique_ptr<Declaration> parseExportDeclaration();

    // Pattern parsing
    std::unique_ptr<Pattern> parsePattern();
    std::unique_ptr<Pattern> parseArrayPattern();
    std::unique_ptr<Pattern> parseObjectPattern();
    std::unique_ptr<Pattern> parseRestPattern();
    std::unique_ptr<Pattern> parseAssignmentPattern();

    // Parameter parsing
    std::vector<std::unique_ptr<Parameter>> parseParameters();
    std::unique_ptr<Parameter> parseParameter();

    // Property parsing
    std::vector<std::unique_ptr<Property>> parseProperties();
    std::unique_ptr<Property> parseProperty();

    // Element parsing
    std::vector<std::unique_ptr<Element>> parseElements();
    std::unique_ptr<Element> parseElement();

    // Clause parsing
    std::vector<std::unique_ptr<CaseClause>> parseCaseClauses();
    std::unique_ptr<CaseClause> parseCaseClause();

    // Catch clause parsing
    std::unique_ptr<CatchClause> parseCatchClause();

    // Import/Export parsing
    std::vector<std::unique_ptr<ImportSpecifier>> parseImportSpecifiers();
    std::unique_ptr<ImportSpecifier> parseImportSpecifier();
    std::vector<std::unique_ptr<ExportSpecifier>> parseExportSpecifiers();
    std::unique_ptr<ExportSpecifier> parseExportSpecifier();

    // Template parsing
    std::vector<std::unique_ptr<TemplateElement>> parseTemplateElements();
    std::unique_ptr<TemplateElement> parseTemplateElement();

    // Meta property parsing
    std::unique_ptr<MetaProperty> parseMetaProperty();

    // Super expression parsing
    std::unique_ptr<SuperExpression> parseSuperExpression();

    // This expression parsing
    std::unique_ptr<ThisExpression> parseThisExpression();

    // Yield expression parsing
    std::unique_ptr<YieldExpression> parseYieldExpression();

    // Await expression parsing
    std::unique_ptr<AwaitExpression> parseAwaitExpression();

    // Identifier parsing
    std::unique_ptr<Identifier> parseIdentifier();

    // Literal parsing
    std::unique_ptr<Literal> parseLiteral();
    std::unique_ptr<Literal> parseStringLiteral();
    std::unique_ptr<Literal> parseNumericLiteral();
    std::unique_ptr<Literal> parseBooleanLiteral();
    std::unique_ptr<Literal> parseNullLiteral();
    std::unique_ptr<Literal> parseUndefinedLiteral();
    std::unique_ptr<Literal> parseRegExpLiteral();
    std::unique_ptr<Literal> parseTemplateLiteral();
    std::unique_ptr<Literal> parseBigIntLiteral();

    // Token management
    Token currentToken() const;
    Token peekToken() const;
    Token peekToken(size_t offset) const;
    void advance();
    void retreat();
    bool hasMoreTokens() const;

    // Token checking
    bool isToken(TokenType type) const;
    bool isToken(const std::string& value) const;
    bool isToken(TokenType type, const std::string& value) const;
    bool isKeyword(const std::string& keyword) const;
    bool isOperator(const std::string& op) const;
    bool isPunctuation(const std::string& punct) const;

    // Expect methods
    Token expect(TokenType type);
    Token expect(const std::string& value);
    Token expect(TokenType type, const std::string& value);
    Token expectKeyword(const std::string& keyword);
    Token expectOperator(const std::string& op);
    Token expectPunctuation(const std::string& punct);

    // Optional methods
    bool optional(TokenType type);
    bool optional(const std::string& value);
    bool optional(TokenType type, const std::string& value);
    bool optionalKeyword(const std::string& keyword);
    bool optionalOperator(const std::string& op);
    bool optionalPunctuation(const std::string& punct);

    // Skip methods
    void skipSemicolon();
    void skipWhitespace();
    void skipComments();

    // Error handling
    void error(const std::string& message);
    void error(const std::string& message, const Token& token);
    void warning(const std::string& message);
    void warning(const std::string& message, const Token& token);

    // Parser state
    size_t position() const { return position_; }
    void setPosition(size_t position) { position_ = position_; }
    void reset();
    bool isStrictMode() const { return strictMode_; }
    void setStrictMode(bool strict) { strictMode_ = strict; }
    bool isModuleMode() const { return moduleMode_; }
    void setModuleMode(bool module) { moduleMode_ = module; }

    // Parser options
    void setOptions(const ParserOptions& options);
    const ParserOptions& options() const { return options_; }

    // Scope management
    void enterScope();
    void exitScope();
    void addBinding(const std::string& name, BindingType type);
    bool hasBinding(const std::string& name) const;
    BindingType getBindingType(const std::string& name) const;

    // Function context
    void enterFunction();
    void exitFunction();
    bool inFunction() const;

    // Class context
    void enterClass();
    void exitClass();
    bool inClass() const;

    // Loop context
    void enterLoop();
    void exitLoop();
    bool inLoop() const;

    // Switch context
    void enterSwitch();
    void exitSwitch();
    bool inSwitch() const;

    // Try context
    void enterTry();
    void exitTry();
    bool inTry() const;

    // With context
    void enterWith();
    void exitWith();
    bool inWith() const;

    // Strict mode checks
    void checkStrictMode(const std::string& feature);
    void checkStrictMode(const std::string& feature, const Token& token);

    // Operator precedence
    int getPrecedence(const std::string& op) const;
    bool isRightAssociative(const std::string& op) const;

    // Automatic semicolon insertion
    bool needsAutomaticSemicolon() const;
    void insertAutomaticSemicolon();

    // Line continuation
    bool isLineContinuation() const;

    // Template literal parsing
    bool inTemplateLiteral() const;
    void enterTemplateLiteral();
    void exitTemplateLiteral();

    // JSX parsing
    bool isJSXMode() const { return jsxMode_; }
    void setJSXMode(bool jsx) { jsxMode_ = jsx; }

    // TypeScript parsing
    bool isTypeScriptMode() const { return typescriptMode_; }
    void setTypeScriptMode(bool typescript) { typescriptMode_ = typescript; }

    // Flow parsing
    bool isFlowMode() const { return flowMode_; }
    void setFlowMode(bool flow) { flowMode_ = flow; }

    // Decorator parsing
    bool isDecoratorMode() const { return decoratorMode_; }
    void setDecoratorMode(bool decorator) { decoratorMode_ = decorator; }

    // Async iteration parsing
    bool isAsyncIterationMode() const { return asyncIterationMode_; }
    void setAsyncIterationMode(bool asyncIteration) { asyncIterationMode_ = asyncIteration; }

    // Optional chaining parsing
    bool isOptionalChainingMode() const { return optionalChainingMode_; }
    void setOptionalChainingMode(bool optionalChaining) { optionalChainingMode_ = optionalChaining; }

    // Nullish coalescing parsing
    bool isNullishCoalescingMode() const { return nullishCoalescingMode_; }
    void setNullishCoalescingMode(bool nullishCoalescing) { nullishCoalescingMode_ = nullishCoalescing; }

    // BigInt parsing
    bool isBigIntMode() const { return bigIntMode_; }
    void setBigIntMode(bool bigInt) { bigIntMode_ = bigInt; }

    // Dynamic import parsing
    bool isDynamicImportMode() const { return dynamicImportMode_; }
    void setDynamicImportMode(bool dynamicImport) { dynamicImportMode_ = dynamicImport; }

    // Top-level await parsing
    bool isTopLevelAwaitMode() const { return topLevelAwaitMode_; }
    void setTopLevelAwaitMode(bool topLevelAwait) { topLevelAwaitMode_ = topLevelAwait; }

    // Import.meta parsing
    bool isImportMetaMode() const { return importMetaMode_; }
    void setImportMetaMode(bool importMeta) { importMetaMode_ = importMeta; }

    // Private fields parsing
    bool isPrivateFieldsMode() const { return privateFieldsMode_; }
    void setPrivateFieldsMode(bool privateFields) { privateFieldsMode_ = privateFields; }

    // Class fields parsing
    bool isClassFieldsMode() const { return classFieldsMode_; }
    void setClassFieldsMode(bool classFields) { classFieldsMode_ = classFields; }

    // Static blocks parsing
    bool isStaticBlocksMode() const { return staticBlocksMode_; }
    void setStaticBlocksMode(bool staticBlocks) { staticBlocksMode_ = staticBlocks; }

    // Ergonomic brand checks parsing
    bool isErgonomicBrandChecksMode() const { return ergonomicBrandChecksMode_; }
    void setErgonomicBrandChecksMode(bool ergonomicBrandChecks) { ergonomicBrandChecksMode_ = ergonomicBrandChecks; }

    // Hashbang parsing
    bool isHashbangMode() const { return hashbangMode_; }
    void setHashbangMode(bool hashbang) { hashbangMode_ = hashbang; }

    // Unicode parsing
    bool isUnicodeMode() const { return unicodeMode_; }
    void setUnicodeMode(bool unicode) { unicodeMode_ = unicode; }

    // Regex parsing
    bool isRegexMode() const { return regexMode_; }
    void setRegexMode(bool regex) { regexMode_ = regex; }

    // Template parsing
    bool isTemplateMode() const { return templateMode_; }
    void setTemplateMode(bool template) { templateMode_ = template; }

    // Arrow function parsing
    bool isArrowFunctionMode() const { return arrowFunctionMode_; }
    void setArrowFunctionMode(bool arrowFunction) { arrowFunctionMode_ = arrowFunction; }

    // Async function parsing
    bool isAsyncFunctionMode() const { return asyncFunctionMode_; }
    void setAsyncFunctionMode(bool asyncFunction) { asyncFunctionMode_ = asyncFunction; }

    // Generator function parsing
    bool isGeneratorFunctionMode() const { return generatorFunctionMode_; }
    void setGeneratorFunctionMode(bool generatorFunction) { generatorFunctionMode_ = generatorFunction; }

    // Async generator function parsing
    bool isAsyncGeneratorFunctionMode() const { return asyncGeneratorFunctionMode_; }
    void setAsyncGeneratorFunctionMode(bool asyncGeneratorFunction) { asyncGeneratorFunctionMode_ = asyncGeneratorFunction; }

    // Class parsing
    bool isClassMode() const { return classMode_; }
    void setClassMode(bool class) { classMode_ = class; }

    // Import parsing
    bool isImportMode() const { return importMode_; }
    void setImportMode(bool import) { importMode_ = import; }

    // Export parsing
    bool isExportMode() const { return exportMode_; }
    void setExportMode(bool export) { exportMode_ = export; }

    // Destructuring parsing
    bool isDestructuringMode() const { return destructuringMode_; }
    void setDestructuringMode(bool destructuring) { destructuringMode_ = destructuring; }

    // Spread parsing
    bool isSpreadMode() const { return spreadMode_; }
    void setSpreadMode(bool spread) { spreadMode_ = spread; }

    // Rest parsing
    bool isRestMode() const { return restMode_; }
    void setRestMode(bool rest) { restMode_ = rest; }

    // Default parameter parsing
    bool isDefaultParameterMode() const { return defaultParameterMode_; }
    void setDefaultParameterMode(bool defaultParameter) { defaultParameterMode_ = defaultParameter; }

    // Rest parameter parsing
    bool isRestParameterMode() const { return restParameterMode_; }
    void setRestParameterMode(bool restParameter) { restParameterMode_ = restParameter; }

    // Arrow parameter parsing
    bool isArrowParameterMode() const { return arrowParameterMode_; }
    void setArrowParameterMode(bool arrowParameter) { arrowParameterMode_ = arrowParameter; }

    // Object literal parsing
    bool isObjectLiteralMode() const { return objectLiteralMode_; }
    void setObjectLiteralMode(bool objectLiteral) { objectLiteralMode_ = objectLiteral; }

    // Array literal parsing
    bool isArrayLiteralMode() const { return arrayLiteralMode_; }
    void setArrayLiteralMode(bool arrayLiteral) { arrayLiteralMode_ = arrayLiteral; }

    // Function literal parsing
    bool isFunctionLiteralMode() const { return functionLiteralMode_; }
    void setFunctionLiteralMode(bool functionLiteral) { functionLiteralMode_ = functionLiteral; }

    // Class literal parsing
    bool isClassLiteralMode() const { return classLiteralMode_; }
    void setClassLiteralMode(bool classLiteral) { classLiteralMode_ = classLiteral; }

    // Template literal parsing
    bool isTemplateLiteralMode() const { return templateLiteralMode_; }
    void setTemplateLiteralMode(bool templateLiteral) { templateLiteralMode_ = templateLiteral; }

    // RegExp literal parsing
    bool isRegExpLiteralMode() const { return regExpLiteralMode_; }
    void setRegExpLiteralMode(bool regExpLiteral) { regExpLiteralMode_ = regExpLiteral; }

    // BigInt literal parsing
    bool isBigIntLiteralMode() const { return bigIntLiteralMode_; }
    void setBigIntLiteralMode(bool bigIntLiteral) { bigIntLiteralMode_ = bigIntLiteral; }

    // Numeric literal parsing
    bool isNumericLiteralMode() const { return numericLiteralMode_; }
    void setNumericLiteralMode(bool numericLiteral) { numericLiteralMode_ = numericLiteral; }

    // String literal parsing
    bool isStringLiteralMode() const { return stringLiteralMode_; }
    void setStringLiteralMode(bool stringLiteral) { stringLiteralMode_ = stringLiteral; }

    // Boolean literal parsing
    bool isBooleanLiteralMode() const { return booleanLiteralMode_; }
    void setBooleanLiteralMode(bool booleanLiteral) { booleanLiteralMode_ = booleanLiteral; }

    // Null literal parsing
    bool isNullLiteralMode() const { return nullLiteralMode_; }
    void setNullLiteralMode(bool nullLiteral) { nullLiteralMode_ = nullLiteral; }

    // Undefined literal parsing
    bool isUndefinedLiteralMode() const { return undefinedLiteralMode_; }
    void setUndefinedLiteralMode(bool undefinedLiteral) { undefinedLiteralMode_ = undefinedLiteral; }

    // Identifier parsing
    bool isIdentifierMode() const { return identifierMode_; }
    void setIdentifierMode(bool identifier) { identifierMode_ = identifier; }

    // Keyword parsing
    bool isKeywordMode() const { return keywordMode_; }
    void setKeywordMode(bool keyword) { keywordMode_ = keyword; }

    // Reserved word parsing
    bool isReservedWordMode() const { return reservedWordMode_; }
    void setReservedWordMode(bool reservedWord) { reservedWordMode_ = reservedWord; }

    // Operator parsing
    bool isOperatorMode() const { return operatorMode_; }
    void setOperatorMode(bool operator) { operatorMode_ = operator; }

    // Punctuation parsing
    bool isPunctuationMode() const { return punctuationMode_; }
    void setPunctuationMode(bool punctuation) { punctuationMode_ = punctuation; }

    // Comment parsing
    bool isCommentMode() const { return commentMode_; }
    void setCommentMode(bool comment) { commentMode_ = comment; }

    // Whitespace parsing
    bool isWhitespaceMode() const { return whitespaceMode_; }
    void setWhitespaceMode(bool whitespace) { whitespaceMode_ = whitespace; }

    // Newline parsing
    bool isNewlineMode() const { return newlineMode_; }
    void setNewlineMode(bool newline) { newlineMode_ = newline; }

    // Tab parsing
    bool isTabMode() const { return tabMode_; }
    void setTabMode(bool tab) { tabMode_ = tab; }

    // Space parsing
    bool isSpaceMode() const { return spaceMode_; }
    void setSpaceMode(bool space) { spaceMode_ = space; }

    // Carriage return parsing
    bool isCarriageReturnMode() const { return carriageReturnMode_; }
    void setCarriageReturnMode(bool carriageReturn) { carriageReturnMode_ = carriageReturn; }

    // Line break parsing
    bool isLineBreakMode() const { return lineBreakMode_; }
    void setLineBreakMode(bool lineBreak) { lineBreakMode_ = lineBreak; }

    // BOM parsing
    bool isBOMMode() const { return bomMode_; }
    void setBOMMode(bool bom) { bomMode_ = bom; }

    // Shebang parsing
    bool isShebangMode() const { return shebangMode_; }
    void setShebangMode(bool shebang) { shebangMode_ = shebang; }

    // Doc comment parsing
    bool isDocCommentMode() const { return docCommentMode_; }
    void setDocCommentMode(bool docComment) { docCommentMode_ = docComment; }

    // Block comment parsing
    bool isBlockCommentMode() const { return blockCommentMode_; }
    void setBlockCommentMode(bool blockComment) { blockCommentMode_ = blockComment; }

    // Line comment parsing
    bool isLineCommentMode() const { return lineCommentMode_; }
    void setLineCommentMode(bool lineComment) { lineCommentMode_ = lineComment; }

    // Invalid parsing
    bool isInvalidMode() const { return invalidMode_; }
    void setInvalidMode(bool invalid) { invalidMode_ = invalid; }

    // End of file parsing
    bool isEndOfFileMode() const { return endOfFileMode_; }
    void setEndOfFileMode(bool endOfFile) { endOfFileMode_ = endOfFile; }

private:
    // Core components
    std::string source_;
    Tokenizer tokenizer_;
    std::vector<Token> tokens_;
    size_t position_;

    // Parser state
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

    // Parser options
    ParserOptions options_;

    // Scope stack
    std::vector<Scope> scopeStack_;

    // Context flags
    bool inFunction_;
    bool inClass_;
    bool inLoop_;
    bool inSwitch_;
    bool inTry_;
    bool inWith_;
    bool inTemplateLiteral_;

    // Error handling
    std::vector<ParseError> errors_;
    std::vector<ParseWarning> warnings_;

    // Helper methods
    void initialize();
    void reset();
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

// Parser options
struct ParserOptions {
    bool strictMode;
    bool moduleMode;
    bool jsxMode;
    bool typescriptMode;
    bool flowMode;
    bool decoratorMode;
    bool asyncIterationMode;
    bool optionalChainingMode;
    bool nullishCoalescingMode;
    bool bigIntMode;
    bool dynamicImportMode;
    bool topLevelAwaitMode;
    bool importMetaMode;
    bool privateFieldsMode;
    bool classFieldsMode;
    bool staticBlocksMode;
    bool ergonomicBrandChecksMode;
    bool hashbangMode;
    bool unicodeMode;
    bool regexMode;
    bool templateMode;
    bool arrowFunctionMode;
    bool asyncFunctionMode;
    bool generatorFunctionMode;
    bool asyncGeneratorFunctionMode;
    bool classMode;
    bool importMode;
    bool exportMode;
    bool destructuringMode;
    bool spreadMode;
    bool restMode;
    bool defaultParameterMode;
    bool restParameterMode;
    bool arrowParameterMode;
    bool objectLiteralMode;
    bool arrayLiteralMode;
    bool functionLiteralMode;
    bool classLiteralMode;
    bool templateLiteralMode;
    bool regExpLiteralMode;
    bool bigIntLiteralMode;
    bool numericLiteralMode;
    bool stringLiteralMode;
    bool booleanLiteralMode;
    bool nullLiteralMode;
    bool undefinedLiteralMode;
    bool identifierMode;
    bool keywordMode;
    bool reservedWordMode;
    bool operatorMode;
    bool punctuationMode;
    bool commentMode;
    bool whitespaceMode;
    bool newlineMode;
    bool tabMode;
    bool spaceMode;
    bool carriageReturnMode;
    bool lineBreakMode;
    bool bomMode;
    bool shebangMode;
    bool docCommentMode;
    bool blockCommentMode;
    bool lineCommentMode;
    bool invalidMode;
    bool endOfFileMode;

    ParserOptions() : strictMode(false), moduleMode(false), jsxMode(false), typescriptMode(false), flowMode(false), decoratorMode(false), asyncIterationMode(false), optionalChainingMode(false), nullishCoalescingMode(false), bigIntMode(false), dynamicImportMode(false), topLevelAwaitMode(false), importMetaMode(false), privateFieldsMode(false), classFieldsMode(false), staticBlocksMode(false), ergonomicBrandChecksMode(false), hashbangMode(false), unicodeMode(false), regexMode(false), templateMode(false), arrowFunctionMode(false), asyncFunctionMode(false), generatorFunctionMode(false), asyncGeneratorFunctionMode(false), classMode(false), importMode(false), exportMode(false), destructuringMode(false), spreadMode(false), restMode(false), defaultParameterMode(false), restParameterMode(false), arrowParameterMode(false), objectLiteralMode(false), arrayLiteralMode(false), functionLiteralMode(false), classLiteralMode(false), templateLiteralMode(false), regExpLiteralMode(false), bigIntLiteralMode(false), numericLiteralMode(false), stringLiteralMode(false), booleanLiteralMode(false), nullLiteralMode(false), undefinedLiteralMode(false), identifierMode(false), keywordMode(false), reservedWordMode(false), operatorMode(false), punctuationMode(false), commentMode(false), whitespaceMode(false), newlineMode(false), tabMode(false), spaceMode(false), carriageReturnMode(false), lineBreakMode(false), bomMode(false), shebangMode(false), docCommentMode(false), blockCommentMode(false), lineCommentMode(false), invalidMode(false), endOfFileMode(false) {}
};

// Scope for binding management
struct Scope {
    std::unordered_map<std::string, BindingType> bindings;
    ScopeType type;

    Scope(ScopeType type) : type(type) {}
};

// Parse error
struct ParseError {
    std::string message;
    TokenPosition position;
    std::string filename;

    ParseError(const std::string& message, const TokenPosition& position, const std::string& filename = "")
        : message(message), position(position), filename(filename) {}
};

// Parse warning
struct ParseWarning {
    std::string message;
    TokenPosition position;
    std::string filename;

    ParseWarning(const std::string& message, const TokenPosition& position, const std::string& filename = "")
        : message(message), position(position), filename(filename) {}
};

} // namespace js
