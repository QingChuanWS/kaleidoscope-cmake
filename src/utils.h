// (c) Copyright 2022 - 2025 Advanced Micro Devices, Inc. All Rights Reserved.
#include <ast.h>
#include <iostream>

void print(const ExprAST &expr, int indentLevel = 0);
void print(const PrototypeAST &proto, int indentLevel = 0);
void print(const FunctionAST &func, int indentLevel = 0);

template <typename T> void print(const T &) {
  std::cout << "Unsupported type for printing." << std::endl;
}
