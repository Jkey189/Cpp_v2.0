#include "../headers/tokens.h"


my::TokenType Token::getType() const {
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
