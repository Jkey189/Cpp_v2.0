#ifndef PARSER_H
#define PARSER_H


#include <complex>

#include "../../includes/libraries.h"
#include "../../lexical-analyzer/headers/lexer.h"


class Parser {
public:
  explicit Parser(LexicalAnalyzer& lexer): lexer_(lexer), currToken_(lexer.getLex()) {
    /*currToken_ = lexer_.getLex();*/
    parserAdvance();
  }

  void program() {
    parserAdvance();
    /*parseProgram();*/
  }

  Token parserAdvance() {
    return lexer_.getTokens()[currCount++];
  }

  static bool isType(const Token& token) {
    return token.getType() == my::TokenType::INT || token.getType() == my::TokenType::FLOAT ||
      token.getType() == my::TokenType::CHAR || token.getType() == my::TokenType::BOOL ||
        token.getType() == my::TokenType::VOID ||token.getType() == my::TokenType::STRING ||
          token.getType() == my::TokenType::ARRAY || token.getType() == my::TokenType::IDENTIFIER;
  }

  [[nodiscard]] Token getCurrToken() const { return currToken_; }


private:
  size_t currCount = 0;
  LexicalAnalyzer lexer_;
  Token currToken_;


  static bool isNumber(const Token& num) {
    for (const auto elem : num.getValue()) {
      if ((elem < '0' || elem > '9') && elem != '.') {
        return false;
      }
    }
    return true;
  }

  static bool isWord(const Token& word) {
    for (const char letter : word.getValue()) {
      if ((letter < 'a' || letter > 'z') && (letter < 'A' || letter > 'Z') && letter != '_') {
        return false;
      }
    }
    return true;
  }


  void expect(const my::TokenType type) {
    if (currToken_.getType() != type) {
      throw std::runtime_error("Syntax error: unexpected token | wrong token type");
    }
    parserAdvance();
  }

  void parseProgram();

  void parseDeclaration();

  void parseInstruction();

  void parseFunction();

  void parseBlock();

  void parseInput();

  void parseOutput();

  void parseExpression();

  void parseLoop();

  void parseSwitch();

  void parseIdentifier();

  void parseInitialization();

  void parseStep();

  void parseNumber();

  void parseLiteral();

  void parseIntegerLiteral();

  void parseFloatLiteral();

  void parseStringLiteral();

  void parseLogicalOr();

  void parseLogicalAnd();

  void parseLogicalComparison();

  void parsePlusMinus();

  void parseMulDiv();

  void parseUnary();

  void parseComma();

  void parseAtom();

  void parseType();
};


#endif //PARSER_H
