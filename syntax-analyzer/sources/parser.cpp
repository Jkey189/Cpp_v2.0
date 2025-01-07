#include "../headers/parser.h"


IdentifierType convertFromTokenTypeToIdentifierType(const my::TokenType& type) {
  switch (type) {
  case my::TokenType::INT:
    return IdentifierType::INT;
  case my::TokenType::FLOAT:
    return IdentifierType::FLOAT;
  case my::TokenType::CHAR:
    return IdentifierType::CHAR;
  case my::TokenType::BOOL:
    return IdentifierType::BOOL;
  case my::TokenType::VOID:
    return IdentifierType::VOID;
  case my::TokenType::STRING:
    return IdentifierType::STRING;
  case my::TokenType::ARRAY:
    return IdentifierType::ARRAY;
  default:
    /*return IdentifierType::UNKNOWN;*/
    throw std::runtime_error("Syntax-semantic error: invalid identifier type");
  }
}


SemanticAnalyzer semanticAnalyzer;


void Parser::parseProgram() {
  while (currToken_.getType() != my::TokenType::END) {
    parseDeclaration();
  }
}

void Parser::parseDeclaration() {
  if (currToken_.getType() == my::TokenType::KEYWORD && currToken_.getValue() == "func") {
    parseFunction();
  } else {
    advance();
    // parseInstruction();
  }
}

void Parser::parseFunction() {
  const std::string functionName = "parseFunction()";

  expect(my::TokenType::KEYWORD, functionName); // 'func'
  advance();

  // is current token - type
  parseType();

  // check identifier
  expect(my::TokenType::IDENTIFIER, functionName); // name of function

  // check parameters
  semanticAnalyzer.enterScope(functionName);
  expect(my::TokenType::LPAREN, functionName); // '('
  if (currToken_.getType() != my::TokenType::RPAREN) { // if we have any parameters
    parseParameters();
  }
  expect(my::TokenType::RPAREN, functionName); // ')'

  parseBlock();
  semanticAnalyzer.exitScope();
}

void Parser::parseParameters() {
  if (currToken_.getType() == my::TokenType::RPAREN) { // we haven't any parameters
    return;
  }

  parseParameter();

  while (currToken_.getType() == my::TokenType::COMMA) {
    advance(); // skip ','
    parseParameter();
  }
}

void Parser::parseParameter() {
  const std::string functionName = "parseParameter()";
  if (!isType(currToken_)) {
    throw std::runtime_error(
      "Syntax error: Expected type for parameter, found '" + currToken_.getValue() +
      "' (" + getTokenValue(currToken_.getType()) + ")." + " || parseParameter()"
      );
  }
  parseType();

  semanticAnalyzer.declareIdentifier(currToken_.getValue(),
    convertFromTokenTypeToIdentifierType(currToken_.getType()));
  expect(my::TokenType::IDENTIFIER, functionName); // name of variable
}

void Parser::parseBlock() {
  const std::string functionName = "parseBlock()";

  expect(my::TokenType::LBRACE, functionName); // '{'

  semanticAnalyzer.enterScope("block"); // VARY BAD - fix in future

  while (currToken_.getType() != my::TokenType::RBRACE) {
    if (currToken_.getType() == my::TokenType::END) {
      throw std::runtime_error("Syntax error: Unexpected end of input inside block || parseBlock()");
    }

    parseInstruction(); // parsing next instruction
  }

  semanticAnalyzer.exitScope();

  expect(my::TokenType::RBRACE, functionName); // '}'
}

void Parser::parseInstruction() {
  const std::string functionName = "parseInstruction()";

  if (currToken_.getType() == my::TokenType::RBRACE) {
    return; // we haven't any instructions
  }

  if (currToken_.getType() == my::TokenType::COMMENT_LITERAL) {
    advance();
  } else if (currToken_.getType() == my::TokenType::LBRACE) {
    parseBlock();
  } else if (currToken_.getType() == my::TokenType::KEYWORD && currToken_.getValue() == "cin") {
    parseInput();
  } else if (currToken_.getType() == my::TokenType::KEYWORD && currToken_.getValue() == "cout") {
    parseOutput();
  } else if (currToken_.getType() == my::TokenType::IF) {
    parseConditional();
  } else if (currToken_.getType() == my::TokenType::FOR || currToken_.getType() == my::TokenType::WHILE) {
    parseLoop();
  } else if (currToken_.getType() == my::TokenType::SWITCH) {
    parseSwitch();
  } else if (currToken_.getType() == my::TokenType::BREAK || currToken_.getType() == my::TokenType::CONTINUE) {
    advance(); // skip 'break' / 'continue'
    advance(); // skip ';'
  } else if (isType(currToken_)) {
    parseInitialization();
  } else if (currToken_.getType() == my::TokenType::RETURN) {
    advance(); // Skip 'return'
    if (currToken_.getType() == my::TokenType::SEMICOLON) {
      advance(); // 'return;'
    } else {
      parseExpression();
    }
  } else if (currToken_.getType() == my::TokenType::SEMICOLON) {
    advance(); // skip ';'
  } else if (currToken_.getType() == my::TokenType::IDENTIFIER) {
    if (lexer_.peek(currCount).getType() == my::TokenType::SEMICOLON) {
      advance(); // skip identifier
      advance(); // skip ';'
    } else {
      parseAssignment();
    }
  } else {
    throw std::runtime_error(
     "Syntax error at token: '" + currToken_.getValue() +
     "' (" + getTokenValue(currToken_.getType()) + "), Expected: " + getTokenValue(my::TokenType::SEMICOLON) +
     " || parseInstruction()");
  } /*else {
    expect(my::TokenType::SEMICOLON);
  }*/
}

void Parser::parseInput() {
  const std::string functionName = "parseInput()";

  advance(); // skip 'cin'
  expect(my::TokenType::IN, functionName); // '>>'
  expect(my::TokenType::IDENTIFIER, functionName); // 'variable'
  while (currToken_.getType() != my::TokenType::SEMICOLON) {
    expect(my::TokenType::IN, functionName); // '>>'
    expect(my::TokenType::IDENTIFIER, functionName); // 'variable'
  }
  expect(my::TokenType::SEMICOLON, functionName); // ';'
}

void Parser::parseOutput() {
  const std::string functionName = "parseOutput()";

  advance(); // skip 'cout'
  expect(my::TokenType::OUT, functionName); // '<<'
  parseExpression();
  while (currToken_.getType() != my::TokenType::SEMICOLON) {
    expect(my::TokenType::OUT, functionName); // '<<'
    parseExpression();
  }
  expect(my::TokenType::SEMICOLON, functionName); // ';'
}

void Parser::parseConditional() {
  const std::string functionName = "parseConditional()";

  advance(); // skip 'if'
  expect(my::TokenType::LPAREN, functionName); // '('
  parseExpression(); // for the first time we haven't any conditions
  expect(my::TokenType::RPAREN, functionName); // ');
  parseBlock(); // 'block'
}

void Parser::parseLoop() {
  const std::string functionName = "parseLoop()";

  if (currToken_.getType() == my::TokenType::WHILE) {
    advance(); // skip 'while'

    expect(my::TokenType::LPAREN, functionName); // '('
    parseExpression();
    expect(my::TokenType::RPAREN, functionName); // ')'

    parseBlock(); // 'block' - loop's body
  } else if (currToken_.getType() == my::TokenType::FOR) {
    advance(); // skip 'for'

    expect(my::TokenType::LPAREN, functionName); // '('
    parseInitialization();
    // expect(my::TokenType::SEMICOLON); // first ';' after initialization
    parseExpression();
    expect(my::TokenType::SEMICOLON, functionName); // second ';' after condition
    parseStep();
    expect(my::TokenType::RPAREN, functionName); // ')'

    parseBlock(); // 'block' - loop's body
  } else { // it useless, but - why not?
    throw std::runtime_error(
    "Syntax error at token: '" + currToken_.getValue() +
    "' (" + getTokenValue(currToken_.getType()) + "), Expected: FOR /or/ WHILE" +
    " || parseLoop()");
  }
}

void Parser::parseInitialization() {
  const std::string functionName = "parseInitialization()";

  const IdentifierType type = convertFromTokenTypeToIdentifierType(currToken_.getType());

  parseType();

  semanticAnalyzer.declareIdentifier(currToken_.getValue(), type);

  expect(my::TokenType::IDENTIFIER, functionName); // variable's name
  while (currToken_.getType() == my::TokenType::LBRACKET) { // is array's element ([i], [i][j], ...)
    advance(); // '['
    parseIndex();
    expect(my::TokenType::RBRACKET, functionName); // '['
  }
  if (currToken_.getType() != my::TokenType::SEMICOLON) {
    expect(my::TokenType::ASSIGN, functionName);
    // advance();
    parseExpression(); // for the first time we haven't any expressions
  }
  expect(my::TokenType::SEMICOLON, functionName); // maybe useful !!!!!!!!!
}

void Parser::parseAssignment() {
  const std::string functionName = "parseAssignment()";

  expect(my::TokenType::IDENTIFIER, functionName); // variable's name
  while (currToken_.getType() == my::TokenType::LBRACKET) { // is array's element ([i], [i][j], ...)
    advance(); // '['
    parseIndex();
    expect(my::TokenType::RBRACKET, functionName); // '['
  }
  if (currToken_.getType() != my::TokenType::SEMICOLON) {
    expect(my::TokenType::ASSIGN, functionName);
  }
  parseExpression(); // for the first time we haven't any expressions
  expect(my::TokenType::SEMICOLON, functionName); // maybe useful !!!!!!!!!
}

void Parser::parseStep() {
  const std::string functionName = "parseStep()";

  expect(my::TokenType::IDENTIFIER, functionName); // name of variable-count
  expect(my::TokenType::ASSIGN, functionName);
  parseExpression();
}

void Parser::parseSwitch() {
  const std::string functionName = "parseSwitch()";

  advance(); // skip 'switch'
  expect(my::TokenType::LPAREN, functionName); // '('
  parseExpression(); // for the first time we haven't any conditions
  expect(my::TokenType::RPAREN, functionName); // ')'

  expect(my::TokenType::LBRACE, functionName); // '{'
  while (currToken_.getType() == my::TokenType::CASE) {
    expect(my::TokenType::CASE, functionName);
    if (currToken_.getType() == my::TokenType::COMMENT_LITERAL) {
      throw std::runtime_error(
      "Syntax error at token: '" + currToken_.getValue() +
      "' (" + getTokenValue(currToken_.getType()) + "), Expected: NOT COMMENT LITERAL!!!" +
      " || parseSwitch()"
      );
    }
    parseLiteral(); // for the first time we haven't any realizations of parseLiteral()
    expect(my::TokenType::COLON, functionName);
    /*while (currToken_.getType() != my::TokenType::CASE &&
         currToken_.getType() != my::TokenType::DEFAULT &&
         currToken_.getType() != my::TokenType::RBRACE) {
      parseInstruction();
         }*/
    while (currToken_.getType() != my::TokenType::BREAK) {
      parseInstruction(); // 'instruction' in case
    }
    expect(my::TokenType::BREAK, functionName); // 'break'
    expect(my::TokenType::SEMICOLON, functionName); // ';'
  }
  expect(my::TokenType::DEFAULT, functionName); // 'default'
  expect(my::TokenType::COLON, functionName); // ':'
  while (currToken_.getType() != my::TokenType::RBRACE) {
    parseInstruction(); // 'instruction' in default
  }
  expect(my::TokenType::RBRACE, functionName); // '}'
}

void Parser::parseLiteral() {
  const std::string functionName = "parseLiteral()";

  if (currToken_.getType() == my::TokenType::COMMENT_LITERAL) {
    // parseCommentLiteral(); // maybe useless methods...
    expect(my::TokenType::COMMENT_LITERAL, functionName);
  } else if (currToken_.getType() == my::TokenType::INTEGER_LITERAL) {
    // parseIntegerLiteral(); // maybe useless methods...
    expect(my::TokenType::INTEGER_LITERAL, functionName);
} else if (currToken_.getType() == my::TokenType::FLOAT_LITERAL) {
    // parseFloatLiteral(); // maybe useless methods...
    expect(my::TokenType::FLOAT_LITERAL, functionName);
  } else if (currToken_.getType() == my::TokenType::STRING_LITERAL) {
    // parseStringLiteral(); // maybe useless methods...
    expect(my::TokenType::STRING_LITERAL, functionName);
  } else if (currToken_.getType() == my::TokenType::CHAR_LITERAL) {
    // parseCharLiteral(); // maybe useless methods...
    expect(my::TokenType::CHAR_LITERAL, functionName);
  } else {
    throw std::runtime_error(
      "Syntax error at token: '" + currToken_.getValue() +
      "' (" + getTokenValue(currToken_.getType()) + "), Expected: LITERAL" +
      " || parseLiteral()"
      );
  }
}

void Parser::parseExpression() {
  parseComma();
}

void Parser::parseComma() {
  parseLogicalOr();
  if (currToken_.getType() == my::TokenType::COMMA) {
    advance(); // skip ','
    parseLogicalOr();
  }
}

void Parser::parseLogicalOr() {
  parseLogicalAnd();
  if (currToken_.getType() == my::TokenType::OR) {
    advance(); // skip '||'
    parseLogicalAnd();
  }
}

void Parser::parseLogicalAnd() {
  parseEqualityOperators();
  if (currToken_.getType() == my::TokenType::AND) {
    advance(); // skip '&&'
    parseEqualityOperators();
  }
}

void Parser::parseEqualityOperators() {
  parseRelationalOperators();
  if (currToken_.getType() == my::TokenType::EQ || currToken_.getType() == my::TokenType::NEQ) {
    advance(); // skip '=='/'!='
    parseRelationalOperators();
  }
}

void Parser::parseRelationalOperators() {
  parsePlusMinus();
  if (currToken_.getType() == my::TokenType::LT || currToken_.getType() == my::TokenType::GT) {
    advance(); // skip '<'/'>'
    parsePlusMinus();
  }
}

void Parser::parsePlusMinus() {
  parseMulDiv();
  if (currToken_.getType() == my::TokenType::PLUS || currToken_.getType() == my::TokenType::MINUS) {
    advance(); // skip '+'/'-'
    parseMulDiv();
  }
}

void Parser::parseMulDiv() {
  parseUnary();
  if (currToken_.getType() == my::TokenType::MUL || currToken_.getType() == my::TokenType::DIV) {
    advance(); // slip '*'/'\/'
    parseUnary();
  }
}

void Parser::parseUnary() {
  parseAtom();
  if (currToken_.getType() == my::TokenType::NOT) {
    advance(); // skip
  }
}

void Parser::parseAtom() {
  const std::string functionName = "parseAtom()";

  if (currToken_.getType() == my::TokenType::KEYWORD &&
    (currToken_.getValue() == "true" || currToken_.getValue() == "false")) {
    advance(); // 'true'/'false'
  } else if (currToken_.getType() == my::TokenType::IDENTIFIER) {
    parseIdentifier(); // 'identifier'
  } else if (currToken_.getType() == my::TokenType::LPAREN) { // '(expression)'
    advance();
    parseExpression();
    expect(my::TokenType::RPAREN, functionName);
  } else {
    parseLiteral(); // 'literal'
  }
}

void Parser::parseIndex() {
  const std::string functionName = "parseIndex()";

  if (currToken_.getType() == my::TokenType::IDENTIFIER) {
    expect(my::TokenType::IDENTIFIER, functionName);
    // parseIdentifier();
  } else if (currToken_.getType() == my::TokenType::INTEGER_LITERAL) {
    expect(my::TokenType::INTEGER_LITERAL, functionName);
  } else {
    throw std::runtime_error(
    "Syntax error at token: '" + currToken_.getValue() +
     "' (" + getTokenValue(currToken_.getType()) + "), Expected: " +
     getTokenValue(my::TokenType::IDENTIFIER) + " or " + getTokenValue(my::TokenType::INTEGER_LITERAL) +
     " || parseIndex()"
      );
  }
}

void Parser::parseType() {
  const std::string functionName = "parseType()";

  if (isType(currToken_)) {
    if (currToken_.getType() == my::TokenType::ARRAY) {
      advance();
      expect(my::TokenType::LT, functionName);
      parseType();
      expect(my::TokenType::GT, functionName);
    } else {
    advance();
    }
  } else {
    throw std::runtime_error("Syntax error: invalid type '" + currToken_.getValue() + "' || pareType()");
  }
}

void Parser::parseIdentifier() { // maybe will be rewriting to expect(my::TokenType::IDENTIFIER);
  const std::string functionName = "parseIdentifier()";

  expect(my::TokenType::IDENTIFIER, functionName);
}
