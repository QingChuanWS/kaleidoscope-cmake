#include "ast.h"
#include "codegen.h"
#include "lexer.h"
#include "parser.h"
#include "token.h"
#include "utils.h"

#include <cstdio>
#include <iostream>
#include <memory>

void HandleDefinition(Parser &parser, LLVMCodeGenVisitor &visitor) {
  fprintf(stderr, "Parsing a function definition.\n");
  if (auto FnAST = parser.ParseDefinition()) {
    fprintf(stderr, "AST Tree: \n");
    print(*FnAST);
    if (auto *FnIR = FnAST->accept(visitor)) {
      fprintf(stderr, "print FnAST IR: \n");
      FnIR->print(llvm::errs());
    }
  } else {
    // Skip token for error recovery.
    parser.ModeToken();
  }
}

void HandleExtern(Parser &parser, LLVMCodeGenVisitor &visitor) {
  fprintf(stderr, "Parsing an extern prototype.\n");
  if (auto ProtoAST = parser.ParseExtern()) {
    fprintf(stderr, "AST Tree: \n");
    print(*ProtoAST);

    if (auto *ProtoIR = ProtoAST->accept(visitor)) {
      fprintf(stderr, "print Proto IR \n");
      ProtoIR->print(llvm::errs());
    }
  } else {
    // Skip token for error recovery.
    parser.ModeToken();
  }
}

void HandleTopLevelExpression(Parser &parser, LLVMCodeGenVisitor &visitor) {
  // Evaluate a top-level expression into an anonymous function.
  fprintf(stderr, "Parsing a top-level expr\n");
  if (auto FnAST = parser.ParseTopLevelExpr()) {
    fprintf(stderr, "AST Tree: \n");
    print(*FnAST);
    if (auto *FnIR = FnAST->accept(visitor)) {
      fprintf(stderr, "print Top Level Expression IR \n");
      FnIR->print(llvm::errs());
    }
  } else {
    // Skip token for error recovery.
    parser.ModeToken();
  }
}

/// top ::= definition | external | expression | ';'
void MainLoop(Parser &parser, LLVMCodeGenVisitor &visitor) {
  fprintf(stderr, "ready> ");
  parser.ModeToken();
  while (true) {
    fprintf(stderr, "ready> ");
    switch (parser.getCurTok()) {
    case tok_eof:
      fprintf(stderr, "get eof token.\n");
      return;
    case tok_semicolons: // ignore top-level semicolons.
      parser.ModeToken();
      break;
    case tok_def:
      HandleDefinition(parser, visitor);
      break;
    case tok_extern:
      HandleExtern(parser, visitor);
      break;
    default:
      HandleTopLevelExpression(parser, visitor);
      break;
    }
  }
}

int main() {
  // get input from standard input
  auto inputStream = std::make_unique<std::istream>(std::cin.rdbuf());

  Parser parser(
      std::move(std::make_unique<TokenBuffer>(std::move(inputStream))));

  LLVMCodeGenVisitor visitor;
  MainLoop(parser, visitor);
  return 0;
}