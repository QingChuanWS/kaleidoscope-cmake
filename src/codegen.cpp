// (c) Copyright 2022 - 2025 Advanced Micro Devices, Inc. All Rights Reserved.

#include "codegen.h"
#include "ast.h"

#include "llvm/ADT/APFloat.h"
#include "llvm/IR/Constants.h"

llvm::Value *LLVMCodeGenVisitor::visit(NumberExprAST &expr) {
  return llvm::ConstantFP::get(*TheContext, llvm::APFloat(expr.getVal()));
}

llvm::Value *LLVMCodeGenVisitor::visit(VariableExprAST &expr) {
  // Look this variable up in the function.
  llvm::Value *V = NamedValues[expr.getName()];
  if (!V)
    return LogErrorV("Unknown variable name");
  return V;
}

llvm::Value *LLVMCodeGenVisitor::visit(BinaryExprAST &expr) {
  llvm::Value *L = expr.getLHS()->accept(*this);
  llvm::Value *R = expr.getRHS()->accept(*this);
  if (!L || !R)
    return nullptr;

  switch (expr.getOp()) {
  case '+':
    return Builder->CreateFAdd(L, R, "addtmp");
  case '-':
    return Builder->CreateFSub(L, R, "subtmp");
  case '*':
    return Builder->CreateFMul(L, R, "multmp");
  case '<':
    L = Builder->CreateFCmpULT(L, R, "cmptmp");
    // Convert bool 0/1 to double 0.0 or 1.0
    return Builder->CreateUIToFP(L, llvm::Type::getDoubleTy(*TheContext),
                                 "booltmp");
  default:
    return LogErrorV("invalid binary operator");
  }
}

llvm::Value *LLVMCodeGenVisitor::visit(CallExprAST &expr) {
  // Look up the name in the global module table.
  llvm::Function *CalleeF = TheModule->getFunction(expr.getCallee());
  if (!CalleeF)
    return LogErrorV("Unknown function referenced");

  // If argument mismatch error.
  auto &Args = expr.getArgs();
  if (CalleeF->arg_size() != Args.size())
    return LogErrorV("Incorrect # arguments passed");

  std::vector<llvm::Value *> ArgsV;
  for (unsigned i = 0, e = Args.size(); i != e; ++i) {
    ArgsV.push_back(Args[i]->accept(*this));
    if (!ArgsV.back())
      return nullptr;
  }

  return Builder->CreateCall(CalleeF, ArgsV, "calltmp");
}

llvm::Function *LLVMCodeGenVisitor::visit(PrototypeAST &expr) {
  // Make the function type:  double(double,double) etc.
  auto &Args = expr.getArgs();
  std::vector<llvm::Type *> Doubles(Args.size(),
                                    llvm::Type::getDoubleTy(*TheContext));
  llvm::FunctionType *FT = llvm::FunctionType::get(
      llvm::Type::getDoubleTy(*TheContext), Doubles, false);

  llvm::Function *F = llvm::Function::Create(
      FT, llvm::Function::ExternalLinkage, expr.getName(), TheModule.get());

  // Set names for all arguments.
  unsigned Idx = 0;
  for (auto &Arg : F->args())
    Arg.setName(Args[Idx++]);

  return F;
}

llvm::Function *LLVMCodeGenVisitor::visit(FunctionAST &expr) {
  // First, check for an existing function from a previous 'extern' declaration.
  auto &Proto = expr.getProto();
  auto name = Proto->getName();
  llvm::Function *TheFunction = TheModule->getFunction(name);

  if (!TheFunction)
    TheFunction = Proto->accept(*this);

  if (!TheFunction)
    return nullptr;

  // Create a new basic block to start insertion into.
  llvm::BasicBlock *BB =
      llvm::BasicBlock::Create(*TheContext, "entry", TheFunction);
  Builder->SetInsertPoint(BB);

  // Record the function arguments in the NamedValues map.
  NamedValues.clear();
  for (auto &Arg : TheFunction->args())
    NamedValues[std::string(Arg.getName())] = &Arg;

  auto &Body = expr.getBody();
  if (llvm::Value *RetVal = Body->accept(*this)) {
    // Finish off the function.
    Builder->CreateRet(RetVal);

    // Validate the generated code, checking for consistency.
    verifyFunction(*TheFunction);

    return TheFunction;
  }

  // Error reading body, remove function.
  TheFunction->eraseFromParent();
  return nullptr;
}
