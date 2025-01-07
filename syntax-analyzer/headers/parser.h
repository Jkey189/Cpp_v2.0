#ifndef PARSER_H
#define PARSER_H


#include <complex>

#include "../../global_functions/global_funcs.h"
#include "../../includes/libraries.h"
#include "../../lexical-analyzer/headers/lexer.h"
#include "../../semantic-analyzer/headers/semantic.h"


class Parser {
public:
  explicit Parser(LexicalAnalyzer& lexer) : lexer_(lexer), currToken_(lexer.getLex()) {
    std::cout << "Number of tokens in Lexer: " << lexer_.getTokens().size() << std::endl;
  }

  void program() {
    parseProgram();
  }

  [[nodiscard]] LexicalAnalyzer getLexer() const { return lexer_; }

  void expect(const my::TokenType type, const std::string& functionName) {
    if (currToken_.getType() != type) {
      throw std::runtime_error(
      "Syntax error at token: '" + currToken_.getValue() +
      "' (" + getTokenValue(currToken_.getType()) + "), Expected: " + getTokenValue(type) +
      " || expect() by " + functionName
      );
    }
    advance(); // probably useless
  }

  void advance() {
    const auto& tokens = lexer_.getTokens(); // Получаем токены из лексера
    if (tokens.empty()) {
      throw std::runtime_error("Parser error: no tokens received from lexer. || advance().1");
    }

    if (currCount >= tokens.size()) {
      throw std::runtime_error("Parser error: unexpected end of input. || advance().2");
    }

    currToken_ = tokens[currCount++];
    std::cout << "Advanced to token: '" << currToken_.getValue() << "'" << std::endl;
  }

  static bool isType(const Token& token) {
    return token.getType() == my::TokenType::INT || token.getType() == my::TokenType::FLOAT ||
      token.getType() == my::TokenType::CHAR || token.getType() == my::TokenType::BOOL ||
        token.getType() == my::TokenType::VOID ||token.getType() == my::TokenType::STRING ||
          token.getType() == my::TokenType::ARRAY;
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

  void parseExpression();

  void parseComma();

  void parseLogicalOr();

  void parseLogicalAnd();

  void parseEqualityOperators();

  void parseRelationalOperators();

  void parsePlusMinus();

  void parseMulDiv();

  void parseUnary();

  void parseAtom();

  void parseIndex();

  void parseType();

  void parseIdentifier();
};


#endif //PARSER_H
