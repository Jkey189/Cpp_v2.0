#ifndef SEMANTIC_H
#define SEMANTIC_H


#include <utility>


#include "../../lexical-analyzer/headers/tokens.h"
#include "tid.h"


class SemanticAnalyzer {
public:
  explicit SemanticAnalyzer() : currentScope("global") {}

  // Управление областями видимости
  void enterScope(const std::string& scopeName);
  void exitScope();

  // Работа с идентификаторами
  void declareIdentifier(const std::string& name, IdentifierType type);
  void useIdentifier(const std::string& name);
  void initializeIdentifier(const std::string& name);

  // Проверки
  void checkType(const std::string& expectedType, const std::string& actualType);

private:
  TID tid; // Используемый TID
  std::string currentScope; // Текущая область видимост
};


#endif //SEMANTIC_H
