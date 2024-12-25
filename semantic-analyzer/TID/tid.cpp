#include "tid.h"


bool TID::exists(const std::string &checkedIdentifier) const {
  return table_.contains(checkedIdentifier);
}

void TID::insertElement(const std::string &identifier, my::TokenType type) {
  if (exists(identifier)) {
    throw std::runtime_error("Semantic error: Identifier '" + identifier + "' is already declared.");
  }

  /*table_[identifier] = type;*/

  if (const auto [fst, snd] = table_.emplace(identifier, type); !snd) {
    throw std::runtime_error("Failed to insert identifier: " + identifier);
  }
}

my::TokenType TID::getType(const std::string &identifier) const {
  if (!exists(identifier)) {
    throw std::runtime_error("Semantic error: Identifier '" + identifier + "' is not declared.");
  }

  return table_.at(identifier);
}
