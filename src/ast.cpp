// (c) Copyright 2022 - 2025 Advanced Micro Devices, Inc. All Rights Reserved.

#include "ast.h"
#include "codegen.h"

llvm::Value *NumberExprAST::accept(ASTVisitor &visitor) {
  return visitor.visit(*this);
}

llvm::Value *VariableExprAST::accept(ASTVisitor &visitor) {
  return visitor.visit(*this);
}

llvm::Value *BinaryExprAST::accept(ASTVisitor &visitor) {
  return visitor.visit(*this);
}

llvm::Value *CallExprAST::accept(ASTVisitor &visitor) {
  return visitor.visit(*this);
}

llvm::Function *PrototypeAST::accept(ASTVisitor &visitor) {
  return visitor.visit(*this);
}

llvm::Function *FunctionAST::accept(ASTVisitor &visitor) {
  return visitor.visit(*this);
}