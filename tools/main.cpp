#include "ast.h"
#include "codegen.h"
#include "kaleidoscope.h"
#include "lexer.h"
#include "parser.h"
#include "token.h"
#include "utils.h"

#include <iostream>
#include <memory>

void HandleDefinition(Parser &parser, LLVMCodeGenVisitor &visitor) {
  CLI_INFO("Parsing a function definition.\n");
  if (auto FnAST = parser.ParseDefinition()) {
    PRINT_AST(*FnAST);
    if (auto *FnIR = FnAST->accept(visitor)) {
      PRINT_IR(FnIR);
    }
  } else {
    // Skip token for error recovery.
    parser.ModeToken();
  }
}

void HandleExtern(Parser &parser, LLVMCodeGenVisitor &visitor) {
  CLI_INFO("Parsing a extern prototype.\n");
  if (auto ProtoAST = parser.ParseExtern()) {
    PRINT_AST(*ProtoAST);
    if (auto *ProtoIR = ProtoAST->accept(visitor)) {
      PRINT_IR(ProtoIR);
    }
  } else {
    // Skip token for error recovery.
    parser.ModeToken();
  }
}

void HandleTopLevelExpression(Parser &parser, LLVMCodeGenVisitor &visitor) {
  CLI_INFO("Parsing a top-level expr\n");
  if (auto FnAST = parser.ParseTopLevelExpr()) {
    PRINT_AST(*FnAST);
    if (auto *FnIR = FnAST->accept(visitor)) {
      PRINT_IR(FnIR);
    }
  } else {
    // Skip token for error recovery.
    parser.ModeToken();
  }
}

/// top ::= definition | external | expression | ';'
void MainLoop(Parser &parser, LLVMCodeGenVisitor &visitor) {
  CLI_INFO("ready> ");
  parser.ModeToken();
  while (true) {
    CLI_INFO("ready> ");
    switch (parser.getCurTok()) {
    case tok_eof:
      CLI_INFO("get eof token.\n");
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

  llvm::InitializeNativeTarget();
  llvm::InitializeNativeTargetAsmPrinter();
  llvm::InitializeNativeTargetAsmParser();

  Parser parser(
      std::move(std::make_unique<TokenBuffer>(std::move(inputStream))));

  LLVMCodeGenVisitor visitor;
  MainLoop(parser, visitor);
  return 0;
}