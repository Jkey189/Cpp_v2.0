#include "../headers/semantic.h"


void SemanticAnalyzer::analyze() {
  while (parser_.getCurrToken().getType() != my::TokenType::END) {
    if (parser_.getCurrToken().getType() == my::TokenType::IDENTIFIER) {
      checkUsage();
    } else if (parser_.getCurrToken().getType() == my::TokenType::KEYWORD) {
      checkDeclaration();
    }
    parser_.parserAdvance();
  }
}


void SemanticAnalyzer::checkDeclaration() {
  if (parser_.getCurrToken().getValue() == "func") {
    checkFunctionDeclaration();
  } else if (parser_.isType(parser_.getCurrToken())) {
    const Token identifierToken = parser_.parserAdvance();

    if (identifierToken.getType() != my::TokenType::IDENTIFIER) {
      throw std::runtime_error("Semantic error: Expected an identifier after type declaration");
    }

    const std::string identifier = identifierToken.getValue();
    const my::TokenType type = parser_.getCurrToken().getType();

    if (tid_.exists(identifier)) {
      throw std::runtime_error("Semantic error: Identifier '" + identifier + "' is already declared.");
    }

    tid_.insertElement(identifier, type);
  }
}

void SemanticAnalyzer::checkUsage() const {
  if (const std::string& identifier = parser_.getCurrToken().getValue(); !tid_.exists(identifier)) {
    throw std::runtime_error("Semantic error: Identifier '" + identifier + "' is not declared.");
  }
}

void SemanticAnalyzer::checkFunctionDeclaration() const {
  parser_.parserAdvance();
  const std::string functionName = parser_.parserAdvance().getValue();

  if (tid_.exists(functionName)) {
    throw std::runtime_error("Semantic error: Function '" + functionName + "' is already declared.");
  }

  tid_.insertElement(functionName, my::TokenType::IDENTIFIER);
}

void SemanticAnalyzer::checkTypeCompatibility(const Token& left, const Token& right) {
  if (left.getType() != right.getType()) {
    throw std::runtime_error("Semantic error: Type mismatch between '"
      + left.getValue() + "' and '" + right.getValue() + "'.");
  }
}
