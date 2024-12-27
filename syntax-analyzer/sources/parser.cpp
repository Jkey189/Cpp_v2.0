#include "../headers/parser.h"


std::string typeToString(const my::TokenType& tokenType) {
  switch (tokenType) {
    case my::TokenType::INTEGER_LITERAL:
      return "INTEGER_LITERAL";
    case my::TokenType::FLOAT_LITERAL:
      return "FLOAT_LITERAL";
    case my::TokenType::STRING_LITERAL:
      return "STRING_LITERAL";
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
  if (currToken_.getType() == my::TokenType::KEYWORD && currToken_.getValue() == "func") {
    parseFunction();
  } else if (currToken_.getType() == my::TokenType::IDENTIFIER) {
    parseIdentifier();
    /*parseInitialization();
    parseAssignment();*/
  } else {
    parseInstruction();
  }
}

void Parser::parseFunction() {
  expect(my::TokenType::KEYWORD);
  parseType();
  expect(my::TokenType::IDENTIFIER);
  expect(my::TokenType::LPAREN); // (

  currToken_.getType() != my::TokenType::RPAREN
  ? parseParameters(), expect(my::TokenType::RPAREN) // ( `parameters` )
  : expect(my::TokenType::RPAREN); // )

  parseBlock(); // block
}

void Parser::parseParameters() {
  parseParameter();
  while (currToken_.getType() == my::TokenType::COMMA) {
    parserAdvance(); // ,
    parseParameter(); // next parameter
  }
}

void Parser::parseParameter() {
  parseType();
  expect(my::TokenType::IDENTIFIER);
}

void Parser::parseBlock() {
  expect(my::TokenType::LBRACE);
  while (currToken_.getType() != my::TokenType::RBRACE) {
    parseInstruction();
  }
  expect(my::TokenType::RBRACE);
}

void Parser::parseInstruction() {
  if (currToken_.getType() == my::TokenType::LPAREN) {
    parserAdvance();
    parseExpression();
    bool wasRparen = false;
    if (currToken_.getType() == my::TokenType::RPAREN) {
      wasRparen = true;
      parserAdvance();
    }
    if (currToken_.getType() == my::TokenType::SEMICOLON && !wasRparen) {
      throw std::runtime_error("Syntax error: unexpected token");
    }
    if (currToken_.getType() == my::TokenType::SEMICOLON && !wasRparen) {
      expect(my::TokenType::SEMICOLON);
    }
  } else if (currToken_.getType() == my::TokenType::LBRACE) { // { block's body }
    parseBlock();
  } else if (currToken_.getType() == my::TokenType::KEYWORD && currToken_.getValue() == "cin") {  // `cin` >> ...
    parseInput();
  } else if (currToken_.getType() == my::TokenType::KEYWORD && currToken_.getValue() == "cout") { // `cout` << ...
    parseOutput();
  } else if (currToken_.getType() == my::TokenType::IF) { // `if` (`condition`) ...
    parseConditional();
  } else if (currToken_.getType() == my::TokenType::FOR ||
    currToken_.getType() == my::TokenType::WHILE) { // `for` (...) /or/ `while` (...)
    parseLoop();
  } else if (currToken_.getType() == my::TokenType::SWITCH) { // `switch` (...) (выбор)
    parseSwitch();
  } else if (currToken_.getType() == my::TokenType::BREAK ||
    currToken_.getType() == my::TokenType::CONTINUE) { // `break` /or/ `continue`
    parserAdvance();
    expect(my::TokenType::SEMICOLON);
  } else if (currToken_.getType() == my::TokenType::RETURN) { // `return`
    parserAdvance();
    parseExpression();
    expect(my::TokenType::SEMICOLON);
  } else if (currToken_.getType() == my::TokenType::IDENTIFIER) { // `identifier`
    if (lexer_.peek(currCount).getType() == my::TokenType::ASSIGN) { // ... = ...
      parseAssignment();
    } else {
      parseExpression();
    }
  } else if (currToken_.getType() == my::TokenType::SEMICOLON) { // `;`
    parserAdvance();
  } else {
    throw std::runtime_error("Syntax error: invalid instruction.");
  }
}

void Parser::parseInput() {
  if (currToken_.getType() != my::TokenType::KEYWORD || currToken_.getValue() != "cin") { // `cin`
    throw std::runtime_error("Syntax error at token: '" + currToken_.getValue() +
        "' (line: " +  std::to_string(currToken_.getLine()) +
        ", column: " + std::to_string(currToken_.getColumn()) +
        "), Expected: " + getTokenValue(currToken_.getType()));
  }
  expect(my::TokenType::IN); // >>
  parseIdentifier();

  while (currToken_.getType() == my::TokenType::IN) {
    parserAdvance();
    parseIdentifier();
  }

  expect(my::TokenType::SEMICOLON);
}

void Parser::parseOutput() {
  if (currToken_.getType() != my::TokenType::KEYWORD || currToken_.getValue() != "cout") { // `cout`
    throw std::runtime_error("Syntax error at token: '" + currToken_.getValue() +
        "' (line: " +  std::to_string(currToken_.getLine()) +
        ", column: " + std::to_string(currToken_.getColumn()) +
        "), Expected: " + getTokenValue(currToken_.getType()));
  }
  expect(my::TokenType::OUT); // <<
  parseIdentifier();

  while (currToken_.getType() != my::TokenType::SEMICOLON) {
    expect(my::TokenType::OUT);
    parseIdentifier();
  }

  expect(my::TokenType::SEMICOLON);
}

void Parser::parseConditional() {
  expect(my::TokenType::IF);
  expect(my::TokenType::LPAREN);
  parseExpression();
  expect(my::TokenType::RPAREN);
  parseBlock();

  if (currToken_.getType() == my::TokenType::ELSE) {
    parserAdvance();
    parseBlock();
  }
}

void Parser::parseLoop() {
  if (currToken_.getType() == my::TokenType::FOR) { // for (`initialization`; `condition`; `step`) ...
    parserAdvance(); // for
    expect(my::TokenType::LPAREN); // (

    // condition
    if (isType(currToken_)) { // `type`
      parseInitialization();
    } else {
      throw std::runtime_error("Syntax error: expected initialization in 'for' loop.");
    }
    expect(my::TokenType::SEMICOLON); // first `;`

    parseExpression(); // condition
    expect(my::TokenType::SEMICOLON); // second `;`

    parseStep(); // step

    expect(my::TokenType::RPAREN); // )

    parseBlock(); // { loop's `for` body }
  } else if (currToken_.getType() == my::TokenType::WHILE) {
    parserAdvance(); // while

    expect(my::TokenType::LPAREN); // (
    parseExpression(); // condition
    expect(my::TokenType::RPAREN); // )
    parseBlock(); // { loop's `while` body }
  }/* else {
    throw std::runtime_error("Syntax error: invalid loop name.");
  }*/
}

void Parser::parseInitialization() {
  parseType(); // `type`
  parseIdentifier(); // `identifier`

  if (currToken_.getType() == my::TokenType::ASSIGN) { // Проверяем `=`
    parserAdvance(); // `=`

    // Вызов parseExpression для проверки выражения справа от `=`
    parseExpression();
  }

  expect(my::TokenType::SEMICOLON); // Проверяем наличие `;` в конце
}

/*void Parser::parseInitialization() {
  if (!isType(currToken_)) { // check, is it real `type`
    throw std::runtime_error("Syntax error: expected type in initialization.");
  }

  parseType(); // `type`
  parseIdentifier(); // `identifier`

  if (currToken_.getType() != my::TokenType::ASSIGN) { // it must be '='
    throw std::runtime_error("Syntax error: invalid naming of variable type.");
  }

  parserAdvance();

  //  parsing assignment
  if (currToken_.getType() == my::TokenType::INTEGER_LITERAL ||
    currToken_.getType() == my::TokenType::FLOAT_LITERAL ||
    currToken_.getType() == my::TokenType::STRING_LITERAL ||
    currToken_.getType() == my::TokenType::IDENTIFIER) {
    parseExpression();
  } else {
    throw std::runtime_error("Syntax error: invalid value in initialization.");
  }
}*/

void Parser::parseAssignment() {
  parseIdentifier();

  while (currToken_.getType() != my::TokenType::ASSIGN) { // ... = <- эту "=" мы и ждем
    if (currToken_.getType() != my::TokenType::LBRACKET) { // smthing another shit
      throw std::runtime_error("Syntax error: assignment expected");
    }

    expect(my::TokenType::LBRACKET); // `[`
    parseIndex(); // `index`
    expect(my::TokenType::RBRACKET); // `]`
  }

  expect(my::TokenType::ASSIGN); // `=`
  parseExpression(); // = `expression`
  expect(my::TokenType::SEMICOLON); // `;`
}

void Parser::parseStep() {
  parseExpression(); // `step` (for example: i = i + 1)
}

void Parser::parseSwitch() {
  expect(my::TokenType::SWITCH); // `switch`
  expect(my::TokenType::LPAREN); // `(`
  parseExpression(); // `condition`
  expect(my::TokenType::RPAREN); // `)`

  expect(my::TokenType::LBRACE); // `{`

  while (currToken_.getType() == my::TokenType::CASE) { // `case` ...
    parserAdvance(); // `case`
    parseLiteral(); // case `literal`
    expect(my::TokenType::COLON); // `:`
    parseBlock(); // case's body
  }

  if (currToken_.getType() == my::TokenType::DEFAULT) { // `default` ...
    parserAdvance(); // `default`
    parseLiteral(); // default 'literal'
    expect(my::TokenType::COLON); // `:`
    parseBlock(); // default's block
  }

   expect(my::TokenType::RBRACE); // `}`
}

void Parser::parseLiteral() {
  if (currToken_.getType() ==my::TokenType::COMMENT_LITERAL) { // comment
    parseCommentLiteral();
  } else if (currToken_.getType() ==my::TokenType::INTEGER_LITERAL) { // int
    parseIntegerLiteral();
  } else if (currToken_.getType() ==my::TokenType::FLOAT_LITERAL) { // float
    parseFloatLiteral();
  } else if (currToken_.getType() ==my::TokenType::STRING_LITERAL) { // string
    parseStringLiteral();
  } else if (currToken_.getType() ==my::TokenType::CHAR_LITERAL) { // char
    parseCharLiteral();
  } else {
    throw std::runtime_error("Syntax error: invalid literal."); // error
  }
}

void Parser::parseCommentLiteral() {
  if (currToken_.getType() != my::TokenType::COMMENT_LITERAL) { // comment error
    throw std::runtime_error("Syntax error: expected a comment literal, got '" + currToken_.getValue() + "'");
  }
  parserAdvance();
}

void Parser::parseIntegerLiteral() {
  if (currToken_.getType() != my::TokenType::INTEGER_LITERAL) { // int error
    throw std::runtime_error("Syntax error: expected an integer literal, got '" + currToken_.getValue() + "'");
  }
  parserAdvance();
}

void Parser::parseFloatLiteral() {
  if (currToken_.getType() != my::TokenType::FLOAT_LITERAL) { // float error
    throw std::runtime_error("Syntax error: expected a float literal, got '" + currToken_.getValue() + "'");
  }
  parserAdvance();
}

void Parser::parseStringLiteral() {
  if (currToken_.getType() != my::TokenType::STRING_LITERAL) { // string error
    throw std::runtime_error("Syntax error: expected a string literal, got '" + currToken_.getValue() + "'");
  }
  parserAdvance();
}

void Parser::parseCharLiteral() {
  if (currToken_.getType() != my::TokenType::CHAR_LITERAL) { // char error
    throw std::runtime_error("Syntax error: expected a char literal, got '" + currToken_.getValue() + "'");
  }
  parserAdvance();
}

void Parser::parseExpression() { // general expression
  parseComma();
}

void Parser::parseComma() { // exp1 `,` exp2 `,` ...
  parseLogicalOr();
  if (currToken_.getType() == my::TokenType::COMMA) {
    parserAdvance();
    parseLogicalOr();
  }
}

void Parser::parseLogicalOr() { // ... `||` ...
  parseLogicalAnd();
  if (currToken_.getType() == my::TokenType::OR) {
    parserAdvance();
    parseLogicalAnd();
  }
}

void Parser::parseLogicalAnd() { // ... `&&` ...
  parseLogicalComparison();
  if (currToken_.getType() == my::TokenType::AND) {
    parserAdvance();
    parseLogicalComparison();
  }
}


void Parser::parseLogicalComparison() { // `==` /or/ `!=`
  parseComparison();
  if (currToken_.getType() == my::TokenType::EQ || currToken_.getType() == my::TokenType::NEQ) {
    parserAdvance();
    parseComparison();
  }
}

void Parser::parseComparison() { // `>` /or/ `<`
  parsePlusMinus();
  if (currToken_.getType() == my::TokenType::GT || currToken_.getType() == my::TokenType::LT) {
    parserAdvance();
    parsePlusMinus();
  }
}

void Parser::parsePlusMinus() { // `+` /or/ `-`
  parseMulDiv();
  if (currToken_.getType() == my::TokenType::PLUS || currToken_.getType() == my::TokenType::MINUS) {
    parserAdvance();
    parseMulDiv();
  }
}

void Parser::parseMulDiv() { // `*` /or/ `/`
  parseUnary();
  if (currToken_.getType() == my::TokenType::MUL || currToken_.getType() == my::TokenType::DIV) {
    parserAdvance();
    parseUnary();
  }
}

void Parser::parseUnary() { // `!` /or/ `-`...
  if (currToken_.getType() == my::TokenType::NOT || currToken_.getType() == my::TokenType::MINUS) {
    parserAdvance();
  }
  parseAtom();
}

void Parser::parseAtom() {
  if (currToken_.getType() == my::TokenType::IDENTIFIER) { // Обработка идентификатора
    parseIdentifier();

    // Индексирование массива
    while (currToken_.getType() == my::TokenType::LBRACKET) {
      parserAdvance(); // `[`
      parseIndex();
      expect(my::TokenType::RBRACKET); // `]`
    }
  } else if (currToken_.getType() == my::TokenType::LPAREN) { // Обработка открывающей скобки
    parserAdvance(); // `(`

    parseExpression(); // Рекурсивно обрабатываем выражение

    if (currToken_.getType() != my::TokenType::RPAREN) {
      throw std::runtime_error("Syntax error: expected ')' but got '" + currToken_.getValue() +
        "' at line " + std::to_string(currToken_.getLine()) +
        ", column " + std::to_string(currToken_.getColumn()) + ".");
    }
    parserAdvance(); // Закрывающая скобка `)`
  } else {
    parseLiteral(); // Обрабатываем литералы
  }
}

/*void Parser::parseAtom() {
  if (currToken_.getType() == my::TokenType::IDENTIFIER) { // `identifier`
    parseIdentifier(); // `identifier`

    if (currToken_.getType() == my::TokenType::LBRACKET) { // ... [`index`]
      parserAdvance();
      parseIndex();
      expect(my::TokenType::RBRACKET);
    }
  } else if (currToken_.getType() == my::TokenType::LPAREN) { // `(` `expression` `)`
    parserAdvance();
    parseExpression();
    expect(my::TokenType::RPAREN);
  } else {
    parseLiteral(); // `literal`
  }
}*/

void Parser::parseIndex() {
  parserAdvance();

  if (currToken_.getType() == my::TokenType::INTEGER_LITERAL) {
    parseIntegerLiteral();
  } else if (currToken_.getType() == my::TokenType::IDENTIFIER) {
    parseIdentifier();
  } else {
    throw std::runtime_error("Syntax error: invalid array index '" + currToken_.getValue() + "'");
  }
}

void Parser::parseType() {
  if (isType(currToken_)) {
    if (currToken_.getType() == my::TokenType::ARRAY) {
      parserAdvance();
      expect(my::TokenType::LT);
      parseType();
      expect(my::TokenType::GT);
    } else {
      parserAdvance();
    }
  } else {
    throw std::runtime_error("Syntax error: invalid type '" + currToken_.getValue() + "'");
  }
}

void Parser::parseIdentifier() {
  expect(my::TokenType::IDENTIFIER);
}
