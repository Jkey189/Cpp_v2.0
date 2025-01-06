#ifndef SEMANTIC_H
#define SEMANTIC_H


#include <utility>

#include "tid.h"
#include "ast-node.h"


class SemanticAnalyzer {
public:
  explicit SemanticAnalyzer(TID  tid, const std::stack<std::string>& scopes) :
  tid_(std::move(tid)), scopeStack_(scopes) {}

  void analyze(const ASTNode& root); // Entry point for analysis

private:
  TID tid_;                         // Table of identifiers
  std::stack<std::string> scopeStack_; // Stack for tracking scopes

  void enterScope(const std::string& scopeName);
  void exitScope();
  std::string currentScope() const;

  void analyzeNode(const ASTNode& node);

  void analyzeProgram(const ASTNode& node);

  void analyzeFunction(const ASTNode& node);

  void analyzeBlock(const ASTNode& node);

  void analyzeVariable(const ASTNode& node);

  void analyzeAssignment(const ASTNode& node);

  void analyzeExpression(const ASTNode& node);

  void analyzeLiteral(const ASTNode& node);

  void analyzeIdentifier(const ASTNode& node);

  void analyzeIf(const ASTNode& node);

  void analyzeLoop(const ASTNode& node);

  void analyzeReturn(const ASTNode& node);

  void analyzeInput(const ASTNode& node);

  void analyzeOutput(const ASTNode& node);

  void analyzeSwitch(const ASTNode& node);

  void analyzeBreak(const ASTNode& node);

  void analyzeContinue(const ASTNode& node);
};


#endif //SEMANTIC_H
