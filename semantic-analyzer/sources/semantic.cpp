#include "../headers/semantic.h"

void SemanticAnalyzer::analyze() {
  while (parser_.getCurrToken().getType() != my::TokenType::END) {
    if (Token token = parser_.getCurrToken(); token.getType() == my::TokenType::KEYWORD && token.getValue() == "func") {
      checkFunction(token);
    } else if (token.getType() == my::TokenType::IDENTIFIER) {
      if (parser_.getLexer().peek(1).getType() == my::TokenType::ASSIGN) {
        checkAssignment(token);
      } else {
        checkVariable(token);
      }
    }
    parser_.parserAdvance();
  }
}

void SemanticAnalyzer::checkFunction(const Token& token) const {
  parser_.parserAdvance(); // Пропускаем `func`

  // Проверяем возвращаемый тип
  const Token typeToken = parser_.getCurrToken();
  if (!parser_.isType(typeToken)) {
    throw std::runtime_error("Semantic error: invalid return type '" + typeToken.getValue() + "'.");
  }
  parser_.parserAdvance();

  // Проверяем имя функции
  const Token nameToken = parser_.getCurrToken();
  if (tid_.exists(nameToken.getValue())) {
    throw std::runtime_error("Semantic error: function name '" + nameToken.getValue() + "' is already used.");
  }
  tid_.add(nameToken.getValue(), typeToken.getType());
  parser_.parserAdvance();

  // Пропускаем параметры функции
  parser_.expect(my::TokenType::LPAREN);
  while (parser_.getCurrToken().getType() != my::TokenType::RPAREN) {
    parser_.parserAdvance();
  }
  parser_.expect(my::TokenType::RPAREN);
}

void SemanticAnalyzer::checkVariable(const Token& token) {
  // Проверяем, объявлена ли переменная
  if (!tid_.exists(token.getValue())) {
    throw std::runtime_error("Semantic error: variable '" + token.getValue() +
                             "' not declared at line " + std::to_string(token.getLine()) +
                             ", column " + std::to_string(token.getColumn()) + ".");
  }

  // Проверяем, если это часть присваивания, совпадают ли типы lvalue и rvalue
  const Token& currToken = parser_.getCurrToken();

  if (currToken.getType() == my::TokenType::ASSIGN) {
    parser_.parserAdvance(); // Пропускаем '='

    Token nextToken = parser_.getCurrToken();

    // Получаем тип переменной (lvalue)
    my::TokenType variableType = tid_.getType(token.getValue());

    // Получаем тип значения (rvalue)
    my::TokenType valueType;
    if (nextToken.getType() == my::TokenType::IDENTIFIER) {
      checkVariable(nextToken); // Проверяем, объявлена ли переменная rvalue
      valueType = tid_.getType(nextToken.getValue());
    } else {
      valueType = nextToken.getType();
    }

    // Сравниваем типы
    if (variableType != valueType) {
      throw std::runtime_error("Semantic error: type mismatch in assignment to '" +
                               token.getValue() + "' at line " + std::to_string(token.getLine()) +
                               ", column " + std::to_string(token.getColumn()) + ".");
    }
  }
}

void SemanticAnalyzer::checkAssignment(const Token& token) {
  checkVariable(token);
  parser_.parserAdvance(); // Пропускаем идентификатор
  parser_.expect(my::TokenType::ASSIGN);

  // Проверка типа переменной и значения
  Token valueToken = parser_.getCurrToken();
  my::TokenType variableType = tid_.getType(token.getValue());
  if (variableType != valueToken.getType()) {
    throw std::runtime_error("Semantic error: type mismatch for variable '" + token.getValue() +
      "' (expected: " + getTokenValue(variableType) + ", got: " + getTokenValue(valueToken.getType()) + ").");
  }
  parser_.parserAdvance();
}

void SemanticAnalyzer::checkExpression(const Token& token) {
  // Следим за состоянием выражения
  bool expectOperator = false; // Ожидаем ли оператор
  bool expectOperand = true;   // Ожидаем ли операнд

  while (parser_.getCurrToken().getType() != my::TokenType::SEMICOLON &&
         parser_.getCurrToken().getType() != my::TokenType::RPAREN &&
         parser_.getCurrToken().getType() != my::TokenType::END) {
    Token currToken = parser_.getCurrToken();

    // Если токен - идентификатор
    if (currToken.getType() == my::TokenType::IDENTIFIER) {
      // Проверяем, объявлена ли переменная
      checkVariable(currToken);

      // Если ожидаем оператор, значит идентификатор не на своем месте
      if (expectOperator) {
        throw std::runtime_error("Semantic error: unexpected identifier '" + currToken.getValue() +
                                 "' at line " + std::to_string(currToken.getLine()) +
                                 ", column " + std::to_string(currToken.getColumn()) +
                                 ". Expected an operator.");
      }

      // Переключаем ожидания
      expectOperator = true;
      expectOperand = false;
    }

    // Если токен - литерал (целочисленный, строковый, вещественный, символ)
    else if (currToken.getType() == my::TokenType::INTEGER_LITERAL ||
             currToken.getType() == my::TokenType::FLOAT_LITERAL ||
             currToken.getType() == my::TokenType::STRING_LITERAL ||
             currToken.getType() == my::TokenType::CHAR_LITERAL) {
      // Литералы допустимы, если мы ожидаем операнд
      if (!expectOperand) {
        throw std::runtime_error("Semantic error: unexpected literal '" + currToken.getValue() +
                                 "' at line " + std::to_string(currToken.getLine()) +
                                 ", column " + std::to_string(currToken.getColumn()) +
                                 ". Expected an operator.");
      }

      // Переключаем ожидания
      expectOperator = true;
      expectOperand = false;
    }

    // Если токен - оператор (например, +, -, *, /)
    else if (currToken.getType() == my::TokenType::PLUS || currToken.getType() == my::TokenType::MINUS ||
             currToken.getType() == my::TokenType::MUL || currToken.getType() == my::TokenType::DIV ||
             currToken.getType() == my::TokenType::EQ || currToken.getType() == my::TokenType::NEQ ||
             currToken.getType() == my::TokenType::AND || currToken.getType() == my::TokenType::OR ||
             currToken.getType() == my::TokenType::LT || currToken.getType() == my::TokenType::GT) {
      // Операторы допустимы, если мы ожидаем оператор
      if (!expectOperator) {
        throw std::runtime_error("Semantic error: unexpected operator '" + currToken.getValue() +
                                 "' at line " + std::to_string(currToken.getLine()) +
                                 ", column " + std::to_string(currToken.getColumn()) +
                                 ". Expected an operand.");
      }

      // Переключаем ожидания
      expectOperator = false;
      expectOperand = true;
    }

    // Если токен - открывающая скобка (для группировки)
    else if (currToken.getType() == my::TokenType::LPAREN) {
      if (!expectOperand) {
        throw std::runtime_error("Semantic error: unexpected '(' at line " +
                                 std::to_string(currToken.getLine()) +
                                 ", column " + std::to_string(currToken.getColumn()) +
                                 ". Expected an operator.");
      }
      parser_.parserAdvance(); // Пропускаем '('
      checkExpression(currToken); // Рекурсивно проверяем содержимое скобок
      if (parser_.getCurrToken().getType() != my::TokenType::RPAREN) {
        throw std::runtime_error("Semantic error: missing closing ')' at line " +
                                 std::to_string(currToken.getLine()) +
                                 ", column " + std::to_string(currToken.getColumn()) + ".");
      }
    }

    // Если токен - закрывающая скобка (она будет обработана в вызове выше)
    else if (currToken.getType() == my::TokenType::RPAREN) {
      break; // Выходим из проверки
    }

    // Если токен неизвестный
    else {
      throw std::runtime_error("Semantic error: invalid token '" + currToken.getValue() +
                               "' at line " + std::to_string(currToken.getLine()) +
                               ", column " + std::to_string(currToken.getColumn()) + ".");
    }

    // Продвигаемся к следующему токену
    parser_.parserAdvance();
  }

  // Если в конце выражения ожидается операнд, но мы достигли конца - ошибка
  if (expectOperand) {
    throw std::runtime_error("Semantic error: incomplete expression at line " +
                             std::to_string(token.getLine()) + ", column " +
                             std::to_string(token.getColumn()) + ".");
  }
}

std::vector<std::string> SemanticAnalyzer::generateRPN() {
  std::vector<std::string> output; // Финальный ПОЛИЗ
  std::stack<std::string> operators; // Стек операторов

  while (parser_.getCurrToken().getType() != my::TokenType::SEMICOLON &&
         parser_.getCurrToken().getType() != my::TokenType::END) {
    Token currToken = parser_.getCurrToken();

    // Если это литерал или идентификатор
    if (currToken.getType() == my::TokenType::IDENTIFIER ||
        currToken.getType() == my::TokenType::INTEGER_LITERAL ||
        currToken.getType() == my::TokenType::FLOAT_LITERAL ||
        currToken.getType() == my::TokenType::STRING_LITERAL ||
        currToken.getType() == my::TokenType::CHAR_LITERAL) {
      output.push_back(currToken.getValue());
    }

    // Если это оператор
    else if (currToken.getType() == my::TokenType::PLUS ||
             currToken.getType() == my::TokenType::MINUS ||
             currToken.getType() == my::TokenType::MUL ||
             currToken.getType() == my::TokenType::DIV) {
      while (!operators.empty() && getPrecedence(operators.top()) >= getPrecedence(currToken.getValue())) {
        output.push_back(operators.top());
        operators.pop();
      }
      operators.push(currToken.getValue());
    }

    // Если это открывающая скобка
    else if (currToken.getType() == my::TokenType::LPAREN) {
      operators.push(currToken.getValue());
    }

    // Если это закрывающая скобка
    else if (currToken.getType() == my::TokenType::RPAREN) {
      while (!operators.empty() && operators.top() != "(") {
        output.push_back(operators.top());
        operators.pop();
      }
      if (operators.empty()) {
        throw std::runtime_error("Semantic error: mismatched parentheses.");
      }
      operators.pop(); // Убираем '(' из стека
    }

    parser_.parserAdvance();
  }

  // Выгружаем оставшиеся операторы
  while (!operators.empty()) {
    if (operators.top() == "(") {
      throw std::runtime_error("Semantic error: mismatched parentheses.");
    }
    output.push_back(operators.top());
    operators.pop();
  }

  return output;
}

// Вспомогательный метод для приоритетов операций
int SemanticAnalyzer::getPrecedence(const std::string& op) {
  if (op == "+" || op == "-") return 1;
  if (op == "*" || op == "/") return 2;
  return 0;
}
