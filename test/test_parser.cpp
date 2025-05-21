// (c) Copyright 2022 - 2025 Advanced Micro Devices, Inc. All Rights Reserved.

#include "lexer.h"
#include "parser.h"
#include "test_utils.h"

#include <gtest/gtest.h>
#include <memory>

TEST(Parser, Function1) {
  std::string input = R"(def foo(x y) x+foo(y, 4.0);)";
  Parser(std::make_unique<TokenBuffer>(get_input_stream(input)));
}

TEST(Parser, Function2) {
  std::string input = R"(def foo(x y) x+y y;)";
  Parser(std::make_unique<TokenBuffer>(get_input_stream(input)));
}

TEST(Parser, Function3) {
  std::string input = R"(def foo(x y) x+y );)";
  Parser(std::make_unique<TokenBuffer>(get_input_stream(input)));
}

TEST(Parser, Function4) {
  std::string input = R"(extern sin(a);)";
  Parser(std::make_unique<TokenBuffer>(get_input_stream(input)));
}