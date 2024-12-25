#ifndef SEMANTIC_H
#define SEMANTIC_H


#include "../../includes/libraries.h"
#include "../../syntax-analyzer/headers/parser.h"
#include "../TID/tid.h"



class SemanticAnalyzer {
public:
  explicit SemanticAnalyzer(Parser& parser, TID& tid) : parser_(parser), tid_(tid) {}

  void analyze();

private:
  Parser& parser_;
  TID& tid_;
  // на время вместо нормального TID
  std::map<std::string, my::TokenType> symbolTable_;

  void checkDeclaration();
  void checkUsage() const;

  static void checkTypeCompatibility(const Token& left, const Token& right);
  void checkFunctionDeclaration() const;
};



#endif //SEMANTIC_H
