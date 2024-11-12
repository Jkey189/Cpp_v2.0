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

  static std::vector<Token> tokenize();


private:
  std::string program_;
  size_t position_;
  Trie keywords_;

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
};


#endif //LEXER_H
