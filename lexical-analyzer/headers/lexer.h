#ifndef LEXER_H
#define LEXER_H


#include "../../includes/libraries.h"
#include "../../global_functions/global_funcs.h"
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
  Token peek(size_t ind);

  std::vector<Token> getTokens() {
    position_ = 0;
    static std::vector<Token> tokens = tokenize();
    return tokens;
  }

private:
  size_t ind = 0;
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

  // Распознаем символьный литерал
  std::string getCharLiteral();

  // Распознаем комментарий
  std::string getComment();

  // Проверки для токенизации и сама токенизация
  void initializeKeywords(const std::string& keywordsPath) const;

  bool isOperator(std::string& op) const;

  [[nodiscard]] Token tokenizeIdentifierOrKeyword();

  static bool isKeyword(const std::string& id);

  [[nodiscard]] static my::TokenType keywordToTokenType(const std::string& keyword);

  /*Token tokenizeNumber();*/

  Token tokenizeOperator(std::string& op);

  static bool isBracket(char c);

  static Token tokenizeBracket(char c);

  Token parseIdentifier();
};


#endif //LEXER_H
