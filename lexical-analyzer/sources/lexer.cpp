#include "../headers/lexer.h"


std::vector<Token> LexicalAnalyzer::tokenize() {
  // std::cout << "Start tokenization now!!!" << std::endl << std::endl; // Для проверки

  std::vector<Token> tokens;
  std::string op;

  while (position_ < program_.size()) {
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
        tokens.emplace_back(my::TokenType::FLOAT_LITERAL, number);
      } else {
        // std::cout << "It's an float!" << std::endl; // Для проверки
        tokens.emplace_back(my::TokenType::INTEGER_LITERAL, number);
      }
    } else if (isIdentifierChar(currChar)) {
      std::string word = getWord();

      if (word == "if") {
        tokens.emplace_back(my::TokenType::IF, word);
      } else if (word == "else") {
        tokens.emplace_back(my::TokenType::ELSE, word);
      } else if (word == "switch") {
        tokens.emplace_back(my::TokenType::SWITCH, word);
      } else if (word == "case") {
        tokens.emplace_back(my::TokenType::CASE, word);
      } else if (word == "default") {
        tokens.emplace_back(my::TokenType::DEFAULT, word);
      } else if (word == "return") {
        tokens.emplace_back(my::TokenType::RETURN, word);
      } else if (word == "break") {
        tokens.emplace_back(my::TokenType::BREAK, word);
      } else if (word == "continue") {
        tokens.emplace_back(my::TokenType::CONTINUE, word);
      } else if (word == "for") {
        tokens.emplace_back(my::TokenType::FOR, word);
      } else if (word == "while") {
        tokens.emplace_back(my::TokenType::WHILE, word);
      } else if (word == "int") {
        tokens.emplace_back(my::TokenType::INT, word);
      } else if (word == "float") {
        tokens.emplace_back(my::TokenType::FLOAT, word);
      } else if (word == "char") {
        tokens.emplace_back(my::TokenType::CHAR, word);
      } else if (word == "bool") {
        tokens.emplace_back(my::TokenType::BOOL, word);
      } else if (word == "void") {
        tokens.emplace_back(my::TokenType::VOID, word);
      } else if (word == "string") {
        tokens.emplace_back(my::TokenType::STRING, word);
      }  else if (word == "array") {
        tokens.emplace_back(my::TokenType::ARRAY, word);
      } else if (keywords_.find(word) && isKeyword(word)) {
        tokens.emplace_back(my::TokenType::KEYWORD, word);
      } else {
        tokens.emplace_back(my::TokenType::IDENTIFIER, word);
      }
    } else if (currChar == '/') {
      if (isComment(currChar)) {
        std::string comment = getComment();
        tokens.emplace_back(my::TokenType::COMMENT_LITERAL, "\"" + comment + "\"");
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
      tokens.emplace_back(my::TokenType::STRING_LITERAL, "\"" + str + "\"");
    } else if (currChar == '\'') {
      std::string charLiter = getCharLiteral();
      tokens.emplace_back(my::TokenType::CHAR_LITERAL, "\'" + charLiter + "\'");
    } else {
      // std::cout << "Unknown..." << std::endl << std::endl; // Для проверки
      tokens.emplace_back(my::TokenType::UNKNOWN, std::string(1, currChar));
      ++position_;
    }
  }

  // std::cout << "The END!" << std::endl << std::endl; // Для проверки
  tokens.emplace_back(my::TokenType::END, "");

  return tokens;
}

Token LexicalAnalyzer::getLex() {
  return getTokens().at(0);
}

/*Token LexicalAnalyzer::peek(const size_t ind) {
  if (const auto& tokens = getTokens(); ind < tokens.size()) {
    return tokens[ind];
  }
  return Token(my::TokenType::END, "");
}*/

Token LexicalAnalyzer::peek(const size_t ind) {
  return ind < getTokens().size() ? getTokens()[ind + 1] :
  (getTokens()[ind + 1].getType() == my::TokenType::END) ? getTokens()[ind + 1] :
  throw std::runtime_error("memory corrupt");
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
  if (position_ < program_.size()) {
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
  if (isAlpha(program_[position_])) {
    throw std::runtime_error("Lexer error: invalid write of number");
  }

  return program_.substr(start, position_ - start);
}

std::string LexicalAnalyzer::getString() {
  ++position_;
  const size_t start = position_;
  bool end = false;

  for (;;) {
    while (position_ < program_.size()) {
      if (program_[position_] == '\\') {
        if (isSpace(program_[position_ + 1]) || isEnter(program_[position_ + 1])) {
          throw std::runtime_error("Lexer error: unexpected token | warning: unknown escape sequence: '\\040' 299");
        }
        if (program_[position_ + 1] == '\"') {
          position_ += 2;
        }
      } else if (program_[position_] == '\"') {
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

  // crutch
  std::string answer;
  for (const auto ch : str) {
    if (ch != '\\') {
      answer.push_back(ch);
    }
  }

  return answer;
}

std::string LexicalAnalyzer::getCharLiteral() {
  ++position_;
  const char charLiter = program_[position_++];
  if (program_[position_] != '\'') {
    throw std::runtime_error("Lexer error: unclosed character literal");
  }
  ++position_;
  return std::string(1, charLiter);
}

std::string LexicalAnalyzer::getComment() {
  ++position_;
  const size_t start = position_;
  bool end = false;
  int count = 0;

  for (;;) {
    while (position_ < program_.size()) {
      if (position_ > 0 && program_[position_] == '*' && program_[position_ - 1] == '/') {
        ++count;
      } else if (program_[position_] == '*' && position_ < program_.size()) {
        if (program_[position_ + 1] == '/') {
          end = true;
          break;
        }
      }
      ++position_;
    }
    if (end) {
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
    if (const char nextChar = program_[position_ + 1]; nextChar == '=') {
      op += nextChar;
    }
  }

  if (ch == '!' && position_ + 1 < program_.size()) {
    if (const char nextChar = program_[position_ + 1]; nextChar == '=') {
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
    if (const char nextChar = program_[position_ + 1]; ch == nextChar) {
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
  std::string identifier = program_.substr(start, position_ - start);
  if (keywords_.find(identifier) || isKeyword(identifier)) {
    return {my::TokenType::KEYWORD, identifier};
  }
  return {my::TokenType::IDENTIFIER, identifier};

  /*const size_t start = position_;

  while (position_ < program_.size() && isIdentifierChar(program_[position_])) {
    ++position_;
  }

  std::string identifier = program_.substr(start, position_);

  if (isKeyword(identifier)) {
    return {my::TokenType::KEYWORD, identifier};
  }
  return {my::TokenType::IDENTIFIER, identifier};*/
}

bool LexicalAnalyzer::isKeyword(const std::string& id) {
  if (id == "true" || id == "false" || id == "const" ||
    id == "cin" || id == "cout" || id == "func") {
    return true;
  }
  return false;
}

my::TokenType LexicalAnalyzer::keywordToTokenType(const std::string& keyword) {
  if (keyword == "int") {
    return my::TokenType::INT;
  }
  if (keyword == "float") {
    return my::TokenType::FLOAT;
  }
  if (keyword == "bool") {
    return my::TokenType::BOOL;
  }
  if (keyword == "void") {
    return my::TokenType::VOID;
  }
  if (keyword == "string") {
    return my::TokenType::STRING;
  }
  if (keyword == "array") {
    return my::TokenType::ARRAY;
  }
  if (keyword == "if") {
    return my::TokenType::IF;
  }
  if (keyword == "else") {
    return my::TokenType::ELSE;
  }
  if (keyword == "switch") {
    return my::TokenType::SWITCH;
  }
  if (keyword == "case") {
    return my::TokenType::CASE;
  }
  if (keyword == "default") {
    return my::TokenType::DEFAULT;
  }
  if (keyword == "for") {
    return my::TokenType::FOR;
  }
  if (keyword == "while") {
    return my::TokenType::WHILE;
  }
  if (keyword == "return") {
    return my::TokenType::RETURN;
  }
  if (keyword == "break") {
    return my::TokenType::BREAK;
  }
  if (keyword == "continue") {
    return my::TokenType::CONTINUE;
  }
  return my::TokenType::IDENTIFIER;
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
    return {my::TokenType::FLOAT_LITERAL, number};
  } else {
    return {my::TokenType::INTEGER_LITERAL, number};
  }
}*/

Token LexicalAnalyzer::tokenizeOperator(std::string& op) {
  position_ += op.size();

  if (op == ">>") {
    return {my::TokenType::IN, op};
  }
  if (op == "<<") {
    return {my::TokenType::OUT, op};
  }
  if (op == "=") {
    return {my::TokenType::ASSIGN, op};
  }
  if (op == "+") {
    return {my::TokenType::PLUS, op};
  }
  if (op == "-") {
    return {my::TokenType::MINUS, op};
  }
  if (op == "*") {
    return {my::TokenType::MUL, op};
  }
  if (op == "/") {
    return {my::TokenType::DIV, op};
  }
  if (op == "<") {
    return {my::TokenType::LT, op};
  }
  if (op == ">") {
    return {my::TokenType::GT, op};
  }
  if (op == "==") {
    return {my::TokenType::EQ, op};
  }
  if (op == "!=") {
    return {my::TokenType::NEQ, op};
  }
  if (op == "&&") {
    return {my::TokenType::AND, op};
  }
  if (op == "||") {
    return {my::TokenType::OR, op};
  }
  if (op == "!") {
    return {my::TokenType::NOT, op};
  }
  if (op == "\"") {
    return {my::TokenType::QUOTEMARK, op};
  }
  if (op == ",") {
    return {my::TokenType::COMMA, op};
  }
  if (op == ":") {
    return {my::TokenType::COLON, op};
  }
  if (op == ";") {
    return {my::TokenType::SEMICOLON, op};
  }
  if (op == "\\") {
    return {my::TokenType::NEXT_STATEMENT, op};
  }
  return {my::TokenType::UNKNOWN, op};
}

bool LexicalAnalyzer::isBracket(const char c) {
  return c == '(' || c == ')' || c == '{' || c == '}' || c == '[' || c == ']';
}


Token LexicalAnalyzer::tokenizeBracket(const char c) {
  if (c == '(') {
    return {my::TokenType::LPAREN, std::string(1, c)};
  }
  if (c == ')') {
    return {my::TokenType::RPAREN, std::string(1, c)};
  }
  if (c == '{') {
    return {my::TokenType::LBRACE, std::string(1, c)};
  }
  if (c == '}') {
    return {my::TokenType::RBRACE, std::string(1, c)};
  }
  if (c == '[') {
    return {my::TokenType::LBRACKET, std::string(1, c)};
  }
  if (c == ']') {
    return {my::TokenType::RBRACKET, std::string(1, c)};
  }
  return {my::TokenType::UNKNOWN, std::string(1, c)};
}

Token LexicalAnalyzer::parseIdentifier() {
  std::string id;

  while (position_ < program_.size() && (isAlpha(program_[position_]) || program_[position_] == '_')) {
    id.push_back(program_[position_++]);
  }

  /*if (id =="int") {
    return {my::TokenType::INT, id};
  }
  if (id =="float") {
    return {my::TokenType::FLOAT, id};
  }
  if (id =="bool") {
    return {my::TokenType::BOOL, id};
  }
  if (id =="void") {
    return {my::TokenType::VOID, id};
  }
  if (id =="string") {
    return {my::TokenType::STRING, id};
  }
  if (id =="array") {
    return {my::TokenType::ARRAY, id};
  }
  if (id =="if") {
    return {my::TokenType::IF, id};
  }
  if (id =="else") {
    return {my::TokenType::ELSE, id};
  }
  if (id =="switch") {
    return {my::TokenType::SWITCH, id};
  }
  if (id =="case") {
    return {my::TokenType::CASE, id};
  }
  if (id =="default") {
    return {my::TokenType::DEFAULT, id};
  }
  if (id =="for") {
    return {my::TokenType::FOR, id};
  }
  if (id =="while") {
    return {my::TokenType::WHILE, id};
  }
  if (id =="return") {
    return {my::TokenType::RETURN, id};
  }
  if (id =="break") {
    return {my::TokenType::BREAK, id};
  }
  if (id =="continue") {
    return {my::TokenType::CONTINUE, id};
  }*/
  return {my::TokenType::IDENTIFIER, id};
}
