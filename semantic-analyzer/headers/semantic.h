#ifndef SEMANTIC_H
#define SEMANTIC_H


#include "../../includes/libraries.h"
#include "../../syntax-analyzer/headers/parser.h"


class SemanticAnalyzer {
public:
  explicit SemanticAnalyzer(const Parser&  parser) : parser_(parser) {}

private:
  Parser parser_;
};


#endif //SEMANTIC_H
