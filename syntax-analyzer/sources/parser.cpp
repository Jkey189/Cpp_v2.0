#include "../headers/parser.h"


std::string typeToString(const my::TokenType& tokenType) {
  switch (tokenType) {
    case my::TokenType::INTEGER_LITERAL:
      return "INTEGER_LITERAL";
    case my::TokenType::FLOAT_LITERAL:
      return "FLOAT_LITERAL";
    case my::TokenType::STRING_LITERAL:
      return "STRING_LITERAL";
    case my::TokenType::BOOL_LITERAL:
      return "BOOL_LITERAL";
    case my::TokenType::CHAR_LITERAL:
      return "CHAR_LITERAL";
    default:
      return "UNKNOWN";
  }
}


void Parser::parseProgram() {
  while (currToken_.getType() != my::TokenType::END) {
    parseDeclaration();
  }
}

void Parser::parseDeclaration() {
  // if (currToken_.getType())
}
