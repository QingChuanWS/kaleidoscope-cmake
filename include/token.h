#pragma once

#include <string>

enum Token {
  tok_init = 0,
  // End Of File
  tok_eof = -1,

  // Commands
  tok_def = -2,
  tok_extern = -3,

  // Primary
  tok_identifier = -4,
  tok_number = -5,
  tok_operator = -6,
};

inline std::string getTokenEnumerationString(int token) {
  switch (token) {
  case tok_eof:
    return "tok_eof";
  case tok_identifier:
    return "tok_identifier";
  case tok_number:
    return "tok_number";
  case tok_def:
    return "tok_def";
  case tok_extern:
    return "tok_extern";
  case tok_operator:
    return "tok_operator";
  case tok_init:
    return "tok_init";
  default:
    return std::to_string(token);
  }
}