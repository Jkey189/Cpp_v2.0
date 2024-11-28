#include "includes/libraries.h"
#include "lexical-analyzer/headers/lexer.h"
#include "syntax-analyzer/headers/parser.h"


std::string getTokenValue(const my::TokenType token) {
  switch (token) {
    case my::TokenType::KEYWORD:
      return "KEYWORD";
    case my::TokenType::INT:
      return "INT";
    case my::TokenType::FLOAT:
      return "FLOAT";
    case my::TokenType::BOOL:
      return "BOOL";
    case my::TokenType::VOID:
      return "VOID";
    case my::TokenType::STRING:
      return "STRING";
    case my::TokenType::ARRAY:
      return "ARRAY";
    case my::TokenType::NUMBER:
      return "NUMBER";
    case my::TokenType::IDENTIFIER:
      return "IDENTIFIER";
    case my::TokenType::IF:
      return "IF";
    case my::TokenType::ELSE:
      return "ELSE";
    case my::TokenType::SWITCH:
      return "SWITCH";
    case my::TokenType::CASE:
      return "CASE";
    case my::TokenType::DEFAULT:
      return "DEFAULT";
    case my::TokenType::FOR:
      return "FOR";
    case my::TokenType::WHILE:
      return "WHILE";
    case my::TokenType::RETURN:
      return "RETURN";
    case my::TokenType::BREAK:
      return "BREAK";
    case my::TokenType::CONTINUE:
      return "CONTINUE";
    case my::TokenType::INTEGER_LITERAL:
      return "INTEGER_LITERAL";
    case my::TokenType::FLOAT_LITERAL:
      return "FLOAT_LITERAL";
    case my::TokenType::STRING_LITERAL:
      return "STRING_LITERAL";
    case my::TokenType::ASSIGN:
      return "ASSIGN";
    case my::TokenType::PLUS:
      return "PLUS";
    case my::TokenType::MINUS:
      return "MINUS";
    case my::TokenType::MUL:
      return "MUL";
    case my::TokenType::DIV:
      return "DIV";
    case my::TokenType::LT:
      return "LT";
    case my::TokenType::GT:
      return "GT";
    case my::TokenType::EQ:
      return "EQ";
    case my::TokenType::NEQ:
      return "NEQ";
    case my::TokenType::AND:
      return "AND";
    case my::TokenType::OR:
      return "OR";
    case my::TokenType::NOT:
      return "NOT";
    case my::TokenType::LPAREN:
      return "LPAREN";
    case my::TokenType::RPAREN:
      return "RPAREN";
    case my::TokenType::LBRACE:
      return "LBRACE";
    case my::TokenType::RBRACE:
      return "RBRACE";
    case my::TokenType::LBRACKET:
      return "LBRACKET";
    case my::TokenType::RBRACKET:
      return "RBRACKET";
    case my::TokenType::COMMA:
      return "COMMA";
    case my::TokenType::SEMICOLON:
      return "SEMICOLON";
    case my::TokenType::COLON:
      return "COLON";
    case my::TokenType::END:
      return "END";
    default: // my::TokenType::UNKNOWN
      return "UNKNOWN";
  }
}

void printTokens(const std::vector<Token>& tokens) {
  for (auto& currToken : tokens) {
    std::cout << "Token value: " << currToken.getValue() << std::endl;
    std::cout << "Token type: " << getTokenValue(currToken.getType()) << std::endl;
    /*std::cout << "Token position: line: " << currToken.getLine() << std::endl;
    std::cout << "Token position: column: " << currToken.getColumn() << std::endl << std::endl;*/
    std::cout << std::endl;
    /*std::this_thread::sleep_for(std::chrono::milliseconds(500));*/
  }
}


int main() {
  // Пути к файлам с кодом и keywords
  const std::string fileName = "../assets/source_file.txt";
  const std::string keywordsPath = "../assets/keywords.txt";

  // Открываем файл...
  std::ifstream sourceFile(fileName, std::ios::binary);

  // Проверка на то, что мы смогли его корректно открыть
  if (!sourceFile.is_open()) {
    std::cerr << "Failed to open file \"" << fileName << "\"" << std::endl;

    if (sourceFile.fail()) {
      std::cerr << "Failed to open file - fail state set." << std::endl;
    }
    return 1;
  }

  // Подсчет длины файла
  sourceFile.seekg(0, std::ios::end);
  size_t lengthOfFile = sourceFile.tellg();
  sourceFile.seekg(0, std::ios::beg);

  // Проверка на то, что файл не пустой
  if (lengthOfFile == 0) {
    std::cerr << "Error: file is empty." << std::endl;
    return -1;
  }

  // Отображение объема файла в байтах
  std::cout << "File size: " << lengthOfFile << " bytes" << std::endl << std::endl;

  // Проверка на корректность подсчета объема файла
  if (lengthOfFile == static_cast<size_t>(-1)) {
    std::cerr << "Error: failed to determine file length." << std::endl;
    return -2;
  }

  // Запись содержимого файла в vector<char>, а затем из vector<char> в string (сразу в string циклится...)
  // Upd: уже не циклится, проблема в lexer'е, исправим далее...
  std::vector<char> sourceCodeVec(lengthOfFile);
  sourceFile.read(sourceCodeVec.data(), lengthOfFile);

  // Проверка на корректность считывания в vector<char>
  // Upd: корректно, но мы все равно оставим, т.к. такая проверка все-таки должна быть
  if (sourceFile.fail()) {
    std::cerr << "Error: failed to read the file content." << std::endl;
    perror("Error details");
    return -3;
  }

  // Выводим первые несколько символов из файла, чтобы убедиться, что содержимое правильно прочитано
  /*std::cout << "First 100 characters of the file content:" << std::endl;
  std::cout << "``````````````````````````````cpp_v2.0``````````````````````````````" << std::endl;
  for (size_t i = 0; i < std::min<size_t>(100, sourceCodeVec.size()); ++i) {
    std::cout << sourceCodeVec[i];
  }
  std::cout << "````````````````````````````````````````````````````````````````````" << std::endl;
  std::cout << std::endl << std::endl;*/

  // Преобразование в строку для дальнейшей обработки
  std::string sourceCode(sourceCodeVec.begin(), sourceCodeVec.end());

  // Закрываем файл. Он нам больше не нужен
  sourceFile.close();

  // Выводим только что считанное содержимое для проверки
  std::cout << "Full source code content:" << std::endl;
  std::cout << "``````````````````````````````cpp_v2.0``````````````````````````````" << std::endl;
  std::cout << sourceCode << std::endl;
  std::cout << "````````````````````````````````````````````````````````````````````" << std::endl;

  // Теперь анализируем содержимое с помощью lexer'а
  LexicalAnalyzer lexer(sourceCode, keywordsPath);

  // Добавляем отладочный вывод, чтобы увидеть, когда в работу вступает lexer. Следующий ход за ним
  std::cout << "Starting tokenization..." << std::endl << std::endl;

  // lexer начал свою работу
  const std::vector<Token> tokens = lexer.tokenize();

  // std::cout << "Source code:\n" << sourceCode << std::endl << std::endl;
  std::cout << "Tokenization completed." << std::endl << std::endl << std::endl;
  std::cout << "Tokens in this source code:" << std::endl << std::endl;
  /*std::this_thread::sleep_for(std::chrono::milliseconds(500));*/
  printTokens(tokens);
  std::cout << std::endl;


  // Сейчас воркает синтаксический анализатор
  std::cout << std::endl << std::endl << std::endl << std::endl;
  Parser parser(lexer);

  try {
    parser.program();
    std::cout << "Syntax analyzer has completed successfully!" << std::endl;
  } catch (const std::exception& e) {
    std::cerr << "Parser's errors: " << e.what() << std::endl;
    return -4;
  }

  return 0;
}
