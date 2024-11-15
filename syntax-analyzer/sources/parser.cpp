#include "../headers/parser.h"


void Parser::parse() {
  advance();
  parseProgram();
}

void Parser::parseProgram() {
  while(currToken_.getType() != TokenType::END) {
    parseDeclaration();
  }
}

void Parser::parseDeclaration() {
  if (isType(currToken_)) {
    Token nextToken = lexer_.peek();

    if (nextToken.getType() == TokenType::LPAREN) {
      parseFunction();
    } else {
      parseVariable();
    }
  } else {
    throw std::runtime_error("Expected declaration");
  }
}

void Parser::parseFunction() {
  parseType();
  expect(TokenType::IDENTIFIER);
  expect(TokenType::LPAREN);

  if (currToken_.getType() != TokenType::RPAREN) {
    parseParameters();
  }

  expect(TokenType::RPAREN);
  parseBlock();
}

void Parser::parseParameters() {
  parseParameter();

  while (currToken_.getType() != TokenType::COMMA) {
    advance();
    parseParameter();
  }
}

void Parser::parseParameter() {
  parseType();
  expect(TokenType::IDENTIFIER);
}

void Parser::parseVariable() {
  parseType();
  expect(TokenType::IDENTIFIER);

  if (currToken_.getType() == TokenType::ASSIGN) {
    advance();
    parseExpression();
  }

  expect(TokenType::SEMICOLON);
}

void Parser::parseBlock() {
  expect(TokenType::LBRACE);

  while (currToken_.getType() != TokenType::RBRACE) {
    parseStatement();
  }

  expect(TokenType::RBRACE);
}

void Parser::parseStatement() {
  if (currToken_.getType() == TokenType::IF) {
    parseIfStatement();
  } else if (currToken_.getType() == TokenType::WHILE || currToken_.getType() == TokenType::FOR) {
    parseLoop();
  } else if (currToken_.getType() == TokenType::SWITCH) {
    parseSwitch();
  } else {
    throw std::runtime_error("Unexpected statement");
  }
}

void Parser::parseOperator() {
  if (currToken_.getType() == TokenType::IDENTIFIER) {
    advance();
    expect(TokenType::ASSIGN);
    parseExpression();
    expect(TokenType::SEMICOLON);
  } else if (currToken_.getType() == TokenType::RETURN) {
    advance();
    parseExpression();
    expect(TokenType::SEMICOLON);
  } else {
    throw std::runtime_error("Unexpected statement");
  }
}

void Parser::parseIfStatement() {
  expect(TokenType::IF);
  expect(TokenType::LPAREN);
  parseExpression();
  expect(TokenType::RPAREN);
  parseBlock();

  if (currToken_.getType() == TokenType::ELSE) {
    advance();
    parseBlock();
  }
}

void Parser::parseLoop() {
  if (currToken_.getType() == TokenType::WHILE) {
    advance();
    expect(TokenType::LPAREN);
    parseExpression();
    expect(TokenType::RPAREN);
    parseBlock();
  } else if (currToken_.getType() == TokenType::FOR) {
    advance();
    expect(TokenType::LPAREN);

    if (isType(currToken_)) {
      parseVariable();
    }
    expect(TokenType::SEMICOLON);

    if (currToken_.getType() != TokenType::SEMICOLON) {
      parseExpression();
    }
    expect(TokenType::SEMICOLON);

    if (currToken_.getType() != TokenType::RPAREN) {
      parseOperator();
    }
    expect(TokenType::RPAREN);
    parseBlock();
  } else {
    throw std::runtime_error("Unexpected statement");
  }
}

void Parser::parseSwitch() {
  expect(TokenType::SWITCH);
  expect(TokenType::LPAREN);
  parseExpression();
  expect(TokenType::RPAREN);
  expect(TokenType::LBRACE);

  while (currToken_.getType() == TokenType::CASE) {
    advance();
    parseLiteral();
    expect(TokenType::COLON);
    parseBlock();
  }

  if (currToken_.getType() == TokenType::DEFAULT) {
    advance();
    expect(TokenType::COLON);
    parseBlock();
  }

  expect(TokenType::RBRACE);
}

void Parser::parseExpression() {
  parseLogicalOr();
}

void Parser::parseLogicalOr() {
  parseLogicalAnd();

  while (currToken_.getType() == TokenType::OR) {
    advance();
    parseLogicalAnd();
  }
}

void Parser::parseLogicalAnd() {
  parseEqNeq();

  while (currToken_.getType() == TokenType::EQ || currToken_.getType() == TokenType::NEQ) {
    advance();
    parseEqNeq();
  }
}

void Parser::parseEqNeq() {
  parseLowGreat();

  while (currToken_.getType() == TokenType::EQ || currToken_.getType() == TokenType::NEQ) {
    advance();
    parseLowGreat();
  }
}

void Parser::parseLowGreat() {
  parsePlusMinus();

  while (currToken_.getType() == TokenType::LT || currToken_.getType() == TokenType::GT) {
    advance();
    parsePlusMinus();
  }
}

void Parser::parsePlusMinus() {
  parseMulDiv();

  while (currToken_.getType() == TokenType::PLUS || currToken_.getType() == TokenType::MINUS) {
    advance();
    parseMulDiv();
  }
}

void Parser::parseMulDiv() {
  parseExpression();

  while (currToken_.getType() == TokenType::MUL || currToken_.getType() == TokenType::DIV) {
    advance();
    parseExpression();
  }
}

void Parser::parseType() {
  if (isType(currToken_)) {
    advance();

    if (currToken_.getType() == TokenType::ARRAY) {
      expect(TokenType::LT);
      parseType();
      expect(TokenType::GT);
    }
  } else {
    throw std::runtime_error("Unexpected statement");
  }
}

void Parser::parseLiteral() {
  if (currToken_.getType() == TokenType::INTEGER_LITERAL ||
    currToken_.getType() == TokenType::FLOAT_LITERAL ||
    currToken_.getType() == TokenType::STRING_LITERAL) {
  advance();
  } else {
    throw std::runtime_error("Literal expected");
  }
}
