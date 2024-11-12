#ifndef TOKENS_H
#define TOKENS_H


#include "../../includes/libraries.h"


enum class TokenType {
  KEYWORD,
  IDENTIFIER,
  INTEGER_NUMBER,
  FLOAT_NUMBER,
  OPERATOR,
  PUNCTUATOR,
  // WHITESPACE,
  UNKNOWN
};

class Token {
public:
  Token(TokenType t, std::string s, int line, int column) :
  type_(t), value_(std::move(s)), position_({line, column}) {}

  Token(TokenType t, std::string s) :
  type_(t), value_(std::move(s)), position_({0,0}) {}

  [[nodiscard]] TokenType getLexeme() const;
  [[nodiscard]] std::string getValue() const;

  [[nodiscard]] size_t getLine() const;
  [[nodiscard]] size_t getColumn() const;


private:
  TokenType type_;
  std::string value_;

  std::pair<size_t, size_t> position_;
};


#endif //TOKENS_H
