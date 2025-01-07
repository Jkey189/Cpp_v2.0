#include "includes/libraries.h"
#include "global_functions/global_funcs.h"

#include "lexical-analyzer/headers/lexer.h"
#include "syntax-analyzer/headers/parser.h"
#include "semantic-analyzer/headers/semantic.h"

#include <filesystem>


void printTokens(const std::vector<Token>& tokens) {
  for (const auto& currToken : tokens) {
    std::cout << "Token value: " << currToken.getValue() << '\n';
    std::cout << "Token type: " << getTokenValue(currToken.getType()) << '\n';
    std::cout << std::endl;
  }
}


int main() {
  // files' paths with Cppt code and keywords
  const std::string fileName = "../assets/source_file.cppt";
  const std::string keywordsPath = "../assets/keywords.txt";

  // open file...
  std::ifstream sourceFile(fileName, std::ios::binary);

  // check that we were able to open it correctly
  if (!sourceFile.is_open()) {
    std::cerr << "Failed to open file \"" << fileName << "\"" << std::endl;

    if (sourceFile.fail()) {
      std::cerr << "Failed to open file - fail state set." << std::endl;
    }
    return 1;
  }

  // count file's length
  sourceFile.seekg(0, std::ios::end);
  int lengthOfFile = static_cast<int>(sourceFile.tellg()); // else - warning (not cool)
  sourceFile.seekg(0, std::ios::beg);

  // check that opening file is not empty
  if (lengthOfFile == 0) {
    std::cerr << "Error: file is empty." << std::endl;
    return -1;
  }

  // print file's value in bytes
  std::cout << "File size: " << lengthOfFile << " bytes" << std::endl << std::endl;

  // check the correctness of the file's size calculation
  if (lengthOfFile == -1) {
    std::cerr << "Error: failed to determine file length." << std::endl;
    return -2;
  }

  // crutch - file's content --> vector<char> --> std::string
  std::vector<char> sourceCodeVec(lengthOfFile);
  sourceFile.read(sourceCodeVec.data(), lengthOfFile);

  // check the correctness of counting to vector<char>
  if (sourceFile.fail()) {
    std::cerr << "Error: failed to read the file content." << std::endl;
    perror("Error details");
    return -3;
  }

  // convert to string
  std::string sourceCode(sourceCodeVec.begin(), sourceCodeVec.end());

  // close file
  sourceFile.close();

  // output file's content (for check)
  std::cout << "Full source code content:" << std::endl;
  std::cout << "````````````````````````````````Cppt````````````````````````````````" << std::endl;
  std::cout << sourceCode << std::endl;
  std::cout << "````````````````````````````````````````````````````````````````````" << std::endl;

  // analyze file's content by lexer
  LexicalAnalyzer lexer(sourceCode, keywordsPath);

  // debugging output (lexer is going to work)
  std::cout << "Starting tokenization..." << std::endl << std::endl;

  const std::vector<Token> tokens = lexer.tokenize();

  // std::cout << "Source code:\n" << sourceCode << std::endl << std::endl;
  std::cout << "Tokenization completed." << std::endl << std::endl << std::endl;
  std::cout << "Tokens in this source code:" << std::endl << std::endl;
  /*std::this_thread::sleep_for(std::chrono::milliseconds(500));*/
  printTokens(tokens);
  std::cout << std::endl;

  // parser is going to work
  std::cout << std::endl << std::endl << std::endl << std::endl;
  Parser parser(lexer);

  // catch parser's errors
  try {
    parser.program();
    std::cout << "Syntax analyzer has completed successfully!" << std::endl;

    // Semantic analysis
    /*try {
      // start semantic here:
      TID tid;
      SemanticAnalyzer semantic(tid, );
      std::cout << "Semantic analysis completed successfully!" << std::endl;
    } catch (const std::exception& e) {
      std::cerr << "Semantic analysis error: " << e.what() << std::endl;
      return -1;
    }*/
  } catch (const std::exception& e) {
    std::cerr << "Parser's errors: " << e.what() << std::endl;
    return -4;
  }

  return 0;
}
