#ifndef TOKENS_H
#define TOKENS_H


#include "../../includes/libraries.h"

namespace my {
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
}

/*inline const char* to_string(my::TokenType e) {
  switch (e) {
    case my::TokenType::KEYWORD: return "KEYWORD";
    case my::TokenType::INT: return "INT";
    case my::TokenType::FLOAT: return "FLOAT";
    case my::TokenType::CHAR: return "CHAR";
    case my::TokenType::BOOL: return "BOOL";
    case my::TokenType::VOID: return "VOID";
    case my::TokenType::STRING: return "STRING";
    case my::TokenType::ARRAY: return "ARRAY";
    case my::TokenType::NUMBER: return "NUMBER";
    case my::TokenType::IDENTIFIER: return "IDENTIFIER";
    case my::TokenType::IF: return "IF";
    case my::TokenType::ELSE: return "ELSE";
    case my::TokenType::SWITCH: return "SWITCH";
    case my::TokenType::CASE: return "CASE";
    case my::TokenType::DEFAULT: return "DEFAULT";
    case my::TokenType::FOR: return "FOR";
    case my::TokenType::WHILE: return "WHILE";
    case my::TokenType::RETURN: return "RETURN";
    case my::TokenType::BREAK: return "BREAK";
    case my::TokenType::CONTINUE: return "CONTINUE";
    case my::TokenType::COMMENT_LITERAL: return "COMMENT_LITERAL";
    case my::TokenType::INTEGER_LITERAL: return "INTEGER_LITERAL";
    case my::TokenType::FLOAT_LITERAL: return "FLOAT_LITERAL";
    case my::TokenType::STRING_LITERAL: return "STRING_LITERAL";
    case my::TokenType::BOOL_LITERAL: return "BOOL_LITERAL";
    case my::TokenType::CHAR_LITERAL: return "CHAR_LITERAL";
    case my::TokenType::IN: return "IN";
    case my::TokenType::OUT: return "OUT";
    case my::TokenType::ASSIGN: return "ASSIGN";
    case my::TokenType::PLUS: return "PLUS";
    case my::TokenType::MINUS: return "MINUS";
    case my::TokenType::MUL: return "MUL";
    case my::TokenType::DIV: return "DIV";
    case my::TokenType::LT: return "LT";
    case my::TokenType::GT: return "GT";
    case my::TokenType::EQ: return "EQ";
    case my::TokenType::NEQ: return "NEQ";
    case my::TokenType::AND: return "AND";
    case my::TokenType::OR: return "OR";
    case my::TokenType::NOT: return "NOT";
    case my::TokenType::QUOTEMARK: return "QUOTEMARK";
    case my::TokenType::NEXT_STATEMENT: return "NEXT_STATEMENT";
    case my::TokenType::LPAREN: return "LPAREN";
    case my::TokenType::RPAREN: return "RPAREN";
    case my::TokenType::LBRACE: return "LBRACE";
    case my::TokenType::RBRACE: return "RBRACE";
    case my::TokenType::LBRACKET: return "LBRACKET";
    case my::TokenType::RBRACKET: return "RBRACKET";
    case my::TokenType::COMMA: return "COMMA";
    case my::TokenType::SEMICOLON: return "SEMICOLON";
    case my::TokenType::COLON: return "COLON";
    case my::TokenType::UNKNOWN: return "UNKNOWN";
    case my::TokenType::END: return "END";
    default: return "unknown";
  }
}*/

class Token {
public:
  Token(my::TokenType t, std::string s, int line, int column) :
  type_(t), value_(std::move(s)), position_({line, column}) {}

  Token(my::TokenType t, std::string s) :
  type_(t), value_(std::move(s)), position_({0,0}) {}

  [[nodiscard]] my::TokenType getType() const;
  [[nodiscard]] std::string getValue() const;

  [[nodiscard]] size_t getLine() const;
  [[nodiscard]] size_t getColumn() const;


private:
  my::TokenType type_;
  std::string value_;

  std::pair<size_t, size_t> position_;
};


#endif //TOKENS_H
