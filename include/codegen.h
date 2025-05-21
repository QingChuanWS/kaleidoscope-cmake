// (c) Copyright 2022 - 2025 Advanced Micro Devices, Inc. All Rights Reserved.

#pragma once

#include "ast.h"
#include "kaleidoscope.h"

#include "llvm/ADT/STLExtras.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/PassManager.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Verifier.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/StandardInstrumentations.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/Target/TargetMachine.h"
#include "llvm/Transforms/InstCombine/InstCombine.h"
#include "llvm/Transforms/Scalar.h"
#include "llvm/Transforms/Scalar/GVN.h"
#include "llvm/Transforms/Scalar/Reassociate.h"
#include "llvm/Transforms/Scalar/SimplifyCFG.h"

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
  std::unique_ptr<llvm::orc::KaleidoscopeJIT> TheJIT;
  std::unique_ptr<llvm::FunctionPassManager> TheFPM;
  std::unique_ptr<llvm::LoopAnalysisManager> TheLAM;
  std::unique_ptr<llvm::FunctionAnalysisManager> TheFAM;
  std::unique_ptr<llvm::CGSCCAnalysisManager> TheCGAM;
  std::unique_ptr<llvm::ModuleAnalysisManager> TheMAM;
  std::unique_ptr<llvm::PassInstrumentationCallbacks> ThePIC;
  std::unique_ptr<llvm::StandardInstrumentations> TheSI;
  std::unique_ptr<llvm::IRBuilder<>> Builder;

  std::map<std::string, llvm::Value *> NamedValues;

  llvm::ExitOnError ExitOnErr;

public:
  LLVMCodeGenVisitor() {
    // Open a new context and module.
    TheContext = std::make_unique<llvm::LLVMContext>();
    TheJIT = ExitOnErr(llvm::orc::KaleidoscopeJIT::Create());

    TheModule = std::make_unique<llvm::Module>("KaleidoscopeJIT", *TheContext);
    TheModule->setDataLayout(TheJIT->getDataLayout());

    // Create a new builder for the module.
    Builder = std::make_unique<llvm::IRBuilder<>>(*TheContext);

    // Create a new pass manager attached to it.
    TheFPM = std::make_unique<llvm::FunctionPassManager>();
    // Create new pass and analysis managers.
    TheLAM = std::make_unique<llvm::LoopAnalysisManager>();
    TheFAM = std::make_unique<llvm::FunctionAnalysisManager>();
    TheCGAM = std::make_unique<llvm::CGSCCAnalysisManager>();
    TheMAM = std::make_unique<llvm::ModuleAnalysisManager>();
    ThePIC = std::make_unique<llvm::PassInstrumentationCallbacks>();
    TheSI = std::make_unique<llvm::StandardInstrumentations>(true);
    TheSI->registerCallbacks(*ThePIC, TheFAM.get());

    // Add transform passes.
    // Do simple "peephole" optimizations and bit-twiddling optzns.
    TheFPM->addPass(llvm::InstCombinePass());
    // Reassociate expressions.
    TheFPM->addPass(llvm::ReassociatePass());
    // Eliminate Common SubExpressions.
    TheFPM->addPass(llvm::GVNPass());
    // Simplify the control flow graph (deleting unreachable blocks, etc).
    TheFPM->addPass(llvm::SimplifyCFGPass());

    llvm::PassBuilder PB;
    PB.registerModuleAnalyses(*TheMAM);
    PB.registerFunctionAnalyses(*TheFAM);
    PB.crossRegisterProxies(*TheLAM, *TheFAM, *TheCGAM, *TheMAM);
  }

  llvm::Value *visit(const NumberExprAST &expr) override;
  llvm::Value *visit(const VariableExprAST &expr) override;
  llvm::Value *visit(BinaryExprAST &expr) override;
  llvm::Value *visit(CallExprAST &expr) override;

  llvm::Function *visit(PrototypeAST &proto) override;
  llvm::Function *visit(FunctionAST &func) override;
};
