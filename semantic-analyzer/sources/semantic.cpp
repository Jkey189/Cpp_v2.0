#include "../headers/semantic.h"


void SemanticAnalyzer::enterScope(const std::string& scopeName) {
  currentScope = scopeName;
}

void SemanticAnalyzer::exitScope() {
  currentScope = "global"; // По умолчанию возвращаемся в глобальную область
}

void SemanticAnalyzer::declareIdentifier(const std::string& name, const IdentifierType type) {
  try {
    tid.addIdentifier(name, type, currentScope);
  } catch (const std::exception& e) {
    throw std::runtime_error("Semantic error: " + std::string(e.what()));
  }
}

void SemanticAnalyzer::useIdentifier(const std::string& name) {
  try {
    tid.markAsUsed(name, currentScope);
  } catch (const std::exception& e) {
    throw std::runtime_error("Semantic error: Variable '" + name + "' used without declaration in scope '" + currentScope + "'.");
  }
}

void SemanticAnalyzer::initializeIdentifier(const std::string& name) {
  try {
    tid.markAsInitialized(name, currentScope);
  } catch (const std::exception& e) {
    throw std::runtime_error("Semantic error: Variable '" + name + "' initialized without declaration in scope '" + currentScope + "'.");
  }
}

void SemanticAnalyzer::checkType(const std::string& expectedType, const std::string& actualType) {
  if (expectedType != actualType) {
    throw std::runtime_error("Type mismatch: Expected '" + expectedType + "', but got '" + actualType + "'.");
  }
}