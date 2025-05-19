#include "lexer.h"
#include "token.h"
#include <cstdio>

// The actual implementation of the lexer is a single function gettok()
// It's called to return the next token from standard input
// gettok works by calling getchar() function to read chars one at a time
// Then it recognizes them and stores the last character read in LastChar
Token TokenBuffer::getTok() {
  // The first thing we need to do is ignore whitespaces between tokens
  while (isspace(LastChar)) {
    LastChar = getchar();
  }

  // Next thing is recognize identifier and specific keywords like "def"
  if (isalpha(LastChar)) {
    IdentifierStr = LastChar;

    // Stacking together all alphanumeric characters into IdentifierStr
    while (isalnum(LastChar = getchar())) {
      IdentifierStr += LastChar;
    }

    if (IdentifierStr == "def") {
      return tok_def;
    }

    if (IdentifierStr == "extern") {
      return tok_extern;
    }

    return tok_identifier;
  }

  // Stacking together only numeric values
  if (isdigit(LastChar) || LastChar == '.') {
    std::string NumStr;

    do {
      NumStr += LastChar;
      LastChar = getchar();
    } while (isdigit(LastChar) || LastChar == '.');

    // Convert numeric string to numeric value
    // that we are store in NumVal
    NumVal = strtod(NumStr.c_str(), 0);
    return tok_number;
  }

  // Handling comments by skipping to the end of the line
  // and return the next token
  if (LastChar == '#') {
    do {
      LastChar = getchar();
    } while (LastChar != EOF && LastChar != '\n' && LastChar != '\r');

    if (LastChar != EOF) {
      return getTok();
    }
  }

  // If the input is an operator character
  // we return the character as a token
  if (LastChar == '+' || LastChar == '-' || LastChar == '*' ||
      LastChar == '/' || LastChar == '%') {
    Operator = LastChar;
    LastChar = getchar();
    return tok_operator;
  }

  if (LastChar == '(' || LastChar == ')') {
    Parentheses = LastChar;
    LastChar = getchar();
    return tok_parentheses;
  }

  // Finally, if the input doesn't match one of the above cases
  // it's either an operator character like '+' or the end of the file
  if (LastChar == EOF) {
    return tok_eof;
  }

  // error handling
  std::cerr << "Unknown token: " << LastChar << std::endl;
  return tok_init;
}

Token TokenBuffer::getNextToken() { return CurTok = getTok(); }
