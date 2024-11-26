#include "../headers/lexer.h"


std::vector<Token> LexicalAnalyzer::tokenize() {
  // std::cout << "Start tokenization now!!!" << std::endl << std::endl; // Для проверки

  std::vector<Token> tokens;
  std::string op;

  while (position_ < program_.size()) {
    // типо жестко работаем))
    /*{
      // очищаем последнюю строчку в консоли (пока хз как)
      std::cout << static_cast<char>(8);
      std::cout << '\b';
      ++COUNT;
      COUNT %= 6;
      if (COUNT == 0) {
        std::cout << std::endl;
        ++COUNT;
      }

      // size_t sleeping = rnd() % 1000 + 500;
      size_t sleeping = 1500;
      std::this_thread::sleep_for(std::chrono::milliseconds(sleeping));
      std::this_thread::sleep_for(std::chrono::milliseconds(1000));
      std::cout << "[ ";
      if (COUNT == 1) {
        if (rng() % 2 != 0) {
          std::cout << static_cast<char>(219) << static_cast<char>(248) << static_cast<char>(248) <<
            static_cast<char>(248) << static_cast<char>(248) << static_cast<char>(248) << static_cast<char>(248);
        } else {
          std::cout << static_cast<char>(219) << static_cast<char>(219) << static_cast<char>(248) <<
            static_cast<char>(248) << static_cast<char>(248) << static_cast<char>(248) << static_cast<char>(248);
        }
      }
      if (COUNT == 2) {
        if (rng() % 2 != 0) {
          std::cout << static_cast<char>(219) << static_cast<char>(219) << static_cast<char>(219) <<
            static_cast<char>(248) << static_cast<char>(248) << static_cast<char>(248) << static_cast<char>(248);
        } else {
          std::cout << static_cast<char>(219) << static_cast<char>(219) << static_cast<char>(219) <<
            static_cast<char>(248) << static_cast<char>(248) << static_cast<char>(248) << static_cast<char>(248);
        }
      }
      if (COUNT == 3) {
        if (rng() % 2 != 0) {
          std::cout << static_cast<char>(219) << static_cast<char>(219) << static_cast<char>(219) <<
            static_cast<char>(219) << static_cast<char>(248) << static_cast<char>(248) << static_cast<char>(248);
        } else {
          std::cout << static_cast<char>(219) << static_cast<char>(219) << static_cast<char>(219) <<
            static_cast<char>(219) << static_cast<char>(219) << static_cast<char>(248) << static_cast<char>(248);
        }
      }
      if (COUNT == 4) {
        if (rng() % 2 != 0) {
          std::cout << static_cast<char>(219) << static_cast<char>(219) << static_cast<char>(219) <<
            static_cast<char>(219) << static_cast<char>(219) << static_cast<char>(248) << static_cast<char>(248);
        } else {
          std::cout << static_cast<char>(219) << static_cast<char>(219) << static_cast<char>(219) <<
            static_cast<char>(219) << static_cast<char>(219) << static_cast<char>(219) << static_cast<char>(248);
        }
      }
      if (COUNT == 5) {
        if (rng() % 2 != 0) {
          std::cout << static_cast<char>(219) << static_cast<char>(219) << static_cast<char>(219) <<
            static_cast<char>(219) << static_cast<char>(219) << static_cast<char>(219) << static_cast<char>(219);
        } else {
          std::cout << static_cast<char>(219) << static_cast<char>(219) << static_cast<char>(219) <<
            static_cast<char>(219) << static_cast<char>(219) << static_cast<char>(219) << static_cast<char>(219);
        }
      }
      std::cout << " ]";
      /*for (int i = 0; i < COUNT; ++i) {
        std::cout << static_cast<char>(248);
      }#1#
      std::cout << std::endl;
    }*/

    const char currChar = program_[position_];

    if (isSpace(currChar) || isEnter(currChar)) {
      ++position_;
      continue;
    }

    if (isDigit(currChar)) {
      // std::cout << "We've found number!" << std::endl; // Для проверки
      std::string number = getNumber();

      if (number.find('.') != std::string::npos) {
        // std::cout << "It's an integer!" << std::endl << std::endl; // Для проверки
        tokens.emplace_back(TokenType::FLOAT_LITERAL, number);
      } else {
        // std::cout << "It's an float!" << std::endl; // Для проверки
        tokens.emplace_back(TokenType::INTEGER_LITERAL, number);
      }
    } else if (isIdentifierChar(currChar)) {
      std::string word = getWord();

      if (word == "int") {
        tokens.emplace_back(TokenType::INT, word);
      } else if (word == "float") {
        tokens.emplace_back(TokenType::FLOAT, word);
      } else if (word == "char") {
        tokens.emplace_back(TokenType::CHAR, word);
      } else if (word == "bool") {
        tokens.emplace_back(TokenType::BOOL, word);
      } else if (word == "void") {
        tokens.emplace_back(TokenType::VOID, word);
      } else if (word == "string") {
        tokens.emplace_back(TokenType::STRING, word);
      }  else if (word == "array") {
        tokens.emplace_back(TokenType::ARRAY, word);
      } else if (keywords_.find(word) /*isKeyword(word)*/) {
        tokens.emplace_back(TokenType::KEYWORD, word);
      } else {
        tokens.emplace_back(TokenType::IDENTIFIER, word);
      }
    } else if (currChar == '/') {
      if (isComment(currChar)) {
        std::string comment = getComment();
        tokens.emplace_back(TokenType::COMMENT_LITERAL, "/*" + comment + "*/");
      }
    } else if (isOperator(op)) {
      // std::cout << "We've found operator!" << std::endl << std::endl; // Для проверки
      tokens.emplace_back(tokenizeOperator(op));
    } else if (isBracket(currChar)) {
      // std::cout << "We've found bracket!" << std::endl << std::endl; // Для проверки
      tokens.emplace_back(tokenizeBracket(currChar));
      ++position_;
    } else if (isAlpha(currChar) || currChar == '_') {
      tokens.emplace_back(tokenizeIdentifierOrKeyword());
    } else if (currChar == '\"') {
      std::string str = getString();
      tokens.emplace_back(TokenType::STRING_LITERAL, "\"" + str + "\"");
    } else {
      // std::cout << "Unknown..." << std::endl << std::endl; // Для проверки
      tokens.emplace_back(TokenType::UNKNOWN, std::string(1, currChar));
      ++position_;
    }
  }

  // std::cout << "The END!" << std::endl << std::endl; // Для проверки
  tokens.emplace_back(TokenType::END, "");

  return tokens;
}

Token LexicalAnalyzer::getLex() {
  if (position_ >= program_.size()) {
    return {TokenType::END, ""};
  }

  const char currChar = program_[position_];

  if (isSpace(currChar) || isEnter(currChar)) {
    ++position_;
  }

  if (isDigit(currChar)) {
    parseNumber();
  }

  if (isAlpha(currChar)) {
    parseIdentifier();
  }

  switch (currChar) {
    case '(':
      return {TokenType::LPAREN, "("};
    case ')':
      return {TokenType::RPAREN, ")"};
    case '{':
      return {TokenType::LBRACE, "{"};
    case '}':
      return {TokenType::RBRACE, "}"};
    case '[':
      return {TokenType::LBRACKET, "["};
    case ']':
      return {TokenType::RBRACKET, "]"};
    case ',':
      return {TokenType::COMMA, ","};
    case ';':
      return {TokenType::SEMICOLON, ";"};
    case ':':
      return {TokenType::COLON, ":"};
    case '=':
      return {TokenType::ASSIGN, "="};
    case '+':
      return {TokenType::PLUS, "+"};
    case '-':
      return {TokenType::MINUS, "-"};
    case '*':
      return {TokenType::MUL, "*"};
    case '/':
      return {TokenType::DIV, "/"};
    case '<':
      return {TokenType::LT, "<"};
    case '>':
      return {TokenType::GT, ">"};
    case '!':
      return {TokenType::NOT, "!"};
    case '\"':
      return {TokenType::QUOTEMARK, "\""};
    case '\\':
      return {TokenType::NEXT_STATEMENT, "\\"};
    default:
      throw std::runtime_error("Unknown character: " + std::string(1, currChar));
  }
}

Token LexicalAnalyzer::peek() {
  const size_t savedPosition = position_;
  Token nextToken = getLex();
  position_ = savedPosition;
  return nextToken;
}

bool LexicalAnalyzer::isSpace(const char c) {
  return c == ' '/* || c == '\t' || c == '\v' || c == '\f'*/;
}

bool LexicalAnalyzer::isEnter(const char c) {
  return c == '\n' || c == '\r';
}

bool LexicalAnalyzer::isAlpha(const char c) {
  return c >= 'a' && c <= 'z' || c >= 'A' && c <= 'Z';
}

bool LexicalAnalyzer::isDigit(const char c) {
  return c >= '0' && c <= '9';
}

bool LexicalAnalyzer::isIdentifierChar(const char c) {
  return isAlpha(c) || isDigit(c) || c == '_';
}

bool LexicalAnalyzer::isComment(const char c) {
  if (position_ + 1 < program_.size()) {
    ++position_;
    if (program_[position_] == '*') {
      return true;
    }
    return false;
  }
  return false;
}


std::string LexicalAnalyzer::getWord() {
  const size_t start = position_;
  while (position_ < program_.size() && isIdentifierChar(program_[position_])) {
    ++position_;
  }

  return program_.substr(start, position_ - start);
}

std::string LexicalAnalyzer::getNumber() {
  const size_t start = position_;
  bool hasDecimal = false;

  while (position_ < program_.size() && isDigit(program_[position_]) || program_[position_] == '.') {
    if (program_[position_] == '.') {
      if (hasDecimal) {
        break;
      }
      hasDecimal = true;
    }
    ++position_;
  }

  return program_.substr(start, position_ - start);
}

std::string LexicalAnalyzer::getString() {
  ++position_;
  const size_t start = position_;
  bool isSpecialSymbol = false;
  bool end = false;

  for (;;) {
    while (position_ < program_.size()) {
      if (program_[position_] == '\\') {
        if (position_ < program_.size()) {
          ++position_;
        } else {
          throw std::runtime_error("Lexer error: unexpected token | impossible to use only `\\'");
        }
      } else if (program_[position_] == '\"') {
        ++position_;
        end = true;
        break;
      } else {
        ++position_;
      }
    }

    if (position_ >= program_.size() || end) {
      break;
    }
  }

  /*std::string str;
  if (position_ < program_.size() && program_[position_] != '\"') {
    str = program_.substr(start, position_ - start);
  } else {
    throw std::runtime_error("Lexer error: unexpected lexeme | must be `\"'");
  }*/

  if (!end) {
    throw std::runtime_error("Lexer error: unexpected token | impossible to use only one `\"'");
  }

  std::string str = program_.substr(start, position_ - start);
  ++position_;

  return str;
}

std::string LexicalAnalyzer::getComment() {
  ++position_;
  const size_t start = position_;

  for (;;) {
    while (position_ < program_.size() && program_[position_] != '*') {
      ++position_;
    }

    if (position_ < program_.size()) {
      if (program_[position_ + 1] == '/') {
        break;
      }
    } else {
      break;
    }
  }

  std::string comment = program_.substr(start, position_ - start);
  position_ += 2;

  return comment;
}


void LexicalAnalyzer::initializeKeywords(const std::string& keywordsPath) const {
  std::ifstream keywordsFile(keywordsPath);

  if (!keywordsFile.is_open()) {
    std::cerr << "Failed to open file " << "\"" << keywordsPath << "\"" << std::endl;

    if (keywordsFile.bad()) {
      std::cerr << "Fatal error: bad-bit is set" << std::endl;
    }

    if (keywordsFile.fail()) {
      std::cerr << "Error details: " << strerror(errno) << std::endl;
    }
  }

  std::string keyword;
  while (std::getline(keywordsFile, keyword)) {
    if (!keyword.empty()) {
      keywords_.insert(keyword);
    }
  }

  keywordsFile.close();
}

bool LexicalAnalyzer::isOperator(std::string& op) const {
  op.clear();

  // Надеемся, что это работает
  const char ch = program_[position_];
  op = std::string(1, ch);

  if (ch == ',' || ch == ':' || ch == ';' || ch == '\\') {
    return true;
  }

  if (ch == '+' || ch == '-' || ch == '*' || ch == '/') {
    return true;
  }

  if (ch == '=' && position_ + 1 < program_.size()) {
    const char nextChar = program_[position_ + 1];

    if (nextChar == '=') {
      op += nextChar;
    }
  }

  if (ch == '!' && position_ + 1 < program_.size()) {
    const char nextChar = program_[position_ + 1];

    if (nextChar == '=') {
      op += nextChar;
    }
  }

  if ((ch == '<' || ch == '>') && position_ + 1 < program_.size()) {
    const char nextChar = program_[position_ + 1];

    if (ch == nextChar) {
      op += nextChar;
    }

    if (ch != nextChar && !isSpace(nextChar) && !isEnter(nextChar)) {
      op += nextChar;
      throw std::runtime_error("Lexer error: unexpected lexeme | impossible to use `" + op + "'");
    }
  }

  if ((ch == '&' || ch == '|') && position_ + 1 < program_.size()) {
    const char nextChar = program_[position_ + 1];

    if (ch == nextChar) {
      op += nextChar;
    } else {
      op += nextChar;
      throw std::runtime_error("Lexer error: unexpected lexeme | impossible to use `" + op + "'");
    }
  }

  static const std::unordered_set<std::string> operators = {
    "!", "=",
    "==", "!=",
    "<", ">",
    "&&", "||",
    ">>", "<<"
  };

  return operators.contains(op);
}

Token LexicalAnalyzer::tokenizeIdentifierOrKeyword() {
  const size_t start = position_;

  while (position_ < program_.size() && isIdentifierChar(program_[position_])) {
    ++position_;
  }

  std::string identifier = program_.substr(start, position_);

  if (isKeyword(identifier)) {
    return {TokenType::KEYWORD, identifier};
  }
  return {TokenType::IDENTIFIER, identifier};
}

bool LexicalAnalyzer::isKeyword(const std::string& id) {
  if (id == "cin" || id == "cout" || id == "if" || id == "else" || id == "switch" || id == "case" || id == "break" || id == "continue" ||
    id == "for" || id == "while" || id == "true" || id == "false" || id == "const") {
    return true;
  }
  return false;
}

TokenType LexicalAnalyzer::keywordToTokenType(const std::string& keyword) {
  if (keyword == "int") {
    return TokenType::INT;
  }
  if (keyword == "float") {
    return TokenType::FLOAT;
  }
  if (keyword == "bool") {
    return TokenType::BOOL;
  }
  if (keyword == "void") {
    return TokenType::VOID;
  }
  if (keyword == "string") {
    return TokenType::STRING;
  }
  if (keyword == "array") {
    return TokenType::ARRAY;
  }
  if (keyword == "if") {
    return TokenType::IF;
  }
  if (keyword == "else") {
    return TokenType::ELSE;
  }
  if (keyword == "switch") {
    return TokenType::SWITCH;
  }
  if (keyword == "case") {
    return TokenType::CASE;
  }
  if (keyword == "default") {
    return TokenType::DEFAULT;
  }
  if (keyword == "for") {
    return TokenType::FOR;
  }
  if (keyword == "while") {
    return TokenType::WHILE;
  }
  if (keyword == "return") {
    return TokenType::RETURN;
  }
  if (keyword == "break") {
    return TokenType::BREAK;
  }
  if (keyword == "continue") {
    return TokenType::CONTINUE;
  }
  return TokenType::IDENTIFIER;
}

/*Token LexicalAnalyzer::tokenizeNumber() {
  std::string number;
  bool isFloat = false;

  while (position_ < program_.size() && (isDigit(program_[position_]) || program_[position_] == '.')) {
    if (program_[position_] == '.') {
      if (isFloat) break;
      isFloat = true;
    }
    number += program_[position_++];
  }

  if (isFloat) {
    return {TokenType::FLOAT_LITERAL, number};
  } else {
    return {TokenType::INTEGER_LITERAL, number};
  }
}*/

Token LexicalAnalyzer::tokenizeOperator(std::string& op) {
  position_ += op.size();

  if (op == ">>") {
    return {TokenType::IN, op};
  }
  if (op == "<<") {
    return {TokenType::OUT, op};
  }
  if (op == "=") {
    return {TokenType::ASSIGN, op};
  }
  if (op == "+") {
    return {TokenType::PLUS, op};
  }
  if (op == "-") {
    return {TokenType::MINUS, op};
  }
  if (op == "*") {
    return {TokenType::MUL, op};
  }
  if (op == "/") {
    return {TokenType::DIV, op};
  }
  if (op == "<") {
    return {TokenType::LT, op};
  }
  if (op == ">") {
    return {TokenType::GT, op};
  }
  if (op == "==") {
    return {TokenType::EQ, op};
  }
  if (op == "!=") {
    return {TokenType::NEQ, op};
  }
  if (op == "&&") {
    return {TokenType::AND, op};
  }
  if (op == "||") {
    return {TokenType::OR, op};
  }
  if (op == "!") {
    return {TokenType::NOT, op};
  }
  if (op == "\"") {
    return {TokenType::QUOTEMARK, op};
  }
  if (op == ",") {
    return {TokenType::COMMA, op};
  }
  if (op == ":") {
    return {TokenType::COLON, op};
  }
  if (op == ";") {
    return {TokenType::SEMICOLON, op};
  }
  if (op == "\\") {
    return {TokenType::NEXT_STATEMENT, op};
  }
  return {TokenType::UNKNOWN, op};
}

bool LexicalAnalyzer::isBracket(const char c) {
  return c == '(' || c == ')' || c == '{' || c == '}' || c == '[' || c == ']';
}


Token LexicalAnalyzer::tokenizeBracket(const char c) {
  if (c == '(') {
    return {TokenType::LPAREN, std::string(1, c)};
  }
  if (c == ')') {
    return {TokenType::RPAREN, std::string(1, c)};
  }
  if (c == '{') {
    return {TokenType::LBRACE, std::string(1, c)};
  }
  if (c == '}') {
    return {TokenType::RBRACE, std::string(1, c)};
  }
  if (c == '[') {
    return {TokenType::LBRACKET, std::string(1, c)};
  }
  if (c == ']') {
    return {TokenType::RBRACKET, std::string(1, c)};
  }
  return {TokenType::UNKNOWN, std::string(1, c)};
}

Token LexicalAnalyzer::parseIdentifier() {
  std::string id;

  while (position_ < program_.size() && (isAlpha(program_[position_]) || program_[position_] == '_')) {
    id.push_back(program_[position_++]);
  }

  /*if (id =="int") {
    return {TokenType::INT, id};
  }
  if (id =="float") {
    return {TokenType::FLOAT, id};
  }
  if (id =="bool") {
    return {TokenType::BOOL, id};
  }
  if (id =="void") {
    return {TokenType::VOID, id};
  }
  if (id =="string") {
    return {TokenType::STRING, id};
  }
  if (id =="array") {
    return {TokenType::ARRAY, id};
  }
  if (id =="if") {
    return {TokenType::IF, id};
  }
  if (id =="else") {
    return {TokenType::ELSE, id};
  }
  if (id =="switch") {
    return {TokenType::SWITCH, id};
  }
  if (id =="case") {
    return {TokenType::CASE, id};
  }
  if (id =="default") {
    return {TokenType::DEFAULT, id};
  }
  if (id =="for") {
    return {TokenType::FOR, id};
  }
  if (id =="while") {
    return {TokenType::WHILE, id};
  }
  if (id =="return") {
    return {TokenType::RETURN, id};
  }
  if (id =="break") {
    return {TokenType::BREAK, id};
  }
  if (id =="continue") {
    return {TokenType::CONTINUE, id};
  }*/
  return {TokenType::IDENTIFIER, id};
}
