#pragma once

#include "token.h"
#include <iostream>
#include <string>

// Provide a simple token buffer

class TokenBuffer {
public:
  TokenBuffer()
      : CurTok(Token::tok_init), IdentifierStr(""), NumVal(0.0), Operator(' '),
        Parentheses(' '), InputBuffer(std::cin.rdbuf()), LastChar(' ') {}

  // The actual implementation of the lexer is a single function gettok()
  // It's called to return the next token from standard input
  // gettok works by calling getchar() function to read chars one at a time
  // Then it recognizes them and stores the last character read in LastChar
  Token getTok();
  Token getNextToken();

  std::string getIdentifier() const { return IdentifierStr; }

  double getNumVal() const { return NumVal; }

  char getOperator() const { return Operator; }

  char getParentheses() const { return Parentheses; }

private:
  // Handling comments by skipping to the end of the line
  // and return the next token
  Token CurTok;
  // If the current token is an identifier
  // IdentifierStr will hold the name of the identifier
  std::string IdentifierStr;
  // If the current token is a numeric literal
  // NumVal holds its value
  double NumVal;
  // If the current token is an operator
  char Operator;
  // If the current token is an comma
  char Parentheses;

  std::streambuf *InputBuffer;

  int LastChar;
};