// (c) Copyright 2022 - 2025 Advanced Micro Devices, Inc. All Rights Reserved.

#pragma once

#include <llvm/IR/Value.h>

#include <memory>
#include <string>
#include <vector>

class ASTVisitor;

class ExprAST {
public:
  virtual ~ExprAST() = default;
  virtual llvm::Value *accept(ASTVisitor &visitor) = 0;
};

class NumberExprAST : public ExprAST {
  double Val;

public:
  NumberExprAST(double Val) : Val(Val) {}
  double getVal() const { return Val; }

  llvm::Value *accept(ASTVisitor &visitor) override;
};

class VariableExprAST : public ExprAST {
  std::string Name;

public:
  VariableExprAST(const std::string &Name) : Name(Name) {}
  const std::string &getName() const { return Name; }

  llvm::Value *accept(ASTVisitor &visitor) override;
};

class BinaryExprAST : public ExprAST {
  char Op;
  std::unique_ptr<ExprAST> LHS, RHS;

public:
  BinaryExprAST(char Op, std::unique_ptr<ExprAST> LHS,
                std::unique_ptr<ExprAST> RHS)
      : Op(Op), LHS(std::move(LHS)), RHS(std::move(RHS)) {}

  char getOp() const { return Op; }
  std::unique_ptr<ExprAST> &getLHS() { return LHS; }
  std::unique_ptr<ExprAST> &getRHS() { return RHS; }

  llvm::Value *accept(ASTVisitor &visitor) override;
};

class CallExprAST : public ExprAST {
  std::string Callee;
  std::vector<std::unique_ptr<ExprAST>> Args;

public:
  CallExprAST(const std::string &Callee,
              std::vector<std::unique_ptr<ExprAST>> Args)
      : Callee(Callee), Args(std::move(Args)) {}

  const std::string &getCallee() const { return Callee; }
  std::vector<std::unique_ptr<ExprAST>> &getArgs() { return Args; }

  llvm::Value *accept(ASTVisitor &visitor) override;
};

class PrototypeAST {
  std::string Name;
  std::vector<std::string> Args;

public:
  PrototypeAST(const std::string &Name, std::vector<std::string> Args)
      : Name(Name), Args(std::move(Args)) {}

  const std::string &getName() const { return Name; }
  std::vector<std::string> &getArgs() { return Args; }

  llvm::Function *accept(ASTVisitor &visitor);
};

class FunctionAST {
  std::unique_ptr<PrototypeAST> Proto;
  std::unique_ptr<ExprAST> Body;

public:
  FunctionAST(std::unique_ptr<PrototypeAST> Proto,
              std::unique_ptr<ExprAST> Body)
      : Proto(std::move(Proto)), Body(std::move(Body)) {}

  std::unique_ptr<PrototypeAST> &getProto() { return Proto; }
  std::unique_ptr<ExprAST> &getBody() { return Body; }

  llvm::Function *accept(ASTVisitor &visitor);
};
