// (c) Copyright 2022 - 2025 Advanced Micro Devices, Inc. All Rights Reserved.

#pragma once

#include "ast.h"
#include "lexer.h"
#include <map>
#include <memory>

class Parser {
  std::unique_ptr<TokenBuffer> lexer;
  /// BinopPrecedence - This holds the precedence for each binary operator that
  /// is defined.
  std::map<char, int> BinopPrecedence;

public:
  Parser(std::unique_ptr<TokenBuffer> lexer,
         std::map<char, int> BinopPrecedence = {{'<', 10},
                                                {'+', 20},
                                                {'-', 20},
                                                {'*', 40}})
      : lexer(std::move(lexer)), BinopPrecedence(BinopPrecedence) {}
  // mode to the next token.
  void ModeToken() { lexer->getNextToken(); }

  Token getCurTok() { return lexer->getCurTok(); }

  int GetTokPrecedence(char Operator);
  /// parenexpr ::= '(' expression ')'
  std::unique_ptr<ExprAST> ParseParenExpr();
  /// numberexpr ::= number
  std::unique_ptr<ExprAST> ParseNumberExpr();
  /// binoprhs
  ///   ::= ('+' primary)*
  std::unique_ptr<ExprAST> ParseBinOpRHS(int ExprPrec,
                                         std::unique_ptr<ExprAST> LHS);
  /// identifierexpr
  ///   ::= identifier
  ///   ::= identifier '(' expression* ')'
  std::unique_ptr<ExprAST> ParseIdentifierExpr();
  /// primary
  ///   ::= identifierexpr
  ///   ::= numberexpr
  ///   ::= parenexpr
  std::unique_ptr<ExprAST> ParsePrimary();
  /// expression
  ///   ::= primary binoprhs
  ///
  std::unique_ptr<ExprAST> ParseExpression();
  /// prototype
  ///   ::= id '(' id* ')'
  std::unique_ptr<PrototypeAST> ParsePrototype();
  /// definition ::= 'def' prototype expression
  std::unique_ptr<FunctionAST> ParseDefinition();

  /// external ::= 'extern' prototype
  std::unique_ptr<PrototypeAST> ParseExtern();

  /// toplevelexpr ::= expression
  std::unique_ptr<FunctionAST> ParseTopLevelExpr();
};