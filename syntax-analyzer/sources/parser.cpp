#include "../headers/parser.h"


std::string typeToString(const my::TokenType& tokenType) {
  switch (tokenType) {
    case my::TokenType::INTEGER_LITERAL:
      return "INTEGER_LITERAL";
    case my::TokenType::FLOAT_LITERAL:
      return "FLOAT_LITERAL";
    case my::TokenType::STRING_LITERAL:
      return "STRING_LITERAL";
    case my::TokenType::BOOL_LITERAL:
      return "BOOL_LITERAL";
    case my::TokenType::CHAR_LITERAL:
      return "CHAR_LITERAL";
    default:
      return "UNKNOWN";
  }
}


void Parser::parseProgram() {
  while (currToken_.getType() != my::TokenType::END) {
    parseDeclaration();
  }
}

void Parser::parseDeclaration() {
  if (currToken_.getType() == my::TokenType::KEYWORD && currToken_.getValue() == "func") {
    parseFunction();
  } else if (currToken_.getType() == my::TokenType::IDENTIFIER) {
    parseIdentifier();
    parseIdentifier();
  } else {
    parseInstruction();
  }
}

void Parser::parseFunction() {
  expect(my::TokenType::KEYWORD);
  parserAdvance();
}

void Parser::parseInstruction() {
  if (lexer_.peek(currCount).getType() == my::TokenType::LBRACE) {
    parserAdvance();
    parseBlock();
    expect(my::TokenType::RBRACE);
  } else if (currToken_.getType() == my::TokenType::KEYWORD && currToken_.getValue() == "cin") {
    parserAdvance();
    parseInput();
  } else if (currToken_.getType() == my::TokenType::KEYWORD && currToken_.getValue() == "cout") {
    parserAdvance();
    parseOutput();
  } else if (currToken_.getType() == my::TokenType::IF) {
    parserAdvance();
    expect(my::TokenType::LPAREN);
    parseExpression();
    expect(my::TokenType::RPAREN);
    parseBlock();
    if (currToken_.getType() == my::TokenType::ELSE) {
      parserAdvance();
      parseBlock();
    }
  } else if (currToken_.getType() == my::TokenType::FOR || currToken_.getType() == my::TokenType::WHILE) {
    parserAdvance();
    parseLoop();
  } else if (currToken_.getType() == my::TokenType::SWITCH) {
    parserAdvance();
    parseSwitch();
  } else if (currToken_.getType() == my::TokenType::IDENTIFIER) {
    parserAdvance();
    expect(my::TokenType::ASSIGN);
    parseExpression();
    expect(my::TokenType::SEMICOLON);
  } else if (currToken_.getType() == my::TokenType::BREAK) {
    parserAdvance();
    expect(my::TokenType::SEMICOLON);
  } else if (currToken_.getType() == my::TokenType::CONTINUE) {
    parserAdvance();
    expect(my::TokenType::SEMICOLON);
  } else if (currToken_.getType() == my::TokenType::SEMICOLON) {
    parserAdvance();
  } else {
    parseExpression();
  }
}

void Parser::parseBlock() {
  expect(my::TokenType::LBRACE);
  parseInstruction();
  if (currToken_.getType() == my::TokenType::NEXT_STATEMENT) {
    parseInstruction();
  }
  expect(my::TokenType::RBRACE);
}

void Parser::parseInput() {
  expect(my::TokenType::IN);
  parseIdentifier();
  while (lexer_.peek(currCount).getType() == my::TokenType::IN) {
    parseIdentifier();
  }
  expect(my::TokenType::SEMICOLON);
}

void Parser::parseOutput() {
  expect(my::TokenType::OUT);
  parseExpression();
  while (currToken_.getType() == my::TokenType::OUT) {
    parseExpression();
  }
  expect(my::TokenType::SEMICOLON);
}

void Parser::parseExpression() {
  parseComma();
  if (currToken_.getType() == my::TokenType::COMMA) {
    parserAdvance();
    parseComma();
  }
}

void Parser::parseLoop() {
  if (currToken_.getType() == my::TokenType::FOR) {
    expect(my::TokenType::LPAREN);
    parseInitialization();
    expect(my::TokenType::SEMICOLON);
    parseExpression();
    expect(my::TokenType::SEMICOLON);
    parseStep();
    expect(my::TokenType::RPAREN);
  } else if (currToken_.getType() == my::TokenType::WHILE) {

  }
}

void Parser::parseInitialization() {
  if (!isType(currToken_)) {
    throw std::runtime_error("Syntax error: '" + currToken_.getValue()  + "' is not a type");
  }
  parseIdentifier();
  if (currToken_.getType() == my::TokenType::ASSIGN) {
    parserAdvance();
    if (isNumber(currToken_)) {
      parseNumber();
    } else {
      parseIdentifier();
    }
    expect(my::TokenType::SEMICOLON);
  }
}

void Parser::parseSwitch() {
  expect(my::TokenType::LPAREN);
  parseExpression();
  expect(my::TokenType::RPAREN);
  expect(my::TokenType::LBRACE);
  while (currToken_.getType() != my::TokenType::DEFAULT) {
    if (currToken_.getType() != my::TokenType::CASE) {
      throw std::runtime_error("Syntax error: " + currToken_.getValue() +
        " was not declared in this scope; did you mean 'case'?");
    }
    parserAdvance();

  }
  expect(my::TokenType::RBRACE);
}

void Parser::parseIdentifier() {
  expect(my::TokenType::IDENTIFIER);
}

void Parser::parseStep() {
  parseExpression();
}

void Parser::parseNumber() {
  if (currToken_.getType() != my::TokenType::INTEGER_LITERAL) {
    if (currToken_.getType() != my::TokenType::FLOAT_LITERAL) {
      throw std::runtime_error("Syntax error: invalid conversion from '" +
        typeToString(currToken_.getType()) + "' to 'FLOAT_LITERAL'");
    }
    throw std::runtime_error("Syntax error: invalid conversion from '" +
      typeToString(currToken_.getType()) + "' to 'INTEGER_LITERAL'");
  }
  parserAdvance();
}

void Parser::parseLiteral() {
  if (currToken_.getType() == my::TokenType::INTEGER_LITERAL) {
    parseIntegerLiteral();
  }
  if (currToken_.getType() == my::TokenType::FLOAT_LITERAL) {
    parseFloatLiteral();
  }
  if (currToken_.getType() == my::TokenType::STRING_LITERAL) {
    parseStringLiteral();
  }
  throw std::runtime_error("Syntax error: invalid conversion to 'VARIABLE_LITERAL'");
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

void Parser::parseComma() {
  parseLogicalOr();
  if (currToken_.getType() == my::TokenType::COMMA) {
    parserAdvance();
    parseLogicalOr();
  }
}

void Parser::parseLogicalOr() {
  parseLogicalAnd();
  if (currToken_.getType() == my::TokenType::AND) {
    parserAdvance();
    parseLogicalAnd();
  }
}

void Parser::parseLogicalAnd() {
  parseLogicalComparison();
  if (currToken_.getType() == my::TokenType::OR) {
    parserAdvance();
    parseLogicalComparison();
  }
}

void Parser::parseLogicalComparison() {
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
  parseUnary();
  if (currToken_.getType() == my::TokenType::MUL || currToken_.getType() == my::TokenType::DIV) {
    parserAdvance();
    parseUnary();
  }
}

void Parser::parseUnary() {
  parseAtom();
  if (currToken_.getType() == my::TokenType::NOT || currToken_.getType() == my::TokenType::MINUS) {
    parserAdvance();
    parseAtom();
  }
}

void Parser::parseAtom() {
  if (currToken_.getType() == my::TokenType::IDENTIFIER) {
    parseIdentifier();
  } else if (currToken_.getType() == my::TokenType::LPAREN) {
    parserAdvance();
    parseExpression();
    expect(my::TokenType::RPAREN);
  } else {
    parseLiteral();
  }
}

void Parser::parseType() {
  if (isType(currToken_)) {
    if (currToken_.getType() == my::TokenType::ARRAY) {
      expect(my::TokenType::LT);
      parseType();
      expect(my::TokenType::GT);
    }
    parserAdvance();
  } else {
    throw std::runtime_error("Syntax error: " + currToken_.getValue() +
        " was not declared in this scope; did you mean ' " +
        ((currToken_.getValue().size() == 3) ? "int" :
          (currToken_.getValue().size() == 4 ? "char /or/ bool /or/ void /or/" :
            (currToken_.getValue().size() == 5) ? "float /or/ array" : "string")) + " '?");
  }
}
