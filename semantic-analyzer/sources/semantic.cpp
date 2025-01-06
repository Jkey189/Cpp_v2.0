#include "../headers/semantic.h"

void SemanticAnalyzer::analyze(const ASTNode& root) {
  try {
    analyzeNode(root);
  } catch (const std::exception& e) {
    throw std::runtime_error("Semantic analysis failed: " + std::string(e.what()));
  }
}

void SemanticAnalyzer::enterScope(const std::string& scopeName) {
  scopeStack_.push(scopeName);
}

void SemanticAnalyzer::exitScope() {
  if (!scopeStack_.empty()) {
    scopeStack_.pop();
  } else {
    throw std::runtime_error("Attempt to exit scope when none exists.");
  }
}

std::string SemanticAnalyzer::currentScope() const {
  return scopeStack_.empty() ? "global" : scopeStack_.top();
}

void SemanticAnalyzer::analyzeNode(const ASTNode& node) {
  switch (node.getType()) {
    case ASTNodeType::PROGRAM:
      analyzeProgram(node);
      break;
    case ASTNodeType::FUNCTION:
      analyzeFunction(node);
      break;
    case ASTNodeType::BLOCK:
      analyzeBlock(node);
      break;
    case ASTNodeType::VARIABLE_DECLARATION:
      analyzeVariable(node);
      break;
    case ASTNodeType::ASSIGNMENT:
      analyzeAssignment(node);
      break;
    case ASTNodeType::EXPRESSION:
      analyzeExpression(node);
      break;
    case ASTNodeType::LITERAL:
      analyzeLiteral(node);
      break;
    case ASTNodeType::IDENTIFIER:
      analyzeIdentifier(node);
      break;
    case ASTNodeType::IF_STATEMENT:
      analyzeIf(node);
      break;
    case ASTNodeType::LOOP_STATEMENT:
      analyzeLoop(node);
      break;
    case ASTNodeType::RETURN_STATEMENT:
      analyzeReturn(node);
      break;
    case ASTNodeType::INPUT:
      analyzeInput(node);
      break;
    case ASTNodeType::OUTPUT:
      analyzeOutput(node);
      break;
    case ASTNodeType::SWITCH:
      analyzeSwitch(node);
      break;
    case ASTNodeType::BREAK:
      analyzeBreak(node);
      break;
    case ASTNodeType::CONTINUE:
      analyzeContinue(node);
      break;
    default:
      throw std::runtime_error("Unknown AST node type encountered.");
  }
}

void SemanticAnalyzer::analyzeProgram(const ASTNode& node) {
  for (const auto& child : node.getChildren()) {
    analyzeNode(*child);
  }
}

void SemanticAnalyzer::analyzeFunction(const ASTNode& node) {
  const std::string& functionName = node.getValue();

  if (tid_.identifierExists(functionName, "global")) {
    throw std::runtime_error("Function '" + functionName + "' is already declared in the global scope.");
  }

  tid_.addIdentifier(functionName, IdentifierType::FUNCTION, "global");
  enterScope(functionName);

  for (const auto& child : node.getChildren()) {
    analyzeNode(*child);
  }

  exitScope();
}

void SemanticAnalyzer::analyzeBlock(const ASTNode& node) {
  enterScope("block_" + std::to_string(reinterpret_cast<std::uintptr_t>(&node)));

  for (const auto& child : node.getChildren()) {
    analyzeNode(*child);
  }

  exitScope();
}

void SemanticAnalyzer::analyzeVariable(const ASTNode& node) {
  const std::string& varName = node.getValue();
  IdentifierType varType = IdentifierType::UNKNOWN; // Placeholder, extract type from node if available

  if (tid_.identifierExists(varName, currentScope())) {
    throw std::runtime_error("Variable '" + varName + "' is already declared in scope '" + currentScope() + "'.");
  }

  tid_.addIdentifier(varName, varType, currentScope());
}

void SemanticAnalyzer::analyzeAssignment(const ASTNode& node) {
  const auto& varNode = node.getChildren().at(0);
  if (!tid_.identifierExists(varNode->getValue(), currentScope())) {
    throw std::runtime_error("Variable '" + varNode->getValue() + "' is not declared in scope '" + currentScope() + "'.");
  }

  tid_.markAsInitialized(varNode->getValue(), currentScope());
  analyzeExpression(*node.getChildren().at(1));
}

void SemanticAnalyzer::analyzeExpression(const ASTNode& node) {
  for (const auto& child : node.getChildren()) {
    analyzeNode(*child);
  }
}

void SemanticAnalyzer::analyzeLiteral(const ASTNode& node) {
  // Literals are self-contained; no additional analysis required.
}

void SemanticAnalyzer::analyzeIdentifier(const ASTNode& node) {
  const std::string& identifier = node.getValue();

  if (!tid_.identifierExists(identifier, currentScope())) {
    throw std::runtime_error("Identifier '" + identifier + "' is not declared in scope '" + currentScope() + "'.");
  }

  tid_.markAsUsed(identifier, currentScope());
}

void SemanticAnalyzer::analyzeIf(const ASTNode& node) {
  analyzeExpression(*node.getChildren().at(0));
  analyzeBlock(*node.getChildren().at(1));

  if (node.getChildren().size() > 2) {
    analyzeBlock(*node.getChildren().at(2));
  }
}

void SemanticAnalyzer::analyzeLoop(const ASTNode& node) {
  analyzeExpression(*node.getChildren().at(0));
  enterScope("loop");
  analyzeBlock(*node.getChildren().at(1));
  exitScope();
}

void SemanticAnalyzer::analyzeReturn(const ASTNode& node) {
  if (!node.getChildren().empty()) {
    analyzeExpression(*node.getChildren().at(0));
  }
}

void SemanticAnalyzer::analyzeInput(const ASTNode& node) {
  for (const auto& child : node.getChildren()) {
    analyzeIdentifier(*child);
  }
}

void SemanticAnalyzer::analyzeOutput(const ASTNode& node) {
  for (const auto& child : node.getChildren()) {
    analyzeExpression(*child);
  }
}

void SemanticAnalyzer::analyzeSwitch(const ASTNode& node) {
  analyzeExpression(*node.getChildren().at(0));

  for (std::size_t i = 1; i < node.getChildren().size(); ++i) {
    analyzeBlock(*node.getChildren().at(i));
  }
}

void SemanticAnalyzer::analyzeBreak(const ASTNode& node) {
  if (scopeStack_.empty() || scopeStack_.top() != "loop") {
    throw std::runtime_error("Break statement is not inside a loop.");
  }
}

void SemanticAnalyzer::analyzeContinue(const ASTNode& node) {
  if (scopeStack_.empty() || scopeStack_.top() != "loop") {
    throw std::runtime_error("Continue statement is not inside a loop.");
  }
}