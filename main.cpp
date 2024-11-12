#include "includes/libraries.h"
#include "lexical-analyzer/headers/lexer.h"


std::string getTokenValue(const TokenType token) {
  switch (token) {
    case TokenType::INTEGER_LITERAL:
      return "INTEGER_LITERAL";
    case TokenType::FLOAT_LITERAL:
      return "FLOAT_LITERAL";
    case TokenType::STRING_LITERAL:
      return "STRING_LITERAL";
    case TokenType::LOGICAL_LITERAL:
      return "LOGICAL_LITERAL";
    case TokenType::INTEGER_TYPE:
      return "INTEGER_TYPE";
    case TokenType::FLOAT_TYPE:
      return "FLOAT_TYPE";
    case TokenType::STRING_TYPE:
      return "STRING_TYPE";
    case TokenType::LOGICAL_TYPE:
      return "LOGICAL_TYPE";
    case TokenType::KEYWORD:
      return "KEYWORD";
    case TokenType::IDENTIFIER:
      return "IDENTIFIER";
    case TokenType::OPERATOR:
      return "OPERATOR";
    case TokenType::PUNCTUATOR:
      return "PUNCTUATOR";
    default: // TokenType::UNKNOWN
      return "UNKNOWN";
  }
}

void printTokens(const std::vector<Token>& tokens) {
  for (auto& currToken : tokens) {
    std::cout << "Token value: " << currToken.getValue() << '\n';
    std::cout << "Token type: " << getTokenValue(currToken.getLexeme()) << '\n';
    std::cout << "Token position: line: " << currToken.getLine() << '\n';
    std::cout << "Token position: column: " << currToken.getColumn() << "\n\n";
  }
}


int main() {
  std::string fileName = "../assets/source_file.txt";
  std::string keywordsPath = "../assets/keywords.txt";

  std::ifstream sourceFile(fileName);

  if (!sourceFile.is_open()) {
    std::cerr << "Failed to open file " << "\"" << fileName << "\"" << std::endl;

    if (sourceFile.bad()) {
      std::cerr << "Fatal error: bad-bit id set" << std::endl;
    }

    if (sourceFile.fail()) {
      std::cerr << "Error details: " << strerror(errno) << std::endl;
    }

    return 1;
  }

  sourceFile.seekg(0, std::ios::end);

  size_t lengthOfFile = sourceFile.tellg();
  std::string sourceCode(lengthOfFile,  ' ');

  sourceFile.seekg(0, std::ios::end);
  sourceFile.read(&sourceCode[0], lengthOfFile);

  LexicalAnalyzer lexer(sourceCode, keywordsPath);

  const std::vector<Token> tokens = LexicalAnalyzer::tokenize();

  std::cout << "Source code:" << '\n' << sourceCode << "\n\n\n";

  std::cout << "Tokens in this source code:" << "\n\n";
  printTokens(tokens);
  std::cout << std::endl;

  sourceFile.close();

  return 0;
}
