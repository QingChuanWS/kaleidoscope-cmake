// (c) Copyright 2022 - 2025 Advanced Micro Devices, Inc. All Rights Reserved.
#include "ast.h"
#include <iostream>
#include <llvm/IR/Value.h>

/// LogError* - These are little helper functions for error handling.
inline std::unique_ptr<ExprAST> LogError(const char *Str) {
  fprintf(stderr, "Error: %s\n", Str);
  return nullptr;
}

inline std::unique_ptr<PrototypeAST> LogErrorP(const char *Str) {
  LogError(Str);
  return nullptr;
}

inline llvm::Value *LogErrorV(const char *Str) {
  LogError(Str);
  return nullptr;
}

void print(ExprAST &expr, int indentLevel = 0);
void print(PrototypeAST &proto, int indentLevel = 0);
void print(FunctionAST &func, int indentLevel = 0);

template <typename T> void print(const T &) {
  std::cout << "Unsupported type for printing." << std::endl;
}
