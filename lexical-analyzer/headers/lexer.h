#ifndef LEXER_H
#define LEXER_H


#include "../../includes/libraries.h"
#include "tokens.h"
#include "trie.h"


class LexicalAnalyzer {
public:
  explicit LexicalAnalyzer(std::string source) : program_(std::move(source)), position_(0) {
    initializeKeywords();
  }

  static std::vector<Token> tokenize();


private:
  std::string program_;
  size_t position_;
  Trie keywords_;

  void initializeKeywords() const {
    std::string ifStr = "if";
    std::string elseStr = "else";
    std::string caseStr = "case";
    std::string switchStr = "switch";
    std::string breakStr = "break";
    std::string continueStr = "continue";
    std::string constStr = "const";
    std::string whileStr = "while";
    std::string forStr = "for";
    std::string returnStr = "return";
    std::string voidStr = "void";
    std::string trueStr = "true";
    std::string falseStr = "false";

    keywords_.insert(ifStr);
    keywords_.insert(elseStr);
    keywords_.insert(caseStr);
    keywords_.insert(switchStr);
    keywords_.insert(breakStr);
    keywords_.insert(continueStr);
    keywords_.insert(constStr);
    keywords_.insert(whileStr);
    keywords_.insert(forStr);
    keywords_.insert(returnStr);
    keywords_.insert(voidStr);
    keywords_.insert(trueStr);
    keywords_.insert(falseStr );
  }
};


#endif //LEXER_H
