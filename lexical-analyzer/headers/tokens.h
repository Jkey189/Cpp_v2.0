#ifndef TOKENS_H
#define TOKENS_H


#include "../../includes/libraries.h"


enum class TokenType {
  INTEGER_LITERAL,
  FLOAT_LITERAL,
  STRING_LITERAL,
  LOGICAL_LITERAL,
  INTEGER_TYPE,
  FLOAT_TYPE,
  STRING_TYPE,
  LOGICAL_TYPE,
  KEYWORD,
  IDENTIFIER,
  OPERATOR,
  PUNCTUATOR,
  UNKNOWN
};

class Token {
public:
  Token(TokenType t, std::string s, int line, int column) :
  type_(t), value_(std::move(s)), position_({line, column}) {}

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
