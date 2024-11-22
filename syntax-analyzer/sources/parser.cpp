#include "../headers/parser.h"

void Parser::parseProgram() {
  while (currToken_.getType() != TokenType::END) {
    parseDeclaration();
  }
}

void Parser::parseDeclaration() {
  if (currToken_.getValue() == "func") {
    advance();
    if (isType(currToken_)) {
      parseFunction();
    } else {
      throw std::runtime_error("Syntax error: unexpected token | `parseDeclaration()' error");
    }
  } else if (isType(currToken_)) {
    parseStatement();
  } else if (lexer_.peek().getType() == TokenType::IDENTIFIER) { // ???
    parseVariable();
  } else {
    throw std::runtime_error("Syntax error: unexpected token | `parseDeclaration()' error");
  }
}

void Parser::parseFunction() {
  if (currToken_.getValue() == "func") {
    advance();
    parseType();
    parseIdentifier();
    expect(TokenType::LPAREN);
    parseParams();
    expect(TokenType::RPAREN);
    parseBlock();
  } else {
    throw std::runtime_error("Syntax error: unexpected token | `parseFunction()' error");
  }
}

void Parser::parseParams() {
  parseParam();
  while (currToken_.getValue() == ",") {
    advance();
    parseParam();
  }
}

void Parser::parseParam() {
  parseType();
  expect(TokenType::IDENTIFIER);
}

void Parser::parseVariable() {
  expect(TokenType::IDENTIFIER);
}

void Parser::parseBlock() {
  expect(TokenType::LBRACE);
  while (currToken_.getType() != TokenType::RBRACE) {
    if (currToken_.getValue() == "\\") {
      parseStatement();
    } else {
      throw std::runtime_error("Syntax error: unexpected token | `parseBlock()' error");
    }
  }
  expect(TokenType::RBRACE);
}

void Parser::parseIf() {
  expect(TokenType::IF);
  expect(TokenType::LPAREN);
  parseExpression();
  expect(TokenType::RPAREN);
  parseBlock();

  if (currToken_.getType() == TokenType::ELSE) {
    advance();
    parseBlock();

    /*if (lexer_.peek().getType() == TokenType::IF) {
        parseIf();
      } else if (lexer_.peek().getType() == TokenType::LBRACE) {
        advance();
        parseBlock();
      } else {
        throw std::runtime_error("Syntax error: unexpected token");
      }*/
  }
}

void Parser::parseLoop() {
  if (currToken_.getType() == TokenType::WHILE) {
    advance();
    expect(TokenType::LBRACE);
    parseExpression();
    expect(TokenType::RBRACE);
    parseBlock();
  } else if (currToken_.getType() == TokenType::FOR) {
    advance();
    expect(TokenType::LBRACE);
    if (currToken_.getType() != TokenType::SEMICOLON) {
      parseInitialization();
    }
    expect(TokenType::SEMICOLON);
    if (currToken_.getType() != TokenType::SEMICOLON) {
      parseExpression();
    }
    expect(TokenType::SEMICOLON);
    if (currToken_.getType() != TokenType::SEMICOLON) {
      parseStep();
    }
    expect(TokenType::RBRACE);
  } else {
    throw std::runtime_error("Syntax error: unexpected token | `parseLoop()' error");
  }
}

void Parser::parseStep() {
  parseOperator();
}

void Parser::parseSwitch() {
  expect(TokenType::SWITCH);
  expect(TokenType::LPAREN);
  parseExpression();
  expect(TokenType::RPAREN);
  expect(TokenType::LBRACE);
  while (currToken_.getType() == TokenType::CASE) {
    parseLiteral();
    expect(TokenType::COLON);
    parseBlock();
  }
  if (currToken_.getType() == TokenType::DEFAULT) {
    expect(TokenType::COLON);
    parseBlock();
  } else {
    throw std::runtime_error("Syntax error: unexpected token | `parseSwitch()' error");
  }
  expect(TokenType::RBRACE);
}

void Parser::parseLiteral() {
  if (isNumber(currToken_.getValue())) {
    if (currToken_.getValue().find('.')) {
      parseFloatLiteral();
    } else {
      parseIntegerLiteral();
    }
  } else if (currToken_.getType() == TokenType::QUOTEMARK) {
    advance();
    parseStringLiteral();
    expect(TokenType::QUOTEMARK);
  } else {
    throw std::runtime_error("Syntax error: unexpected token | `parseLiteral()' error");
  }
}

void Parser::parseIntegerLiteral() {
  expect(TokenType::INTEGER_LITERAL);
}

void Parser::parseFloatLiteral() {
  expect(TokenType::FLOAT_LITERAL);
}

void Parser::parseStringLiteral() {
  expect(TokenType::STRING_LITERAL);
}

void Parser::parseExpression() {
  parseLogicalOr();
}

void Parser::parseLogicalOr() {
  parseLogicalAnd();
  if (currToken_.getType() == TokenType::AND) {
    advance();
    parseLogicalAnd();
  }
}

void Parser::parseLogicalAnd() {
  parseEqNotEq();
  if (currToken_.getType() == TokenType::EQ || currToken_.getType() == TokenType::NEQ) {
    advance();
    parseEqNotEq();
  }
}

void Parser::parseEqNotEq() {
  parseComparison();
  if (currToken_.getType() == TokenType::EQ || currToken_.getType() == TokenType::NEQ) {
    advance();
    parseComparison();
  }
}

void Parser::parseComparison() {
  parsePlusMinus();
  if (currToken_.getType() == TokenType::LT || currToken_.getType() == TokenType::GT) {
    advance();
    parsePlusMinus();
  }
}

void Parser::parsePlusMinus() {
  parseMulDiv();
  if (currToken_.getType() == TokenType::PLUS || currToken_.getType() == TokenType::MINUS) {
    advance();
    parseMulDiv();
  }
}

void Parser::parseMulDiv() {
  parseUnaryExpression();
  if (currToken_.getType() == TokenType::MUL || currToken_.getType() == TokenType::DIV) {
    advance();
    parseUnaryExpression();
  }
}

void Parser::parseUnaryExpression() {
  parseAtomExpression();
  if (currToken_.getType() == TokenType::NOT || currToken_.getType() == TokenType::NOT) {
    advance();
    parseAtomExpression();
  }
}

void Parser::parseAtomExpression() {
  if (currToken_.getType() == TokenType::LPAREN) {
    advance();
    parseExpression();
    expect(TokenType::RPAREN);
  } else if (currToken_.getType() == TokenType::IDENTIFIER) {
    parseIdentifier();
    if (currToken_.getType() == TokenType::LBRACKET) {
      advance();
      parseExpression();
      expect(TokenType::RBRACKET);
    }
  } else if (isNumber(currToken_.getValue()) || currToken_.getType() == TokenType::QUOTEMARK) {
    parseLiteral();
  } else {
    throw std::runtime_error("Syntax error: unexpected token | `parseAtomExpression()' error");
  }
}

void Parser::parseType() {
  if (isType(currToken_)) {
    if (currToken_.getValue() == "array") {
      expect(TokenType::LT);
      // advance();
      parseType();
      expect(TokenType::GT);
    }
  } else {
    throw std::runtime_error("Syntax error: unexpected token");
  }
}
