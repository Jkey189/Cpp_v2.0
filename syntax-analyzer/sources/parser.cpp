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
  if (currToken_.getType() == my::TokenType::LBRACE) { // { blocks body }
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

  while (currToken_.getType() == my::TokenType::OUT) {
    parserAdvance();
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

    parseBlock(); // { loops `for` body }
  } else if (currToken_.getType() == my::TokenType::WHILE) {
    parserAdvance(); // while

    expect(my::TokenType::LPAREN); // (
    parseExpression(); // condition
    expect(my::TokenType::RPAREN); // )
    parseBlock(); // { loops `while` body }
  }/* else {
    throw std::runtime_error("Syntax error: invalid loop name.");
  }*/
}

void Parser::parseInitialization() {
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
}

void Parser::parseAssignment() {
  parseIdentifier();

  while (currToken_.getType() != my::TokenType::ASSIGN) { // ... = <- эту "=" мы и ждем
    if (currToken_.getType() != my::TokenType::LBRACKET) { // smthing another shit
      throw std::runtime_error("Syntax error: assignment expected");
    }

    expect(my::TokenType::LBRACKET);
    parseIndex();
    expect(my::TokenType::RBRACKET);
  }

  expect(my::TokenType::ASSIGN);
  parseExpression();
  expect(my::TokenType::SEMICOLON);
}

void Parser::parseStep() {
  parseExpression();
}

void Parser::parseSwitch() {
  expect(my::TokenType::SWITCH);
  expect(my::TokenType::LPAREN);
  parseExpression();
}

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
