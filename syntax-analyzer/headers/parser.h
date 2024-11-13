#ifndef PARSER_H
#define PARSER_H


#include <complex>

#include "../../includes/libraries.h"
#include "../../lexical-analyzer/headers/lexer.h"


class Parser {
public:
  void parse();


private:
  LexicalAnalyzer lexer_;
  Token currToken_;


  bool isType(const Token& token) {
    return token.getType() == TokenType::INT || token.getType() == TokenType::FLOAT ||
      token.getType() == TokenType::CHAR || token.getType() == TokenType::BOOL ||
        token.getType() == TokenType::VOID ||token.getType() == TokenType::STRING ||
          token.getType() == TokenType::ARRAY || token.getType() == TokenType::IDENTIFIER;
  }


  void advance() {
    currToken_ = lexer_.getLex();
  }

  void expect(TokenType type) {
    if (currToken_.getType() != type) {
      throw std::runtime_error("Syntax error: unexpected token");
    }
    advance();
  }


  // <программа> -> <декларация>*
  void parseProgram();

  // <декларация> -> <функция> | <переменная>
  void parseDeclaration();

  // <функция> -> <тип> <идентификатор> ( [ <параметры> ] ) { <блок> }
  void parseFunction();

  // <параметры> -> <параметр> ( , <параметр> )*
  void parseParameters();

  // <параметр> -> <тип> <идентификатор>
  void parseParameter();

  // <переменная> -> <тип> <идентификатор> [ '=' <выражение> ] ';'
  void parseVariable();

  // <блок> -> '{' <инструкция>* '}'
  void parseBlock();

  // <инструкция> -> <оператор> | <условная_инструкция> | <цикл> | <выбор> | <функция>
  void parseStatement();

  // <оператор> -> <идентификатор> '=' <выражение> ';' | return <выражение> ';' | ...
  void parseOperator();

  // <условная_инструкция> -> if '(' <выражение> ')' <блок> [else <блок>]
  void parseIfStatement();

  // <цикл> -> while '(' <выражение> ')' <блок> | for '(' [<инициализация>] ';' ...
  void parseLoop();

  // <выбор> -> switch '(' <выражение> ')' '{' ( case <литерал> : <блок> )* [default: <блок>] '}'
  void parseSwitch();

  // <выражение> -> <выражение_лог_или> и другие правила выражений
  void parseExpression();

  void parseLogicalOr();

  void parseLogicalAnd();

  void parseEqNeq();

  void parseLowGreat();

  void parsePlusMinus();

  void parseMulDiv();

  // <тип> -> int | float | double | char | bool | void | string | array<тип>
  void parseType();

  void parseLiteral(); // lexical literals analyzing
};


#endif //PARSER_H
