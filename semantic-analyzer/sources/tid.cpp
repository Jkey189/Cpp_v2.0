#include "../headers/tid.h"


void TID::addIdentifier(const std::string& name, IdentifierType type, const std::string& scope) {
  if (identifierExists(name, scope)) {
    throw std::runtime_error("Identifier '" + name + "' already exists in scope '" + scope + "'.");
  }
  table_[name].emplace_back(name, scope, type, false, false, "");
}

IdentifierInfo TID::getIdentifier(const std::string& name, const std::string& scope) const {
  for (const auto& info : table_.at(name)) {
    if (info.scope == scope) {
      return info;
    }
  }
  throw std::runtime_error("Identifier '" + name + "' not found in scope '" + scope + "'.");
}

void TID::markAsUsed(const std::string& name, const std::string& scope) {
  if (auto* identifier = findIdentifier(name, scope)) {
    identifier->isUsed = true;
  } else {
    throw std::runtime_error("Identifier '" + name + "' not found in scope '" + scope + "'.");
  }
}

void TID::markAsInitialized(const std::string& name, const std::string& scope) {
  if (auto* identifier = findIdentifier(name, scope)) {
    identifier->isInitialized = true;
  } else {
    throw std::runtime_error("Identifier '" + name + "' not found in scope '" + scope + "'.");
  }
}

bool TID::identifierExists(const std::string& name, const std::string& scope) const {
  if (table_.contains(name)) {
    for (const auto& info : table_.at(name)) {
      if (info.scope == scope) {
        return true;
      }
    }
  }
  return false;
}

void TID::printTable() const {
  for (const auto& [name, infos] : table_) {
    for (const auto& info : infos) {
      std::cout << "Name: " << info.name
                << ", Type: " << static_cast<int>(info.type)
                << ", Scope: " << info.scope
                << ", Initialized: " << (info.isInitialized ? "Yes" : "No")
                << ", Used: " << (info.isUsed ? "Yes" : "No")
                << ", Additional Info: " << info.additionalInfo
                << std::endl;
    }
  }
}

IdentifierInfo* TID::findIdentifier(const std::string& name, const std::string& scope) {
  if (table_.contains(name)) {
    for (auto& info : table_[name]) {
      if (info.scope == scope) {
        return &info;
      }
    }
  }
  return nullptr;
}
