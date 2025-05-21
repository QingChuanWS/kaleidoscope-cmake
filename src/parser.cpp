// (c) Copyright 2022 - 2025 Advanced Micro Devices, Inc. All Rights Reserved.

#include "parser.h"
#include "token.h"
#include "utils.h"

#include <memory>

/// GetTokPrecedence - Get the precedence of the pending binary operator token.
int Parser::GetTokPrecedence(char Operator) {
  if (!isascii(Operator))
    return -1;

  // Make sure it's a declared binop.
  int TokPrec = BinopPrecedence[Operator];
  if (TokPrec <= 0)
    return -1;
  return TokPrec;
}

/// identifierexpr
///   ::= identifier
///   ::= identifier '(' expression* ')'
std::unique_ptr<ExprAST> Parser::ParseIdentifierExpr() {
  std::string IdName = lexer->getIdentifier();

  lexer->getNextToken(); // eat identifier.

  if (lexer->getCurTok() != tok_parentheses ||
      lexer->getParentheses() != '(') // Simple variable ref.
    return std::make_unique<VariableExprAST>(IdName);

  // Call.
  lexer->getNextToken(); // eat (
  std::vector<std::unique_ptr<ExprAST>> Args;
  if (lexer->getCurTok() != tok_parentheses || lexer->getParentheses() != ')') {
    while (true) {
      if (auto Arg = ParseExpression())
        Args.push_back(std::move(Arg));
      else
        return nullptr;

      if (lexer->getCurTok() == tok_parentheses &&
          lexer->getParentheses() == ')')
        break;

      if (lexer->getCurTok() != tok_semicolons)
        return LogError("Expected ')' or ',' in argument list");
      lexer->getNextToken();
    }
  }

  // Eat the ')'.
  lexer->getNextToken();

  return std::make_unique<CallExprAST>(IdName, std::move(Args));
}

/// binoprhs
///   ::= ('+' primary)*
std::unique_ptr<ExprAST> Parser::ParseBinOpRHS(int ExprPrec,
                                               std::unique_ptr<ExprAST> LHS) {
  // If this is a binop, find its precedence.
  while (true) {
    int TokPrec = lexer->getCurTok() == tok_operator
                      ? GetTokPrecedence(lexer->getOperator())
                      : -1;

    // If this is a binop that binds at least as tightly as the current binop,
    // consume it, otherwise we are done.
    if (TokPrec < ExprPrec)
      return LHS;

    // Okay, we know this is a binop.
    int BinOp = lexer->getOperator();
    lexer->getNextToken(); // eat binop

    // Parse the primary expression after the binary operator.
    auto RHS = ParsePrimary();
    if (!RHS)
      return nullptr;

    // If BinOp binds less tightly with RHS than the operator after RHS, let
    // the pending operator take RHS as its LHS.
    int NextPrec = lexer->getCurTok() == tok_operator
                       ? GetTokPrecedence(lexer->getOperator())
                       : -1;
    if (TokPrec < NextPrec) {
      RHS = ParseBinOpRHS(TokPrec + 1, std::move(RHS));
      if (!RHS)
        return nullptr;
    }

    // Merge LHS/RHS.
    LHS =
        std::make_unique<BinaryExprAST>(BinOp, std::move(LHS), std::move(RHS));
  }
}

/// numberexpr ::= number
std::unique_ptr<ExprAST> Parser::ParseNumberExpr() {
  auto Result = std::make_unique<NumberExprAST>(lexer->getNumVal());
  lexer->getNextToken(); // consume the number
  return std::move(Result);
}

/// parenexpr ::= '(' expression ')'
std::unique_ptr<ExprAST> Parser::ParseParenExpr() {
  if (lexer->getParentheses() != '(') {
    return LogError("expected '('");
  }
  lexer->getNextToken(); // eat (.
  auto V = ParseExpression();
  if (!V)
    return nullptr;

  if (lexer->getCurTok() != tok_parentheses || lexer->getParentheses() != ')')
    return LogError("expected ')'");
  lexer->getNextToken(); // eat ).
  return V;
}

/// primary
///   ::= identifierexpr
///   ::= numberexpr
///   ::= parenexpr
std::unique_ptr<ExprAST> Parser::ParsePrimary() {
  switch (lexer->getCurTok()) {
  case tok_identifier:
    return ParseIdentifierExpr();
  case tok_number:
    return ParseNumberExpr();
  case tok_parentheses:
    return ParseParenExpr();
  default:
    return LogError("unknown token when expecting an expression");
  }
}

/// expression
///   ::= primary binoprhs
///
std::unique_ptr<ExprAST> Parser::ParseExpression() {
  auto LHS = ParsePrimary();
  if (!LHS)
    return nullptr;
  return ParseBinOpRHS(0, std::move(LHS));
}

/// prototype
///   ::= id '(' id* ')'
std::unique_ptr<PrototypeAST> Parser::ParsePrototype() {
  if (lexer->getCurTok() != tok_identifier)
    return LogErrorP("Expected function name in prototype");

  std::string FnName = lexer->getIdentifier();
  lexer->getNextToken();

  if (lexer->getCurTok() != tok_parentheses || lexer->getParentheses() != '(')
    return LogErrorP("Expected '(' in prototype");

  // Read the list of argument names.
  std::vector<std::string> ArgNames;
  while (lexer->getNextToken() == tok_identifier)
    ArgNames.push_back(lexer->getIdentifier());
  if (lexer->getCurTok() != tok_parentheses || lexer->getParentheses() != ')')
    return LogErrorP("Expected ')' in prototype");

  // success.
  lexer->getNextToken(); // eat ')'.

  return std::make_unique<PrototypeAST>(FnName, std::move(ArgNames));
}

/// definition ::= 'def' prototype expression
std::unique_ptr<FunctionAST> Parser::ParseDefinition() {
  lexer->getNextToken(); // eat def.
  auto Proto = ParsePrototype();
  if (!Proto)
    return nullptr;

  if (auto E = ParseExpression())
    return std::make_unique<FunctionAST>(std::move(Proto), std::move(E));
  return nullptr;
}

/// toplevelexpr ::= expression
std::unique_ptr<FunctionAST> Parser::ParseTopLevelExpr() {
  if (auto E = ParseExpression()) {
    // Make an anonymous proto.
    auto Proto = std::make_unique<PrototypeAST>("__anon_expr",
                                                std::vector<std::string>());
    return std::make_unique<FunctionAST>(std::move(Proto), std::move(E));
  }
  return nullptr;
}

/// external ::= 'extern' prototype
std::unique_ptr<PrototypeAST> Parser::ParseExtern() {
  lexer->getNextToken(); // eat extern.
  return ParsePrototype();
}
