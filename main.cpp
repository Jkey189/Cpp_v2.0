#include "includes/libraries.h"
#include "lexical-analyzer/headers/lexer.h"
#include "syntax-analyzer/headers/parser.h"


std::string getTokenValue(const TokenType token) {
  switch (token) {
    case TokenType::KEYWORD:
      return "KEYWORD";
    case TokenType::INT:
      return "INT";
    case TokenType::FLOAT:
      return "FLOAT";
    case TokenType::BOOL:
      return "BOOL";
    case TokenType::VOID:
      return "VOID";
    case TokenType::STRING:
      return "STRING";
    case TokenType::ARRAY:
      return "ARRAY";
    case TokenType::NUMBER:
      return "NUMBER";
    case TokenType::IDENTIFIER:
      return "IDENTIFIER";
    case TokenType::IF:
      return "IF";
    case TokenType::ELSE:
      return "ELSE";
    case TokenType::SWITCH:
      return "SWITCH";
    case TokenType::CASE:
      return "CASE";
    case TokenType::DEFAULT:
      return "DEFAULT";
    case TokenType::FOR:
      return "FOR";
    case TokenType::WHILE:
      return "WHILE";
    case TokenType::RETURN:
      return "RETURN";
    case TokenType::BREAK:
      return "BREAK";
    case TokenType::CONTINUE:
      return "CONTINUE";
    case TokenType::INTEGER_LITERAL:
      return "INTEGER_LITERAL";
    case TokenType::FLOAT_LITERAL:
      return "FLOAT_LITERAL";
    case TokenType::STRING_LITERAL:
      return "STRING_LITERAL";
    case TokenType::ASSIGN:
      return "ASSIGN";
    case TokenType::PLUS:
      return "PLUS";
    case TokenType::MINUS:
      return "MINUS";
    case TokenType::MUL:
      return "MUL";
    case TokenType::DIV:
      return "DIV";
    case TokenType::LT:
      return "LT";
    case TokenType::GT:
      return "GT";
    case TokenType::EQ:
      return "EQ";
    case TokenType::NEQ:
      return "NEQ";
    case TokenType::AND:
      return "AND";
    case TokenType::OR:
      return "OR";
    case TokenType::NOT:
      return "NOT";
    case TokenType::LPAREN:
      return "LPAREN";
    case TokenType::RPAREN:
      return "RPAREN";
    case TokenType::LBRACE:
      return "LBRACE";
    case TokenType::RBRACE:
      return "RBRACE";
    case TokenType::LBRACKET:
      return "LBRACKET";
    case TokenType::RBRACKET:
      return "RBRACKET";
    case TokenType::COMMA:
      return "COMMA";
    case TokenType::SEMICOLON:
      return "SEMICOLON";
    case TokenType::COLON:
      return "COLON";
    case TokenType::END:
      return "END";
    default: // TokenType::UNKNOWN
      return "UNKNOWN";
  }
}

void printTokens(const std::vector<Token>& tokens) {
  for (auto& currToken : tokens) {
    std::cout << "Token value: " << currToken.getValue() << std::endl;;
    std::cout << "Token type: " << getTokenValue(currToken.getType()) << std::endl;
    /*std::cout << "Token position: line: " << currToken.getLine() << std::endl;
    std::cout << "Token position: column: " << currToken.getColumn() << std::endl << std::endl;*/
    std::cout << std::endl;
  }
}


int main() {
  /*const std::string fileName = "../assets/source_file.txt";
  const std::string keywordsPath = "../assets/keywords.txt";

  std::ifstream sourceFile(fileName, std::ios::binary);

  // Проверка на открытие файла
  if (!sourceFile.is_open()) {
    std::cerr << "Failed to open file " << "\"" << fileName << "\"" << std::endl;

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
    std::cerr << "Error: file if empty." << std::endl;
    return -1;
  }

  // Отображение объема файла в байтах
  std::cout << "File size: " << lengthOfFile << " bytes" << std::endl;

  // Проверка на корректность подсчета объема файла
  if (lengthOfFile == static_cast<size_t>(-1)) {
    std::cerr << "Error: failed to determine file length." << std::endl;
    return -2;
  }

  // Запись содержимого файла в vector<char>, а затем из vector<char> в string (сразу в string циклится...)
  // И проверка на корректность считывания в vector<char>
  std::vector<char> sourceCodeVec(lengthOfFile);
  sourceFile.read(sourceCodeVec.data(), lengthOfFile);

  if (sourceFile.fail()) {
    std::cerr << "Error: failed to read the file content." << std::endl;
    perror("Error details");
    return -3;
  }

  std::string sourceCode(sourceCodeVec.begin(), sourceCodeVec.end());

  // Закрываем файл. Он нам больше не нужен
  sourceFile.close();

  LexicalAnalyzer lexer(sourceCode, keywordsPath);

  const std::vector<Token> tokens = lexer.tokenize();

  std::cout << "Source code:\n" << sourceCode << std::endl << std::endl;

  std::cout << "Tokens in this source code:" << std::endl << std::endl;
  printTokens(tokens);
  std::cout << std::endl;

  return 0;*/

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
  std::cout << "First 100 characters of the file content:" << std::endl;
  std::cout << "``````````````````````````````cpp_v2.0``````````````````````````````" << std::endl;
  for (size_t i = 0; i < std::min<size_t>(100, sourceCodeVec.size()); ++i) {
    std::cout << sourceCodeVec[i];
  }
  std::cout << "````````````````````````````````````````````````````````````````````" << std::endl;
  std::cout << std::endl << std::endl;

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
  std::cout << "Starting tokenization..." << std::endl;

  // lexer начал свою работу
  const std::vector<Token> tokens = lexer.tokenize();
  std::cout << "Tokenization completed." << std::endl;

  /*std::cout << "Source code:\n" << sourceCode << std::endl << std::endl;

  // Выводим получившиеся лексемы
  std::cout << "Tokens in this source code:" << std::endl << std::endl;
  printTokens(tokens);
  std::cout << std::endl;*/


  // Сейчас воркает синтаксический анализатор
  std::cout << std::endl << std::endl << std::endl << std::endl;
  Parser parser(lexer);

  try {
    parser.parse();
    std::cout << "Syntax analyzer has completed successfully!" << std::endl;
  } catch (const std::exception& e) {
    std::cerr << "Ошибка синтаксического анализатора: " << e.what() << std::endl;
    return -4;
  }

  return 0;
}
