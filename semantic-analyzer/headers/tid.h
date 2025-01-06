#ifndef TID_H
#define TID_H


#include <utility>

#include "../../includes/libraries.h"
#include "../../global_functions/global_funcs.h"


enum class IdentifierType {
  FUNCTION, INT, FLOAT, CHAR, BOOL, VOID, STRING, ARRAY, UNKNOWN
};

struct IdentifierInfo {
  explicit IdentifierInfo(std::string  iName, std::string  iScope,
    const IdentifierType& iType,
    const bool iIsInitialized, const bool iIsUsed,
    std::string  iAdditionalInfo) : name(std::move(iName)), scope(std::move(iScope)),
  type(iType), isInitialized(iIsInitialized), isUsed(iIsUsed),
  additionalInfo(std::move(iAdditionalInfo)) {}


  std::string name;
  std::string scope;

  IdentifierType type;

  bool isInitialized;
  bool isUsed;

  std::string additionalInfo;
};


class TID {
public:
  // Adds a new identifier to the table
  void addIdentifier(const std::string& name, IdentifierType type, const std::string& scope);

  // Retrieves information about an identifier
  [[nodiscard]] IdentifierInfo getIdentifier(const std::string& name, const std::string& scope) const;

  // Updates an identifier's usage status
  void markAsUsed(const std::string& name, const std::string& scope);

  // Updates an identifier's initialization status
  void markAsInitialized(const std::string& name, const std::string& scope);

  // Checks if an identifier exists in the table
  [[nodiscard]] bool identifierExists(const std::string& name, const std::string& scope) const;

  // Prints the entire table (for debugging purposes)
  void printTable() const;

private:
  std::unordered_map<std::string, std::vector<IdentifierInfo>> table_;

  // Helper to find an identifier within a scope
  IdentifierInfo* findIdentifier(const std::string& name, const std::string& scope);
};


#endif //TID_H
