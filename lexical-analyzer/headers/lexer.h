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

  std::vector<Token> getTokens() { return tokenize(); }


private:
  std::string program_;
  size_t position_;
  Trie keywords_;

  // Функции для проверки символа - буква, цифра, или вообще whitespace...
  static bool isSpace(char c);
  static bool isEnter(char c);
  static bool isAlpha(char c);
  static bool isDigit(char c);
  static bool isIdentifierChar(char c);
  [[nodiscard]] bool isComment(char);

  // Конвертим к слову
  std::string getWord();

  // А здесь получаем число
  std::string getNumber();

  // Распознаем строковый летрал
  std::string getString();

  // Распознаем комментарий
  std::string getComment();

  // Проверки для токенизации и сама токенизация
  void initializeKeywords(const std::string& keywordsPath) const;

  bool isOperator(std::string& op) const;

  [[nodiscard]] Token tokenizeIdentifierOrKeyword();

  static bool isKeyword(const std::string& id);

  [[nodiscard]] static TokenType keywordToTokenType(const std::string& keyword);

  /*Token tokenizeNumber();*/

  Token tokenizeOperator(std::string& op);

  static bool isBracket(char c);

  static Token tokenizeBracket(char c);


  Token parseNumber() {
    std::string number;

    while (position_ < program_.size() && isDigit(program_[position_])) {
      number.push_back(program_[position_++]);
    }

    return {TokenType::NUMBER, number};
  }

  Token parseIdentifier();
};


#endif //LEXER_H
