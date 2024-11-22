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

  void program() {
    advance();
    parseProgram();
  }


private:
  LexicalAnalyzer lexer_;
  Token currToken_;


  static bool isNumber(const std::string& num) {
    for (const auto ch : num) {
      if ((ch < '0' || ch > '9') && ch != '.') {
        return false;
      }
    }
    return true;
  }

  static bool isWord(const std::string& word) {
    for (const auto ch : word) {
      if ((ch < 'a' || ch < 'z') && (ch < 'A' || ch < 'Z') && ch != '_') {
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
      throw std::runtime_error("Syntax error: unexpected token | wrong token type");
    }
    advance();
  }


  // программа
  void parseProgram();

  // декларация
  void parseDeclaration();

  // функция
  void parseFunction();

  // параметры
  void parseParams();

  // параметр
  void parseParam();

  // переменная
  void parseVariable();

  // блок
  void parseBlock();

  // инструкция
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
    if (lexer_.peek().getType() == TokenType::COMMA) {
      parseOperator();
      expect(TokenType::COMMA);
      parseOperator();
    } else if (currToken_.getType() == TokenType::IDENTIFIER) {
      if (lexer_.peek().getType() == TokenType::ASSIGN) {
        parseIdentifier();
        expect(TokenType::ASSIGN);
        parseExpression();
        expect(TokenType::SEMICOLON);
      } else {
        parseVariable();
        expect(TokenType::SEMICOLON);
      }

      /*parseIdentifier();
      advance();

      if (currToken_.getType() == TokenType::ASSIGN) {
        advance();
        parseExpression();
        expect(TokenType::SEMICOLON);
      } else if (currToken_.getType() == TokenType::SEMICOLON) {
        advance();
      } else {
        throw std::runtime_error("Syntax error: unexpected token");
      }*/
    } else if (currToken_.getType() == TokenType::RETURN) {
      advance();
      parseExpression();
      expect(TokenType::SEMICOLON);
    } else if (currToken_.getType() == TokenType::BREAK || currToken_.getType() == TokenType::CONTINUE) {
      advance();
      expect(TokenType::SEMICOLON);
    } else if (currToken_.getType() == TokenType::IF ||
      currToken_.getType() == TokenType::FOR || currToken_.getType() == TokenType::WHILE ||
      currToken_.getType() == TokenType::SWITCH ||
      currToken_.getValue() == "func") { // add operator
      parseStatement();
      expect(TokenType::SEMICOLON);
    } else if (currToken_.getType() == TokenType::SEMICOLON) {
      advance();
    } else {
      throw std::runtime_error("Syntax error: unexpected token | `parseOperator()' error");
    }
  }

  // условный оператор if
  void parseIf();

  // цикл
  void parseLoop();

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
          throw std::runtime_error("Syntax error: unexpected token | `parseInitialization()' error");
        }
        expect(TokenType::SEMICOLON);
      } else {
        throw std::runtime_error("Syntax error: unexpected token | `parseInitialization()' error");
      }
    } else {
      throw std::runtime_error("Syntax error: unexpected token | `parseInitialization()' error");
    }
  }

  // шаг
  void parseStep();

  // выбор switch
  void parseSwitch();

  // литерал
  void parseLiteral();

  // целочисленный литерал
  void parseIntegerLiteral();

  // вещественный литерал
  void parseFloatLiteral();

  // строковый литерал
  void parseStringLiteral();

  // выражение
  void parseExpression();

  // логическое ИЛИ
  void parseLogicalOr();

  // логическое И
  void parseLogicalAnd();

  // логическое равно / неравно
  void parseEqNotEq();

  // логическое сравнение
  void parseComparison();

  // плюс / минус
  void parsePlusMinus();

  // умножить / делить
  void parseMulDiv();

  // унарное выражение
  void parseUnaryExpression();

  // атомное выражение
  void parseAtomExpression();

  // тип
  void parseType();

  void parseIdentifier() {
    if (isNumber(currToken_.getValue())) {
      throw std::runtime_error("Syntax error: unexpected token | `parseIdentifier()' error ([0-9][a-zA-Z] - bad)");
    }

  }

  // [a-zA-Z]
  void parseWord() {

  }

  // [0-9]
  void parseNumber() {

  }
};


#endif //PARSER_H
