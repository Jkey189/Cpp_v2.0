#ifndef PARSER_H
#define PARSER_H


#include <complex>

#include "../../global_functions/global_funcs.h"
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
    if (currCount < lexer_.getTokens().size()) {
      return lexer_.getTokens()[currCount++];
    }
    throw std::runtime_error("Syntax error: unexpected end of input.");
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
    if (currToken_.getType() != type) { // make expected throw more informative
      throw std::runtime_error("Syntax error at token: '" + currToken_.getValue() +
        "' (line: " +  std::to_string(currToken_.getLine()) +
        ", column: " + std::to_string(currToken_.getColumn()) +
        "), Expected: " + getTokenValue(type));
    }
    parserAdvance();
  }

  void parseProgram();

  void parseDeclaration();

  void parseInstruction();

  void parseFunction();

  void parseIdentification();
};


#endif //PARSER_H
