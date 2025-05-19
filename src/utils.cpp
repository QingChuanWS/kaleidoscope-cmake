#include "utils.h"
#include "ast.h"
#include <iostream>
#include <vector>
// 重载：FunctionAST
void print(const FunctionAST &func, int indentLevel) {
  std::string indent(indentLevel * 2, ' ');
  std::cout << indent << "FunctionAST:" << std::endl;
  if (func.getProto())
    print(*func.getProto(), indentLevel + 1);
  if (func.getBody())
    print(*func.getBody(), indentLevel + 1);
}

// 重载：PrototypeAST
void print(const PrototypeAST &proto, int indentLevel) {
  std::string indent(indentLevel * 2, ' ');
  std::cout << indent << "Prototype: " << proto.getName() << "(";
  const auto &args = proto.getArgs();
  for (size_t i = 0; i < args.size(); ++i) {
    std::cout << args[i];
    if (i != args.size() - 1)
      std::cout << ", ";
  }
  std::cout << ")" << std::endl;
}

// 重载：ExprAST（多态分发）
void print(const ExprAST &expr, int indentLevel) {
  std::string indent(indentLevel * 2, ' ');

  if (auto *num = dynamic_cast<const NumberExprAST *>(&expr)) {
    std::cout << indent << "NumberExprAST: " << num->getVal() << std::endl;

  } else if (auto *var = dynamic_cast<const VariableExprAST *>(&expr)) {
    std::cout << indent << "VariableExprAST: " << var->getName() << std::endl;

  } else if (auto *bin = dynamic_cast<const BinaryExprAST *>(&expr)) {
    std::cout << indent << "BinaryExprAST: " << bin->getOp() << std::endl;
    print(*bin->getLHS(), indentLevel + 1);
    print(*bin->getRHS(), indentLevel + 1);

  } else if (auto *call = dynamic_cast<const CallExprAST *>(&expr)) {
    std::cout << indent << "CallExprAST: " << call->getCallee() << "("
              << std::endl;
    const auto &args = call->getArgs();
    for (size_t i = 0; i < args.size(); ++i) {
      print(*args[i], indentLevel + 1);
    }
    std::cout << indent << ")" << std::endl;

  } else {
    std::cout << indent << "Unknown ExprAST type." << std::endl;
  }
}