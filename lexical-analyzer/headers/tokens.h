#ifndef TOKENS_H
#define TOKENS_H


#include "../../includes/libraries.h"


enum class TokenType {
  KEYWORD,
  INT, FLOAT, CHAR, BOOL, VOID, STRING, ARRAY,

  NUMBER, IDENTIFIER, IF, ELSE, SWITCH, CASE, DEFAULT, FOR, WHILE, RETURN, BREAK, CONTINUE,

  COMMENT_LITERAL,
  INTEGER_LITERAL,
  FLOAT_LITERAL,
  STRING_LITERAL,
  BOOL_LITERAL,
  CHAR_LITERAL,

  IN, // >>
  OUT, // <<
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
  QUOTEMARK, // "
  NEXT_STATEMENT, // \

  LPAREN, // (
  RPAREN, // )
  LBRACE, // {
  RBRACE, // }
  LBRACKET, // [
  RBRACKET, // ]
  COMMA, // ,
  SEMICOLON, // ;
  COLON, // :

  UNKNOWN,
  END
};

class Token {
public:
  Token(TokenType t, std::string s, int line, int column) :
  type_(t), value_(std::move(s)), position_({line, column}) {}

  Token(TokenType t, std::string s) :
  type_(t), value_(std::move(s)), position_({0,0}) {}

  [[nodiscard]] TokenType getType() const;
  [[nodiscard]] std::string getValue() const;

  [[nodiscard]] size_t getLine() const;
  [[nodiscard]] size_t getColumn() const;


private:
  TokenType type_;
  std::string value_;

  std::pair<size_t, size_t> position_;
};


#endif //TOKENS_H
