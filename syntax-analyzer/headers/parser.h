#ifndef PARSER_H
#define PARSER_H


#include <complex>

#include "../../includes/libraries.h"
#include "../../lexical-analyzer/headers/lexer.h"


class Parser {
public:
  explicit Parser(LexicalAnalyzer& lexer): lexer_(lexer), currToken_(lexer.getLex()) {
    /*currToken_ = lexer_.getLex();*/
    parserAdvance();
  }

  void program() {
    parserAdvance();
    parseProgram();
  }


private:
  size_t currCount = 0;
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
    return token.getType() == my::TokenType::INT || token.getType() == my::TokenType::FLOAT ||
      token.getType() == my::TokenType::CHAR || token.getType() == my::TokenType::BOOL ||
        token.getType() == my::TokenType::VOID ||token.getType() == my::TokenType::STRING ||
          token.getType() == my::TokenType::ARRAY || token.getType() == my::TokenType::IDENTIFIER;
  }


  Token parserAdvance() {
    return lexer_.getTokens()[currCount++];
  }

  

  void expect(my::TokenType type) {
    if (currToken_.getType() != type) {
      throw std::runtime_error("Syntax error: unexpected token | wrong token type");
    }
    parserAdvance();
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
    if (currToken_.getValue() == "cin") {

    } else if (currToken_.getType() == my::TokenType::IF) {
      parseIf();
    } else if (currToken_.getType() == my::TokenType::FOR || currToken_.getType() == my::TokenType::WHILE) {
      parseLoop();
    } else if (currToken_.getType() == my::TokenType::SWITCH) {
      parseSwitch();
    } else if (currToken_.getValue() == "func") {
      parseFunction();
    } else {
      throw std::runtime_error("Syntax error: unexpected token");
    }
  }

  // оператор
  void parseOperator() {
    if (lexer_.peek().getType() == my::TokenType::COMMA) {
      parseOperator();
      expect(my::TokenType::COMMA);
      parseOperator();
    } else if (currToken_.getType() == my::TokenType::IDENTIFIER) {
      if (lexer_.peek().getType() == my::TokenType::ASSIGN) {
        parseIdentifier();
        expect(my::TokenType::ASSIGN);
        parseExpression();
        expect(my::TokenType::SEMICOLON);
      } else {
        parseVariable();
        expect(my::TokenType::SEMICOLON);
      }

      /*parseIdentifier();
      parserAdvance();

      if (currToken_.getType() == my::TokenType::ASSIGN) {
        parserAdvance();
        parseExpression();
        expect(my::TokenType::SEMICOLON);
      } else if (currToken_.getType() == my::TokenType::SEMICOLON) {
        parserAdvance();
      } else {
        throw std::runtime_error("Syntax error: unexpected token");
      }*/
    } else if (currToken_.getType() == my::TokenType::RETURN) {
      parserAdvance();
      parseExpression();
      expect(my::TokenType::SEMICOLON);
    } else if (currToken_.getType() == my::TokenType::BREAK || currToken_.getType() == my::TokenType::CONTINUE) {
      parserAdvance();
      expect(my::TokenType::SEMICOLON);
    } else if (currToken_.getType() == my::TokenType::IF ||
      currToken_.getType() == my::TokenType::FOR || currToken_.getType() == my::TokenType::WHILE ||
      currToken_.getType() == my::TokenType::SWITCH ||
      currToken_.getValue() == "func") { // add operator
      parseStatement();
      expect(my::TokenType::SEMICOLON);
    } else if (currToken_.getType() == my::TokenType::SEMICOLON) {
      parserAdvance();
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
    if (currToken_.getType() == my::TokenType::IDENTIFIER) {
      parserAdvance();
      if (currToken_.getType() == my::TokenType::ASSIGN) {
        parserAdvance();
        if (currToken_.getType() == my::TokenType::IDENTIFIER) {
          parserAdvance();
        } else if (isNumber(currToken_.getValue())) {
          parserAdvance();
        } else {
          throw std::runtime_error("Syntax error: unexpected token | `parseInitialization()' error");
        }
        expect(my::TokenType::SEMICOLON);
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
