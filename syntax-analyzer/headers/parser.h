#ifndef PARSER_H
#define PARSER_H


#include <complex>

#include "../../includes/libraries.h"
#include "../../lexical-analyzer/headers/lexer.h"


class Parser {
public:
  explicit Parser(LexicalAnalyzer& lexer): lexer_(lexer), currToken_(lexer.getLex()) {
    /*currToken_ = lexer_.getLex();*/
    advance();
  }

  void parse() {
    program();
  }


private:
  LexicalAnalyzer lexer_;
  Token currToken_;


  static bool isNumber(const std::string& num) {
    for (const auto ch : num) {
      if (ch != '.' && (ch < '0' || ch > '9')) {
        return false;
      }
    }
    return true;
  }


  static bool isType(const Token& token) {
    return token.getType() == TokenType::INT || token.getType() == TokenType::FLOAT ||
      token.getType() == TokenType::CHAR || token.getType() == TokenType::BOOL ||
        token.getType() == TokenType::VOID ||token.getType() == TokenType::STRING ||
          token.getType() == TokenType::ARRAY || token.getType() == TokenType::IDENTIFIER;
  }


  void advance() {
    /*const Token prevToken = currToken_;*/
    currToken_ = lexer_.getLex();

    /*if (currToken_.getType() == TokenType::END && prevToken.getType() != TokenType::END) {
      throw std::runtime_error("Expression expected");
    }*/
  }

  void expect(TokenType type) {
    if (currToken_.getType() != type) {
      throw std::runtime_error("Syntax error: unexpected token");
    }
    advance();
  }


  // запускаем parser
  void program() {
    advance();
    parseProgram();
  }

  // программа
  void parseProgram() {
    while (currToken_.getType() != TokenType::END) {
      parseDeclaration();
    }
  }

  // декларация
  void parseDeclaration() {
    if (currToken_.getValue() == "func") {
      Token nextToken = lexer_.peek();
      if (isType(nextToken)) {
        parseFunction();
      } else {
        throw std::runtime_error("Syntax error: unexpected token");
      }
    } else if (isType(currToken_)) {
      parseDeclaration();
    } else {
      throw std::runtime_error("Syntax error: unexpected token");
    }
  }

  // функция
  void parseFunction() {
    parseType();
    parseIdentifier();
    expect(TokenType::LPAREN);
    parseParams();
    expect(TokenType::RPAREN);
    parseBlock();
  }

  // параметры
  void parseParams() {
    parseParam();
    while (currToken_.getValue() == ",") {
      advance();
      parseParam();
    }
  }

  // параметр
  void parseParam() {
    parseType();
    expect(TokenType::IDENTIFIER);
  }

  // переменная
  void parseVariable() {
    expect(TokenType::IDENTIFIER);
  }

  // блок
  void parseBlock() {
    expect(TokenType::LBRACE);
    while (currToken_.getType() != TokenType::RBRACE) {
      parseStatement();
    }
    expect(TokenType::RBRACE);
  }

  // состояние
  void parseStatement() {
    // add operator
    if (currToken_.getType() == TokenType::IF) {
      parseIf();
    } else if (currToken_.getType() == TokenType::FOR || currToken_.getType() == TokenType::WHILE) {
      parseLoop();
    } else if (currToken_.getType() == TokenType::SWITCH) {
      parseSwitch();
    } else if (currToken_.getValue() == "func") {
      parseFunction();
    } else {
      throw std::runtime_error("Syntax error: unexpected token");
    }
  }

  // оператор
  void parseOperator() {
    if (currToken_.getType() == TokenType::IDENTIFIER) {
      parseIdentifier();
      advance();

      if (currToken_.getType() == TokenType::ASSIGN) {
        advance();
        parseExpression();
        expect(TokenType::SEMICOLON);
      } else if (currToken_.getType() == TokenType::SEMICOLON) {
        advance();
      } else {
        throw std::runtime_error("Syntax error: unexpected token");
      }
    } else if (currToken_.getType() == TokenType::RETURN) {
      advance();
      parseExpression();
      expect(TokenType::SEMICOLON);
    } else if (currToken_.getType() == TokenType::BREAK) {
      advance();
      expect(TokenType::SEMICOLON);
    } else if (currToken_.getType() == TokenType::CONTINUE) {
      advance();
      expect(TokenType::CONTINUE);
    } else if (currToken_.getType() == TokenType::IF ||
      currToken_.getType() == TokenType::FOR || currToken_.getType() == TokenType::WHILE ||
      currToken_.getType() == TokenType::SWITCH ||
      currToken_.getValue() == "func") {
      parseStatement();
      expect(TokenType::SEMICOLON);
      } else if (currToken_.getType() == TokenType::SEMICOLON) {
        advance();
      } else {
        throw std::runtime_error("Syntax error: unexpected token");
      }
  }

  // условный оператор if
  void parseIf() {
    expect(TokenType::IF);
    expect(TokenType::LPAREN);
    parseExpression();
    expect(TokenType::RPAREN);
    parseBlock();

    if (currToken_.getType() == TokenType::ELSE) {
      if (lexer_.peek().getType() == TokenType::IF) {
        parseIf();
      } else if (lexer_.peek().getType() == TokenType::LBRACE) {
        advance();
        parseBlock();
      } else {
        throw std::runtime_error("Syntax error: unexpected token");
      }
    }
  }

  // цикл
  void parseLoop() {
    if (currToken_.getType() == TokenType::FOR) {
      advance();
      expect(TokenType::LBRACE);
      parseInitialization();
      expect(TokenType::SEMICOLON);
      parseExpression();
      expect(TokenType::SEMICOLON);
      parseStep();
      expect(TokenType::RBRACE);
    } else if (currToken_.getType() == TokenType::WHILE) {
      advance();
      expect(TokenType::LBRACE);
      parseExpression();
      expect(TokenType::RBRACE);
      parseBlock();
    } else {
      throw std::runtime_error("Syntax error: unexpected token");
    }
  }

  // инициализация
  void parseInitialization() {
    if (isType(currToken_)) {
      parseType();
    }
    if (currToken_.getType() == TokenType::IDENTIFIER) {
      advance();
      if (currToken_.getType() == TokenType::ASSIGN) {
        advance();
        if (currToken_.getType() == TokenType::IDENTIFIER) {
          advance();
        } else if (isNumber(currToken_.getValue())) {
          advance();
        } else {
          throw std::runtime_error("Syntax error: unexpected token");
        }
        expect(TokenType::SEMICOLON);
      } else {
        throw std::runtime_error("Syntax error: unexpected token");
      }
    } else {
      throw std::runtime_error("Syntax error: unexpected token");
    }
  }

  // шаг
  void parseStep() {
    parseOperator();
  }

  // выбор switch
  void parseSwitch() {
    expect(TokenType::SWITCH);
    expect(TokenType::LPAREN);
    parseExpression();
    expect(TokenType::RPAREN);
    expect(TokenType::LBRACE);
    while (currToken_.getType() == TokenType::CASE) {
      parseLiteral();
      expect(TokenType::COLON);
      parseBlock();
    }
    if (currToken_.getType() == TokenType::DEFAULT) {
      expect(TokenType::COLON);
      parseBlock();
    }
    expect(TokenType::RBRACE);
  }

  // литерал
  void parseLiteral();

  // выражение
  void parseExpression() {
    if (isNumber(currToken_.getValue())) {
      parseLiteral();
    }
    parseLogicalOr();
  }

  // логическое ИЛИ
  void parseLogicalOr() {
    parseLogicalAnd();
    if (currToken_.getType() == TokenType::AND) {
      advance();
      parseLogicalAnd();
    }
  }

  // логическое И
  void parseLogicalAnd();

  void parseIdentifier();

  void parseType() {
    advance();

    if (isType(currToken_)) {
      advance();
      if (currToken_.getValue() == "array") {
        expect(TokenType::LT);
        // advance();
        parseType();
        expect(TokenType::GT);
      }
    } else {
      throw std::runtime_error("Syntax error: unexpected token");
    }
  }
};


#endif //PARSER_H
