#ifndef GLOBAL_FUNCS_H
#define GLOBAL_FUNCS_H


#include "../includes/libraries.h"
#include "../lexical-analyzer/headers/tokens.h"


inline std::string getTokenValue(const my::TokenType token) {
  switch (token) {
    case my::TokenType::KEYWORD:
      return "KEYWORD";
    case my::TokenType::INT:
      return "INT";
    case my::TokenType::FLOAT:
      return "FLOAT";
    case my::TokenType::BOOL:
      return "BOOL";
    case my::TokenType::VOID:
      return "VOID";
    case my::TokenType::STRING:
      return "STRING";
    case my::TokenType::CHAR:
      return "CHAR";
    case my::TokenType::ARRAY:
      return "ARRAY";
    case my::TokenType::IDENTIFIER:
      return "IDENTIFIER";
    case my::TokenType::IF:
      return "IF";
    case my::TokenType::ELSE:
      return "ELSE";
    case my::TokenType::SWITCH:
      return "SWITCH";
    case my::TokenType::CASE:
      return "CASE";
    case my::TokenType::DEFAULT:
      return "DEFAULT";
    case my::TokenType::FOR:
      return "FOR";
    case my::TokenType::WHILE:
      return "WHILE";
    case my::TokenType::RETURN:
      return "RETURN";
    case my::TokenType::BREAK:
      return "BREAK";
    case my::TokenType::CONTINUE:
      return "CONTINUE";
    case my::TokenType::COMMENT_LITERAL:
      return "COMMENT_LITERAL";
    case my::TokenType::INTEGER_LITERAL:
      return "INTEGER_LITERAL";
    case my::TokenType::FLOAT_LITERAL:
      return "FLOAT_LITERAL";
    case my::TokenType::STRING_LITERAL:
      return "STRING_LITERAL";
    case my::TokenType::CHAR_LITERAL:
      return "CHAR_LITERAL";
    case my::TokenType::ASSIGN:
      return "ASSIGN";
    case my::TokenType::PLUS:
      return "PLUS";
    case my::TokenType::MINUS:
      return "MINUS";
    case my::TokenType::MUL:
      return "MUL";
    case my::TokenType::DIV:
      return "DIV";
    case my::TokenType::LT:
      return "LT";
    case my::TokenType::GT:
      return "GT";
    case my::TokenType::EQ:
      return "EQ";
    case my::TokenType::NEQ:
      return "NEQ";
    case my::TokenType::AND:
      return "AND";
    case my::TokenType::OR:
      return "OR";
    case my::TokenType::NOT:
      return "NOT";
    case my::TokenType::LPAREN:
      return "LPAREN";
    case my::TokenType::RPAREN:
      return "RPAREN";
    case my::TokenType::LBRACE:
      return "LBRACE";
    case my::TokenType::RBRACE:
      return "RBRACE";
    case my::TokenType::LBRACKET:
      return "LBRACKET";
    case my::TokenType::RBRACKET:
      return "RBRACKET";
    case my::TokenType::COMMA:
      return "COMMA";
    case my::TokenType::SEMICOLON:
      return "SEMICOLON";
    case my::TokenType::COLON:
      return "COLON";
    case my::TokenType::END:
      return "END";
    default: // my::TokenType::UNKNOWN
      return "UNKNOWN";
  }
}


#endif //GLOBAL_FUNCS_H
