#ifndef AST_NODE_H
#define AST_NODE_H


#include <utility>

#include "../../global_functions/global_funcs.h"


enum class ASTNodeType {
  PROGRAM,
  FUNCTION,
  BLOCK,
  VARIABLE_DECLARATION,
  ASSIGNMENT,
  EXPRESSION,
  LITERAL,
  IDENTIFIER,
  IF_STATEMENT,
  LOOP_STATEMENT,
  RETURN_STATEMENT,
  INPUT,
  OUTPUT,
  SWITCH,
  BREAK,
  CONTINUE
};

class ASTNode {
public:
  explicit ASTNode(const ASTNodeType& type, std::string  value = "") :
  type_(type), value_(std::move(value)) {}

  void addChild(const std::shared_ptr<ASTNode>& child) {
    children_.emplace_back(child);
  }

  [[nodiscard]] const std::vector<std::shared_ptr<ASTNode>>& getChildren() const {
    return children_;
  }

  [[nodiscard]] ASTNodeType getType() const { return type_; }
  [[nodiscard]] std::string getValue() const { return value_; }

private:
  ASTNodeType type_;
  std::string value_;
  std::vector<std::shared_ptr<ASTNode>> children_;
};


#endif //AST_NODE_H
