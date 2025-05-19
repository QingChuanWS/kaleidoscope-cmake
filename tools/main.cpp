#include "ast.h"
#include "lexer.h"
#include "parser.h"
#include "token.h"

#include <iostream>
#include <memory>
#include <sstream>

int main() {
  fprintf(stderr, "ready> ");
  // get input from standard input
  auto inputStream = std::make_unique<std::istream>(std::cin.rdbuf());

  Parser parser(
      std::move(std::make_unique<TokenBuffer>(std::move(inputStream))));
  parser.MainLoop();
  return 0;
}