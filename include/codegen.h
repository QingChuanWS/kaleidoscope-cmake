// (c) Copyright 2022 - 2025 Advanced Micro Devices, Inc. All Rights Reserved.

#pragma once

#include "ast.h"

#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Verifier.h"

#include <map>

class ASTVisitor {
public:
  virtual llvm::Value *visit(const NumberExprAST &expr) = 0;
  virtual llvm::Value *visit(const VariableExprAST &expr) = 0;
  virtual llvm::Value *visit(BinaryExprAST &expr) = 0;
  virtual llvm::Value *visit(CallExprAST &expr) = 0;
  virtual llvm::Function *visit(PrototypeAST &proto) = 0;
  virtual llvm::Function *visit(FunctionAST &func) = 0;
  virtual ~ASTVisitor() = default;
};

class LLVMCodeGenVisitor : public ASTVisitor {
private:
private:
  std::unique_ptr<llvm::LLVMContext> TheContext;
  std::unique_ptr<llvm::Module> TheModule;
  std::unique_ptr<llvm::IRBuilder<>> Builder;

  std::map<std::string, llvm::Value *> NamedValues;

public:
  LLVMCodeGenVisitor() {
    // Open a new context and module.
    TheContext = std::make_unique<llvm::LLVMContext>();
    TheModule = std::make_unique<llvm::Module>("my cool jit", *TheContext);
    // Create a new builder for the module.
    Builder = std::make_unique<llvm::IRBuilder<>>(*TheContext);
  }

  llvm::Value *visit(const NumberExprAST &expr) override;
  llvm::Value *visit(const VariableExprAST &expr) override;
  llvm::Value *visit(BinaryExprAST &expr) override;
  llvm::Value *visit(CallExprAST &expr) override;

  llvm::Function *visit(PrototypeAST &proto) override;
  llvm::Function *visit(FunctionAST &func) override;
};
