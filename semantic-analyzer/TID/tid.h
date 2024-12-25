#ifndef TID_H
#define TID_H


#include "../../syntax-analyzer/headers/parser.h"
#include <stdexcept>


class TID {
public:
  explicit TID() = default;
  explicit TID(const std::map<std::string, my::TokenType>& symbolTable) : table_(symbolTable) {}
  TID(const TID& other) : table_(other.table_) {}
  ~TID() = default;

  bool exists(const std::string& checkedIdentifier) const;
  void insertElement(const std::string& identifier, my::TokenType type);
  my::TokenType getType(const std::string& identifier) const;


private:
  std::map<std::string, my::TokenType> table_;
};



#endif //TID_H
