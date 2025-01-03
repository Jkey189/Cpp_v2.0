#include "../headers/parser.h"


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
  expect(my::TokenType::KEYWORD); // 'func'
  advance();

  // is current token - type
  if (isType(currToken_)) {
    advance(); // function's type
  } else {
    throw std::runtime_error("Syntax error: Expected a type after 'func' || parseFunction()");
  }

  // check identifier
  expect(my::TokenType::IDENTIFIER); // name of function

  // check parameters
  expect(my::TokenType::LPAREN); // '('
  if (currToken_.getType() != my::TokenType::RPAREN) { // if we have any parameters
    parseParameters();
  }
  expect(my::TokenType::RPAREN); // ')'

  // check function's body (block)
  parseBlock();
  /*expect(my::TokenType::LBRACE); // '{'
  parseBlock();                  // block
  expect(my::TokenType::RBRACE); // '}'*/
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
  if (!isType(currToken_)) {
    throw std::runtime_error(
      "Syntax error: Expected type for parameter, found '" + currToken_.getValue() +
      "' (" + getTokenValue(currToken_.getType()) + ")." + " || parseParameter()"
      );
  }

  advance(); // skip type of variable
  expect(my::TokenType::IDENTIFIER); // name of variable
}

void Parser::parseBlock() {
  expect(my::TokenType::LBRACE); // '{'

  while (currToken_.getType() != my::TokenType::RBRACE) {
    if (currToken_.getType() == my::TokenType::END) {
      throw std::runtime_error("Syntax error: Unexpected end of input inside block || parseBlock()");
    }

    parseInstruction(); // parsing next instruction
  }

  expect(my::TokenType::RBRACE); // '}'
}

void Parser::parseInstruction() {
  if (currToken_.getType() == my::TokenType::RBRACE) {
    return; // we haven't any instructions
  }

  if (currToken_.getType() == my::TokenType::LBRACE) {
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
    // advance();
    parseSwitch();
  } else if (currToken_.getType() == my::TokenType::BREAK || currToken_.getType() == my::TokenType::CONTINUE) {
    advance(); // skip 'break' / 'continue'
    advance(); // skip ';'
  } else if (isType(currToken_)) {
    parseInitialization();
  } else if (currToken_.getType() == my::TokenType::IDENTIFIER &&
    lexer_.peek(currCount).getType() == my::TokenType::ASSIGN) {
    // advance();
    parseAssignment();
  } else if (currToken_.getType() == my::TokenType::RETURN) {
    advance(); // skip 'return'
    advance();
    // parseExpression();
  } else if (currToken_.getType() == my::TokenType::IDENTIFIER /*currToken_.getType() != my::TokenType::SEMICOLON*/) {
    advance();
    // parseExpression();
  } else if (currToken_.getType() == my::TokenType::SEMICOLON) {
    advance(); // skip ';'
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
  advance(); // skip 'cin'
  expect(my::TokenType::IN); // '>>'
  expect(my::TokenType::IDENTIFIER); // 'variable'
  while (currToken_.getType() != my::TokenType::SEMICOLON) {
    expect(my::TokenType::IN); // '>>'
    expect(my::TokenType::IDENTIFIER); // 'variable'
  }
  expect(my::TokenType::SEMICOLON); // ';'
}

void Parser::parseOutput() {
  advance(); // skip 'cout'
  expect(my::TokenType::OUT); // '<<'
  // parseExpression();
  expect(my::TokenType::IDENTIFIER); // 'identifier' - for the first time
  while (currToken_.getType() != my::TokenType::SEMICOLON) {
    expect(my::TokenType::OUT); // '<<'
    // parseExpression();
    expect(my::TokenType::IDENTIFIER); // 'identifier' - for the first time
  }
  expect(my::TokenType::SEMICOLON); // ';'
}

void Parser::parseConditional() {
  advance(); // skip 'if'
  expect(my::TokenType::LPAREN); // '('
  // parseExpression(); // for the first time we haven't any conditions
  expect(my::TokenType::RPAREN); // ');
  parseBlock(); // 'block'
}

void Parser::parseLoop() {
  if (currToken_.getType() == my::TokenType::WHILE) {
    advance(); // skip 'while'

    expect(my::TokenType::LPAREN); // '('
    // parseExpression(); // for the first time we haven't any conditions
    expect(my::TokenType::RPAREN); // ')'

    parseBlock(); // 'block' - loop's body
  } else if (currToken_.getType() == my::TokenType::FOR) {
    advance(); // skip 'for'

    expect(my::TokenType::LPAREN); // '('
    parseInitialization(); // for the first time we haven't any initializations
    expect(my::TokenType::SEMICOLON); // first ';' after initialization
    // parseExpression(); // for the first time we haven't any conditions
    expect(my::TokenType::SEMICOLON); // second ';' after condition
    // parseStep(); // for the first time we haven't any realizations of parseStep()
    expect(my::TokenType::RPAREN); // ')'

    parseBlock(); // 'block' - loop's body
  } else { // it useless, but - why not?
    throw std::runtime_error(
    "Syntax error at token: '" + currToken_.getValue() +
    "' (" + getTokenValue(currToken_.getType()) + "), Expected: FOR /or/ WHILE" +
    " || parseLoop()");
  }
}

void Parser::parseInitialization() {
  if (isType(currToken_)) {
    advance(); // variable's type
  } else {
    throw std::runtime_error("Syntax error: Expected a type after 'func' || parseInitialization()");
  }

  expect(my::TokenType::IDENTIFIER); // variable's name
  while (currToken_.getType() == my::TokenType::LBRACKET) { // is array's element ([i], [i][j], ...)
    advance(); // '['
    // parseIndex(); // for the first time we haven't any realizations of parseIndex()
    expect(my::TokenType::RBRACKET); // '['
  }
  if (currToken_.getType() != my::TokenType::SEMICOLON) {
    expect(my::TokenType::ASSIGN);
    advance();
  }
  // parseExpression(); // for the first time we haven't any expressions
  // expect(my::TokenType::SEMICOLON); // maybe useful !!!!!!!!!
}

void Parser::parseAssignment() {
  expect(my::TokenType::IDENTIFIER); // variable's name
  while (currToken_.getType() == my::TokenType::LBRACKET) { // is array's element ([i], [i][j], ...)
    advance(); // '['
    // parseIndex(); // for the first time we haven't any realizations of parseIndex()
    expect(my::TokenType::RBRACKET); // '['
  }
  if (currToken_.getType() != my::TokenType::SEMICOLON) {
    expect(my::TokenType::ASSIGN);
    advance();
  }
  // parseExpression(); // for the first time we haven't any expressions
  // expect(my::TokenType::SEMICOLON); // maybe useful !!!!!!!!!
}

void Parser::parseStep() {
  advance();
  // parseExpression(); // for the first time we haven't any expressions
}

void Parser::parseSwitch() { // TODO: fix unused ';' in the last instruction in 'case'
  advance(); // skip 'switch'
  expect(my::TokenType::LPAREN); // '('
  // parseExpression(); // for the first time we haven't any conditions
  expect(my::TokenType::RPAREN); // ')'

  expect(my::TokenType::LBRACE); // '{'
  while (currToken_.getType() == my::TokenType::CASE) {
    expect(my::TokenType::CASE);
    if (currToken_.getType() == my::TokenType::COMMENT_LITERAL) {
      throw std::runtime_error(
      "Syntax error at token: '" + currToken_.getValue() +
      "' (" + getTokenValue(currToken_.getType()) + "), Expected: NOT COMMENT LITERAL!!!" +
      " || parseSwitch()"
      );
    }
    parseLiteral(); // for the first time we haven't any realizations of parseLiteral()
    expect(my::TokenType::COLON);
    /*while (currToken_.getType() != my::TokenType::CASE &&
         currToken_.getType() != my::TokenType::DEFAULT &&
         currToken_.getType() != my::TokenType::RBRACE) {
      parseInstruction();
         }*/
    while (currToken_.getType() != my::TokenType::BREAK) {
      parseInstruction(); // 'instruction' in case
      // expect(my::TokenType::SEMICOLON); // TODO: check 52:8 (source_file.cppt / "Big example for parser")
    }
    expect(my::TokenType::BREAK); // 'break'
    expect(my::TokenType::SEMICOLON); // ';'
  }
  expect(my::TokenType::DEFAULT); // 'default'
  expect(my::TokenType::COLON); // ':'
  while (currToken_.getType() != my::TokenType::RBRACE) {
    parseInstruction(); // 'instruction' in default
  }
  expect(my::TokenType::RBRACE); // '}'
}

void Parser::parseLiteral() {
  if (currToken_.getType() == my::TokenType::COMMENT_LITERAL) {
    // parseCommentLiteral(); // maybe useless methods...
    expect(my::TokenType::COMMENT_LITERAL);
  } else if (currToken_.getType() == my::TokenType::INTEGER_LITERAL) {
    // parseIntegerLiteral(); // maybe useless methods...
    expect(my::TokenType::INTEGER_LITERAL);
} else if (currToken_.getType() == my::TokenType::FLOAT_LITERAL) {
    // parseFloatLiteral(); // maybe useless methods...
    expect(my::TokenType::FLOAT_LITERAL);
  } else if (currToken_.getType() == my::TokenType::STRING_LITERAL) {
    // parseStringLiteral(); // maybe useless methods...
    expect(my::TokenType::STRING_LITERAL);
  } else if (currToken_.getType() == my::TokenType::CHAR_LITERAL) {
    // parseCharLiteral(); // maybe useless methods...
    expect(my::TokenType::CHAR_LITERAL);
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
  if (currToken_.getType() == my::TokenType::NOT || currToken_.getType() == my::TokenType::MINUS) {
    advance(); // skip
  }
}

void Parser::parseAtom() {
  advance();
}
