#ifndef SEMANTIC_H
#define SEMANTIC_H


#include "../../includes/libraries.h"
#include "../../syntax-analyzer/headers/parser.h"


class TID {
public:
  explicit TID() : symbolTable_(std::map<std::string, my::TokenType>()) {}
  explicit TID(const std::map<std::string, my::TokenType>& symbolTable)
    : symbolTable_(symbolTable) {}

  bool exists(const std::string& name) const {
    return symbolTable_.contains(name);
  }

  void add(const std::string& name, const my::TokenType type) {
    if (exists(name)) {
      throw std::runtime_error("Semantic error: variable '" + name + "' is already declared.");
    }
    symbolTable_[name] = type;
  }

  my::TokenType getType(const std::string& name) const {
    const auto it = symbolTable_.find(name);
    if (it == symbolTable_.end()) {
      throw std::runtime_error("Semantic error: variable '" + name + "' is not declared.");
    }
    return it->second;
  }

private:
  std::map<std::string, my::TokenType> symbolTable_;
};


class SemanticAnalyzer {
public:
  explicit SemanticAnalyzer(Parser& parser, TID& tid)
    : parser_(parser), tid_(tid) {}

  void analyze();

  std::vector<std::string> generateRPN();


private:
  Parser& parser_;
  TID& tid_;

  void checkFunction(const Token& token) const;
  void checkVariable(const Token& token);
  void checkAssignment(const Token& token);
  void checkExpression(const Token& token);

  static int getPrecedence(const std::string &op);
};



#endif //SEMANTIC_H
