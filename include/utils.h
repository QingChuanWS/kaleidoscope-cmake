// (c) Copyright 2022 - 2025 Advanced Micro Devices, Inc. All Rights Reserved.
#include "ast.h"
#include <iostream>
#include <llvm/IR/Value.h>

// 定义日志级别
enum class LogLevel { NONE, ERROR, INFO, DEBUG };

// 默认日志级别可通过编译选项覆盖
#ifndef LOG_LEVEL
#define LOG_LEVEL LogLevel::DEBUG
#endif

// 日志宏定义
#define LOG(level, message)                                                    \
  do {                                                                         \
    if (level <= LOG_LEVEL) {                                                  \
      fprintf(stderr, "[%s] %s:%d - %s\n", #level, __FILE__, __LINE__,         \
              message);                                                        \
    }                                                                          \
  } while (0)

#define LOG_DEBUG(message) LOG(LogLevel::DEBUG, message)
#define LOG_INFO(message) LOG(LogLevel::INFO, message)
#define LOG_ERROR(message) LOG(LogLevel::ERROR, message)

#define CLI(level, message)                                                    \
  do {                                                                         \
    if (level <= LOG_LEVEL) {                                                  \
      fprintf(stderr, message);                                                \
    }                                                                          \
  } while (0)

#define CLI_DEBUG(message) CLI(LogLevel::DEBUG, message)
#define CLI_INFO(message) CLI(LogLevel::INFO, message)
#define CLI_ERROR(message) CLI(LogLevel::ERROR, message)

// ===== AST/IR 打印系统 =====
enum class PrintMode { NONE = 0, AST = 1 << 0, IR = 1 << 1, BOTH = AST | IR };

inline PrintMode operator|(PrintMode a, PrintMode b) {
  return static_cast<PrintMode>(static_cast<int>(a) | static_cast<int>(b));
}

inline bool operator&(PrintMode a, PrintMode b) {
  return (static_cast<int>(a) & static_cast<int>(b)) != 0;
}

#ifndef PRINT_MODE
#define PRINT_MODE PrintMode::IR
#endif

#define SHOULD_PRINT(mode) (PRINT_MODE & mode)

// 打印AST和IR的辅助宏
#define PRINT_AST(node)                                                        \
  do {                                                                         \
    if (LogLevel::DEBUG <= LOG_LEVEL && SHOULD_PRINT(PrintMode::AST)) {        \
      fprintf(stderr, "AST Tree: \n");                                         \
      print(node);                                                             \
    }                                                                          \
  } while (0)

#define PRINT_IR(ir)                                                           \
  do {                                                                         \
    if (LogLevel::DEBUG <= LOG_LEVEL && SHOULD_PRINT(PrintMode::IR)) {         \
      fprintf(stderr, "print IR: \n");                                         \
      ir->print(llvm::errs());                                                 \
    }                                                                          \
  } while (0)

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
