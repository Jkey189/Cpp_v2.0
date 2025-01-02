#ifndef PARSER_H
#define PARSER_H


#include <complex>

#include "../../global_functions/global_funcs.h"
#include "../../includes/libraries.h"
#include "../../lexical-analyzer/headers/lexer.h"


class Parser {
public:
  explicit Parser(LexicalAnalyzer& lexer) : lexer_(lexer), currToken_(lexer.getLex()) {
    std::cout << "Number of tokens in Lexer: " << lexer_.getTokens().size() << std::endl;

    /*const auto& tokens = lexer_.getTokens();
    std::cout << "Tokens in Parser constructor:" << std::endl;
    for (const auto& token : tokens) {
      std::cout << "Token: " << token.getValue()
                << " (Type: " << getTokenValue(token.getType())
                << ", Line: " << token.getLine()
                << ", Column: " << token.getColumn() << ")" << std::endl;
    }*/

    // parserAdvance();
  }

  /*explicit Parser(const LexicalAnalyzer& lexer) : currCount(0), lexer_(lexer), currToken_(lexer_.getLex()) {
    parserAdvance();

    std::cout << "Parser initialized with first token: '" << currToken_.getValue()
          << "' (Type: " << getTokenValue(currToken_.getType())
          << ", Line: " << currToken_.getLine()
          << ", Column: " << currToken_.getColumn() << ")" << std::endl;


    const auto tokens = lexer_.getTokens();

    std::cout << "Tokens received from lexer:" << std::endl;
    for (const auto &token: tokens) {
      std::cout << "Token: " << token.getValue()
          << " (Type: " << getTokenValue(token.getType())
          << ", Line: " << token.getLine()
          << ", Column: " << token.getColumn() << ")" << std::endl;
    }

    if (tokens.empty()) {
      throw std::runtime_error("Lexer returned an empty token list.");
    }

    currToken_ = tokens.at(currCount++)#1#;
  }*/

  void program() {
    parseProgram();
  }

  [[nodiscard]] LexicalAnalyzer getLexer() const { return lexer_; }


  void expect(const my::TokenType type) {
    if (currToken_.getType() != type) {
      throw std::runtime_error(
      "Syntax error at token: '" + currToken_.getValue() +
      "' (" + getTokenValue(currToken_.getType()) + "), Expected: " + getTokenValue(type)
      );
    }
    advance();
  }

  /*void expect(const my::TokenType type) {
    if (currToken_.getType() != type) { // make expected throw more informative
      throw std::runtime_error("Syntax error at token: '" + currToken_.getValue() +
        "', Expected: " + getTokenValue(type));
    }
    parserAdvance();
  }*/

  void advance() {
    const auto& tokens = lexer_.getTokens(); // Получаем токены из лексера
    if (tokens.empty()) {
      throw std::runtime_error("Parser error: no tokens received from lexer.");
    }

    if (currCount >= tokens.size()) {
      throw std::runtime_error("Parser error: unexpected end of input.");
    }

    currToken_ = tokens[currCount++];
    std::cout << "Advanced to token: '" << currToken_.getValue() << "'" << std::endl;
  }

  /*Token parserAdvance() {
    if (currToken_.getType() == my::TokenType::END) {
      return currToken_;
    }

    // Отладочный вывод текущего токена
    std::cout << "Current token: " << currToken_.getValue()
              << " (Type: " << getTokenValue(currToken_.getType())
              << ", Line: " << currToken_.getLine()
              << ", Column: " << currToken_.getColumn() << ")" << std::endl;

    if (currCount < lexer_.getTokens().size() - 1) { // checking boards
      currToken_ = lexer_.getTokens()[currCount++];
      return currToken_;
    } else if (currCount == lexer_.getTokens().size() - 1) { // last token
      currToken_ = lexer_.getTokens()[currCount];
      ++currCount; // update index
      return currToken_;
    }
    throw std::runtime_error("Syntax error: unexpected end of input.");
  }*/

  /*Token parserAdvance() {
    if (currCount < lexer_.getTokens().size()) {
      return lexer_.getTokens()[currCount++];
    }
    throw std::runtime_error("Syntax error: unexpected end of input.");
  }*/

  static bool isType(const Token& token) {
    return token.getType() == my::TokenType::INT || token.getType() == my::TokenType::FLOAT ||
      token.getType() == my::TokenType::CHAR || token.getType() == my::TokenType::BOOL ||
        token.getType() == my::TokenType::VOID ||token.getType() == my::TokenType::STRING ||
          token.getType() == my::TokenType::ARRAY || token.getType() == my::TokenType::IDENTIFIER;
  }

  [[nodiscard]] Token getCurrToken() const { return currToken_; }


private:
  size_t currCount = 0;
  LexicalAnalyzer lexer_;
  Token currToken_;


  static bool isNumber(const Token& num) {
    for (const auto elem : num.getValue()) {
      if ((elem < '0' || elem > '9') && elem != '.') {
        return false;
      }
    }
    return true;
  }

  static bool isWord(const Token& word) {
    for (const char letter : word.getValue()) {
      if ((letter < 'a' || letter > 'z') && (letter < 'A' || letter > 'Z') && letter != '_') {
        return false;
      }
    }
    return true;
  }


  void parseProgram();

  void parseDeclaration();

  void parseFunction();

  void parseParameters();

  void parseParameter();

  void parseBlock();

  void parseInstruction();

  void parseInput();

  void parseOutput();

  void parseConditional();

  void parseLoop();

  void parseInitialization();

  void parseAssignment();

  void parseStep();

  void parseSwitch();

  void parseLiteral();

  void parseCommentLiteral();

  void parseIntegerLiteral();

  void parseFloatLiteral();

  void parseStringLiteral();

  void parseCharLiteral();

  void parseExpression();

  void parseComma();

  void parseLogicalOr();

  void parseLogicalAnd();

  void parseLogicalComparison();

  void parseComparison();

  void parsePlusMinus();

  void parseMulDiv();

  void parseUnary();

  void parseAtom();

  void parseIndex();

  void parseType();

  void parseIdentifier();  // last statement
};


#endif //PARSER_H
