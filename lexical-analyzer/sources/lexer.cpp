#include "../headers/lexer.h"


std::vector<Token> LexicalAnalyzer::tokenize() {
  std::cout << "Start tokenization now!!!" << std::endl << std::endl;
  std::vector<Token> tokens;
  std::string op;

  while (position_ < program_.size()) {
    const char currChar = program_[position_];

    if (isSpace(currChar) || isEnter(currChar)) {
      ++position_;
      continue;
    }

    if (isDigit(currChar)) {
      std::string number = getNumber();

      if (number.find('.') != std::string::npos) {
        tokens.emplace_back(TokenType::FLOAT_NUMBER, number);
      } else {
        tokens.emplace_back(TokenType::INTEGER_NUMBER, number);
      }
    } else if (currChar == '+' ||
      currChar == '-' ||
      currChar == '*' ||
      currChar == '/') {
      tokens.emplace_back(TokenType::OPERATOR, std::string(1, currChar));
      ++position_;
    } else if (currChar == '(' ||
      currChar == ')' ||
      currChar == '{' ||
      currChar == '}' ||
      currChar == ';') {
      tokens.emplace_back(TokenType::PUNCTUATOR, std::string(1, currChar));
      ++position_;
    } else {
      tokens.emplace_back(TokenType::UNKNOWN, std::string(1, currChar));
      ++position_;
    }

    /*if (currChar == '(' ||
      currChar == ')' ||
      currChar == '{' ||
      currChar == '}' ||
      currChar == ';') {
      std::cout << "Found a semicolon (';')" << std::endl;
      tokens.emplace_back(TokenType::PUNCTUATOR, std::string(1, currChar));
      ++position_;
    } else if (std::isspace(currChar)) {
      /*tokens.emplace_back(TokenType::WHITESPACE, std::string(1, currChar));#1#
      ++position_;
      std::cout << "That was just a whitespace..." << std::endl;
    } else if (std::isalpha(currChar) || currChar == '_') {
      tokens.emplace_back(tokenizeIdentifierOrKeyword());
      /*std::cout << "Yee. We've found an identifier!!!" << std::endl;#1# // it's in `tokenizeIdentifierOrKeyword`
    } else if (std::isdigit(currChar)) {
      tokens.emplace_back(tokenizeNumber());
      std::cout << "Yee. We've found a number!!!" << std::endl;
    } else if (isOperator(op)) {
      tokens.emplace_back(tokenizeOperator(op));
      ++position_;
      std::cout << "Yee. We've found an operator!!!" << std::endl;
    }*/
  }

  return tokens;
}

