#ifndef TOKENS_H
#define TOKENS_H


#include "../../includes/libraries.h"


enum class TokenType {
  INT, FLOAT, CHAR, BOOL, VOID, STRING, ARRAY,

  IDENTIFIER, IF, ELSE, SWITCH, CASE, FOR, WHILE, DEFAULT, RETURN, BREAK, CONTINUE,

  ASSIGN, // =
  PLUS, // +
  MINUS, // -
  MUL, // *
  DIV, // /
  LT, // <
  GT, // >
  EQ, // ==
  NEQ, // !=
  AND, // &&
  OR, // ||
  NOT, // !

  LPAREN, // (
  RPAREN, // )
  LBRACE, // {
  RBRACE, // }
  LBRACKET, // [
  RBRACKET, // ]
  COMMA, // ,
  SEMICOLON, // ;

  END
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
