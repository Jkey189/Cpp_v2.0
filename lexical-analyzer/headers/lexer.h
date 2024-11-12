#ifndef LEXER_H
#define LEXER_H


#include "../../includes/libraries.h"
#include "tokens.h"
#include "trie.h"


class LexicalAnalyzer {
public:
  explicit LexicalAnalyzer(std::string source, const std::string &keywordsPath) :
  program_(std::move(source)), position_(0) {
    initializeKeywords(keywordsPath);
  }

  std::vector<Token> tokenize();
  Token getLex();
  Token peek();


private:
  std::string program_;
  size_t position_;
  Trie keywords_;

  // Функции для проверки символа - буква, цифра, или вообще whitespace...
  static bool isSpace(const char c) {
    return c == ' ';
  }

  static bool isEnter(const char c) {
    return c == '\n';
  }

  static bool isAlpha(const char c) {
    return c >= 'a' && c <= 'z' || c >= 'A' && c <= 'Z';
  }

  static bool isDigit(const char c) {
    return c >= '0' && c <= '9';
  }

  static bool isAlphaNumeric(const char c) {
    return isAlpha(c) || isDigit(c);
  }

  // Конвертим к слову
  std::string getWord() {
    size_t start = position_;
    while (position_ < program_.length() && isAlphaNumeric(program_[position_])) {
      ++position_;
    }

    return program_.substr(start, position_ - start);
  }

  // А здесь получаем число
  std::string getNumber() {
    size_t start = position_;
    bool hasDecimal = false;

    while (position_ < program_.length() && isDigit(program_[position_]) || program_[position_] == '.') {
      if (program_[position_] == '.') {
        if (hasDecimal) { break; }
        hasDecimal = true;
      }
      ++position_;
    }

    return program_.substr(start, position_ - start);
  }

  // Проверки для токенизации
  void initializeKeywords(const std::string& keywordsPath) const {
    std::ifstream keywordsFile(keywordsPath);

    if (!keywordsFile.is_open()) {
      std::cerr << "Failed to open file " << "\"" << keywordsPath << "\"" << std::endl;

      if (keywordsFile.bad()) {
        std::cerr << "Fatal error: bad-bit is set" << std::endl;
      }

      if (keywordsFile.fail()) {
        std::cerr << "Error details: " << strerror(errno) << std::endl;
      }
    }

    std::string keyword;
    while (std::getline(keywordsFile, keyword)) {
      if (!keyword.empty()) {
        keywords_.insert(keyword);
      }
    }

    keywordsFile.close();
  }

  bool isOperator(std::string& op) const {
    op.clear();

    const char ch = program_[position_];
    op = std::string(1, ch);

    if (ch == '+' || ch == '-' || ch == '*' || ch == '/') {
      return false;
    }

    if ((ch == '=' || ch == '<' || ch == '>') && position_ + 1 < program_.size()) {
      const char nextCh = program_[position_];

      if ((ch == '=' && nextCh == '=') ||
        (ch == '<' && nextCh == '=') ||
        (ch == '>' && nextCh == '=')) {
        op += nextCh;
        return true;
      }
    }

    if (ch == '<' || ch == '>') {
      return true;
    }

    return false;
  }

  Token tokenizeIdentifierOrKeyword() {
    const size_t start = position_;

    while (position_ < program_.length() && (std::isalnum(program_[position_]) ||
      program_[position_] == '_')) {
      ++position_;
    }

    std::string word = program_.substr(start, position_ - start);
    TokenType wordType = keywords_.find(word) ? TokenType::KEYWORD : TokenType::IDENTIFIER;

    std::cout << "Yee. We've found an identifier!!! " << "It's a/an " << word << std::endl;

    return {wordType, word};
  }

  Token tokenizeNumber() {
    const size_t start = position_;

    while (position_ < program_.length() && std::isdigit(program_[position_])) {
      ++position_;
    }

    return {TokenType::INTEGER_NUMBER, program_.substr(start, position_ - start)};
  }

  Token tokenizeOperator(std::string& op) {
    position_ += op.size();

    return {TokenType::OPERATOR, op};
  }


  Token parseNumber() {
    std::string number;

    while (position_ < program_.size() && isDigit(program_[position_])) {
      number.push_back(program_[position_++]);
    }

    return {TokenType::NUMBER, number};
  }

  Token parseIdentifier() {
    std::string id;

    while (position_ < program_.size() && (isAlpha(program_[position_]) || program_[position_] == '_')) {
      id.push_back(program_[position_++]);
    }

    switch (id) {
      case "int":
        return {TokenType::INT, id};
      case "float":
        return {TokenType::FLOAT, id};
      case "char":
        return {TokenType::CHAR, id};
      case "bool":
        return {TokenType::BOOL, id};
      case "void":
        return {TokenType::VOID, id};
      case "string":
        return {TokenType::STRING, id};
      case "array":
        return {TokenType::ARRAY, id};
      case "if":
        return {TokenType::IF, id};
      case "else":
        return {TokenType::ELSE, id};
      case "switch":
        return {TokenType::SWITCH, id};
      case "case":
        return {TokenType::CASE, id};
      case "default":
        return {TokenType::DEFAULT, id};
      case "for":
        return {TokenType::FOR, id};
      case "while":
        return {TokenType::WHILE, id};
      case "return":
        return {TokenType::RETURN, id};
      case "break":
        return {TokenType::BREAK, id};
      case "continue":
        return {TokenType::CONTINUE, id};
      default:
        return {TokenType::IDENTIFIER, id};
    }
  }
};


#endif //LEXER_H
