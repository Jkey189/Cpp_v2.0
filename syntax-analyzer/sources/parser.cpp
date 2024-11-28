#include "../headers/parser.h"

void Parser::parseProgram() {
  while (currToken_.getType() != my::TokenType::END) {
    parseDeclaration();
  }
}

void Parser::parseDeclaration() {
  if (currToken_.getValue() == "func") {
    parserAdvance();
    if (isType(currToken_)) {
      parseFunction();
    } else {
      throw std::runtime_error("Syntax error: unexpected token | `parseDeclaration()' error");
    }
  } else if (isType(currToken_)) {
    parseStatement();
  } else if (lexer_.peek().getType() == my::TokenType::IDENTIFIER) { // ???
    parseVariable();
  } else {
    throw std::runtime_error("Syntax error: unexpected token | `parseDeclaration()' error");
  }
}

void Parser::parseFunction() {
  if (currToken_.getValue() == "func") {
    parserAdvance();
    parseType();
    parseIdentifier();
    expect(my::TokenType::LPAREN);
    parseParams();
    expect(my::TokenType::RPAREN);
    parseBlock();
  } else {
    throw std::runtime_error("Syntax error: unexpected token | `parseFunction()' error");
  }
}

void Parser::parseParams() {
  parseParam();
  while (currToken_.getValue() == ",") {
    parserAdvance();
    parseParam();
  }
}

void Parser::parseParam() {
  parseType();
  expect(my::TokenType::IDENTIFIER);
}

void Parser::parseVariable() {
  expect(my::TokenType::IDENTIFIER);
}

void Parser::parseBlock() {
  expect(my::TokenType::LBRACE);
  while (currToken_.getType() != my::TokenType::RBRACE) {
    if (currToken_.getValue() == "\\") {
      parseStatement();
    } else {
      throw std::runtime_error("Syntax error: unexpected token | `parseBlock()' error");
    }
  }
  expect(my::TokenType::RBRACE);
}

void Parser::parseIf() {
  expect(my::TokenType::IF);
  expect(my::TokenType::LPAREN);
  if (currToken_.getType() == my::TokenType::RPAREN) {
    throw std::runtime_error("Syntax error: unexpected token | `parseIf()' error");
  }
  parseExpression();
  expect(my::TokenType::RPAREN);
  parseBlock();

  if (currToken_.getType() == my::TokenType::ELSE) {
    parserAdvance();
    parseBlock();

    /*if (lexer_.peek().getType() == my::TokenType::IF) {
        parseIf();
      } else if (lexer_.peek().getType() == my::TokenType::LBRACE) {
        parserAdvance();
        parseBlock();
      } else {
        throw std::runtime_error("Syntax error: unexpected token");
      }*/
  }
}

void Parser::parseLoop() {
  if (currToken_.getType() == my::TokenType::WHILE) {
    parserAdvance();
    expect(my::TokenType::LBRACE);
    parseExpression();
    expect(my::TokenType::RBRACE);
    parseBlock();
  } else if (currToken_.getType() == my::TokenType::FOR) {
    parserAdvance();
    expect(my::TokenType::LBRACE);
    if (currToken_.getType() != my::TokenType::SEMICOLON) {
      parseInitialization();
    }
    expect(my::TokenType::SEMICOLON);
    if (currToken_.getType() != my::TokenType::SEMICOLON) {
      parseExpression();
    }
    expect(my::TokenType::SEMICOLON);
    if (currToken_.getType() != my::TokenType::SEMICOLON) {
      parseStep();
    }
    expect(my::TokenType::RBRACE);
  } else {
    throw std::runtime_error("Syntax error: unexpected token | `parseLoop()' error");
  }
}

void Parser::parseStep() {
  parseOperator();
}

void Parser::parseSwitch() {
  expect(my::TokenType::SWITCH);
  expect(my::TokenType::LPAREN);
  parseExpression();
  expect(my::TokenType::RPAREN);
  expect(my::TokenType::LBRACE);
  while (currToken_.getType() == my::TokenType::CASE) {
    parseLiteral();
    expect(my::TokenType::COLON);
    parseBlock();
  }
  if (currToken_.getType() == my::TokenType::DEFAULT) {
    expect(my::TokenType::COLON);
    parseBlock();
  } else {
    throw std::runtime_error("Syntax error: unexpected token | `parseSwitch()' error");
  }
  expect(my::TokenType::RBRACE);
}

void Parser::parseLiteral() {
  if (isNumber(currToken_.getValue())) {
    if (currToken_.getValue().find('.')) {
      parseFloatLiteral();
    } else {
      parseIntegerLiteral();
    }
  } else if (currToken_.getType() == my::TokenType::QUOTEMARK) {
    parserAdvance();
    parseStringLiteral();
    expect(my::TokenType::QUOTEMARK);
  } else {
    throw std::runtime_error("Syntax error: unexpected token | `parseLiteral()' error");
  }
}

void Parser::parseIntegerLiteral() {
  expect(my::TokenType::INTEGER_LITERAL);
}

void Parser::parseFloatLiteral() {
  expect(my::TokenType::FLOAT_LITERAL);
}

void Parser::parseStringLiteral() {
  expect(my::TokenType::STRING_LITERAL);
}

void Parser::parseExpression() {
  parseLogicalOr();
}

void Parser::parseLogicalOr() {
  parseLogicalAnd();
  if (currToken_.getType() == my::TokenType::AND) {
    parserAdvance();
    parseLogicalAnd();
  }
}

void Parser::parseLogicalAnd() {
  parseEqNotEq();
  if (currToken_.getType() == my::TokenType::EQ || currToken_.getType() == my::TokenType::NEQ) {
    parserAdvance();
    parseEqNotEq();
  }
}

void Parser::parseEqNotEq() {
  parseComparison();
  if (currToken_.getType() == my::TokenType::EQ || currToken_.getType() == my::TokenType::NEQ) {
    parserAdvance();
    parseComparison();
  }
}

void Parser::parseComparison() {
  parsePlusMinus();
  if (currToken_.getType() == my::TokenType::LT || currToken_.getType() == my::TokenType::GT) {
    parserAdvance();
    parsePlusMinus();
  }
}

void Parser::parsePlusMinus() {
  parseMulDiv();
  if (currToken_.getType() == my::TokenType::PLUS || currToken_.getType() == my::TokenType::MINUS) {
    parserAdvance();
    parseMulDiv();
  }
}

void Parser::parseMulDiv() {
  parseUnaryExpression();
  if (currToken_.getType() == my::TokenType::MUL || currToken_.getType() == my::TokenType::DIV) {
    parserAdvance();
    parseUnaryExpression();
  }
}

void Parser::parseUnaryExpression() {
  parseAtomExpression();
  if (currToken_.getType() == my::TokenType::NOT || currToken_.getType() == my::TokenType::NOT) {
    parserAdvance();
    parseAtomExpression();
  }
}

void Parser::parseAtomExpression() {
  if (currToken_.getType() == my::TokenType::LPAREN) {
    parserAdvance();
    parseExpression();
    expect(my::TokenType::RPAREN);
  } else if (currToken_.getType() == my::TokenType::IDENTIFIER) {
    parseIdentifier();
    if (currToken_.getType() == my::TokenType::LBRACKET) {
      parserAdvance();
      parseExpression();
      expect(my::TokenType::RBRACKET);
    }
  } else if (isNumber(currToken_.getValue()) || currToken_.getType() == my::TokenType::QUOTEMARK) {
    parseLiteral();
  } else {
    throw std::runtime_error("Syntax error: unexpected token | `parseAtomExpression()' error");
  }
}

void Parser::parseType() {
  if (isType(currToken_)) {
    if (currToken_.getValue() == "array") {
      expect(my::TokenType::LT);
      // parserAdvance();
      parseType();
      expect(my::TokenType::GT);
    }
  } else {
    throw std::runtime_error("Syntax error: unexpected token");
  }
}
