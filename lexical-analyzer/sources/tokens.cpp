#include "../headers/tokens.h"


TokenType Token::getLexeme() const {
  return type_;
}

std::string Token::getValue() const {
  return value_;
}

size_t Token::getLine() const {
  return position_.first;
}

size_t Token::getColumn() const {
  return position_.second;
}
