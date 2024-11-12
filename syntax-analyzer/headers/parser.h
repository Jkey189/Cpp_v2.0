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
  void parseProgram() {
    while(currToken_.getType() != TokenType::END) {
      parseDeclaration();
    }
  }

  // <декларация> -> <функция> | <переменная>
  void parseDeclaration() {
    if (isType(currToken_)) {
      Token nextToken = lexer_.peek();

      if (nextToken.getType() == TokenType::LPAREN) {
        parseFunction();
      } else {
        parseVariable();
      }
    } else {
      throw std::runtime_error("Expected declaration");
    }
  }

  // <функция> -> <тип> <идентификатор> ( [ <параметры> ] ) { <блок> }
  void parseFunction() {
    parseType();
    expect(TokenType::IDENTIFIER);
    expect(TokenType::LPAREN);

    if (currToken_.getType() != TokenType::RPAREN) {
      parseParameters();
    }

    expect(TokenType::RPAREN);
    parseBlock();
  }

  // <параметры> -> <параметр> ( , <параметр> )*
  void parseParameters() {
    parseParameter();

    while (currToken_.getType() != TokenType::COMMA) {
      advance();
      parseParameter();
    }
  }

  // <параметр> -> <тип> <идентификатор>
  void parseParameter() {
    parseType();
    expect(TokenType::IDENTIFIER);
  }

  // <переменная> -> <тип> <идентификатор> [ '=' <выражение> ] ';'
  void parseVariable() {
    parseType();
    expect(TokenType::IDENTIFIER);

    if (currToken_.getType() == TokenType::ASSIGN) {
      advance();
      parseExpression();
    }

    expect(TokenType::SEMICOLON);
  }

  // <блок> -> '{' <инструкция>* '}'
  void parseBlock() {
    expect(TokenType::LBRACE);

    while (currToken_.getType() != TokenType::RBRACE) {
      parseStatement();
    }

    expect(TokenType::RBRACE);
  }

  // <инструкция> -> <оператор> | <условная_инструкция> | <цикл> | <выбор> | <функция>
  void parseStatement() {
    if (currToken_.getType() == TokenType::IF) {
      parseIfStatement();
    } else if (currToken_.getType() == TokenType::WHILE || currToken_.getType() == TokenType::FOR) {
      parseLoop();
    } else if (currToken_.getType() == TokenType::SWITCH) {
      parseSwitch();
    } else {
      throw std::runtime_error("Unexpected statement");
    }
  }

  // <оператор> -> <идентификатор> '=' <выражение> ';' | return <выражение> ';' | ...
  void parseOperator() {
    if (currToken_.getType() == TokenType::IDENTIFIER) {
      advance();
      expect(TokenType::ASSIGN);
      parseExpression();
      expect(TokenType::SEMICOLON);
    } else if (currToken_.getType() == TokenType::RETURN) {
      advance();
      parseExpression();
      expect(TokenType::SEMICOLON);
    } else {
      throw std::runtime_error("Unexpected statement");
    }
  }

  // <условная_инструкция> -> if '(' <выражение> ')' <блок> [else <блок>]
  void parseIfStatement() {
    expect(TokenType::IF);
    expect(TokenType::LPAREN);
    parseExpression();
    expect(TokenType::RPAREN);
    parseBlock();

    if (currToken_.getType() == TokenType::ELSE) {
      advance();
      parseBlock();
    }
  }

  // <цикл> -> while '(' <выражение> ')' <блок> | for '(' [<инициализация>] ';' ...
  void parseLoop() {
    if (currToken_.getType() == TokenType::WHILE) {
      advance();
      expect(TokenType::LPAREN);
      parseExpression();
      expect(TokenType::RPAREN);
      parseBlock();
    } else if (currToken_.getType() == TokenType::FOR) {
      advance();
      expect(TokenType::LPAREN);

      if (isType(currToken_)) {
        parseVariable();
      }
      expect(TokenType::SEMICOLON);

      if (currToken_.getType() != TokenType::SEMICOLON) {
        parseExpression();
      }
      expect(TokenType::SEMICOLON);

      if (currToken_.getType() != TokenType::RPAREN) {
        parseOperator();
      }
      expect(TokenType::RPAREN);
      parseBlock();
    } else {
      throw std::runtime_error("Unexpected statement");
    }
  }

  // <выбор> -> switch '(' <выражение> ')' '{' ( case <литерал> : <блок> )* [default: <блок>] '}'
  void parseSwitch() {
    expect(TokenType::SWITCH);
    expect(TokenType::LPAREN);
    parseExpression();
    expect(TokenType::RPAREN);
    expect(TokenType::LBRACE);

    while (currToken_.getType() == TokenType::CASE) {
      advance();
      parseLiteral();
      expect(TokenType::COLON);
      parseBlock();
    }

    if (currToken_.getType() == TokenType::DEFAULT) {
      advance();
      expect(TokenType::COLON);
      parseBlock();
    }

    expect(TokenType::RBRACE);
  }

  // <выражение> -> <выражение_лог_или> и другие правила выражений
  void parseExpression() {
    parseLogicalOr();
  }

  void parseLogicalOr() {
    parseLogicalAnd();

    while (currToken_.getType() == TokenType::OR) {
      advance();
      parseLogicalAnd();
    }
  }

  void parseLogicalAnd();

  // ...
  // Распишу позже

  // <тип> -> int | float | double | char | bool | void | string | array<тип>
  void parseType() {
    if (isType(currToken_)) {
      advance();

      if (currToken_.getType() == TokenType::ARRAY) {
        expect(TokenType::LT);
        parseType();
        expect(TokenType::GT);
      }
    } else {
      throw std::runtime_error("Unexpected statement");
    }
  }

  void parseLiteral(); // lexical literals analyzing
};


#endif //PARSER_H
