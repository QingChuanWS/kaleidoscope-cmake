#include "token.h"
#include <bits/types/FILE.h>
#include <gtest/gtest.h>
#include <lexer.h>

FILE *get_input_stream(const char *input) {
  // Use fmemopen to create a stream from the input string
  FILE *stream = fmemopen((void *)input, strlen(input), "r");
  if (!stream) {
    perror("fmemopen failed");
    exit(1);
  }
  return stream;
}

TEST(Lexer, GetTokNumber) {
  // Test the lexer with a numeric input
  stdin = get_input_stream("42.0");
  TokenBuffer lexer;

  // Read tokens and check their values
  EXPECT_EQ(lexer.getTok(), Token::tok_number);
  EXPECT_EQ(lexer.getNumVal(), 42.0);

  EXPECT_EQ(lexer.getTok(), Token::tok_eof);
}

TEST(Lexer, GetTokIdentifier) {
  // Test the lexer with an identifier input
  stdin = get_input_stream("foo");
  TokenBuffer lexer;
  // Read tokens and check their values
  auto tok = lexer.getTok();
  EXPECT_EQ(tok, Token::tok_identifier)
      << "Expected tok_identifier"
      << ", got " << getTokenEnumerationString(tok);
  EXPECT_EQ(lexer.getIdentifier(), "foo");
}

TEST(Lexer, GetTokDef) {
  // Test the lexer with a def input
  stdin = get_input_stream("def");
  TokenBuffer lexer;

  // Read tokens and check their values
  auto tok = lexer.getTok();
  EXPECT_EQ(tok, Token::tok_def) << "Expected tok_def"
                                 << ", got " << getTokenEnumerationString(tok);
}

TEST(Lexer, GetTokExtern) {
  // Test the lexer with an extern input
  stdin = get_input_stream("extern");
  TokenBuffer lexer;

  // Read tokens and check their values
  auto tok = lexer.getTok();
  // Read tokens and check their values
  EXPECT_EQ(tok, Token::tok_extern)
      << "Expected tok_extern"
      << ", got " << getTokenEnumerationString(tok);
}

TEST(Lexer, GetTokComment) {
  // Test the lexer with a comment input
  stdin = get_input_stream("# This is a comment\n");
  TokenBuffer lexer;
  // Read tokens and check their values
  EXPECT_EQ(lexer.getTok(), Token::tok_eof);
}

TEST(Lexer, GetTokOperator) {
  // Test the lexer with an operator input
  stdin = get_input_stream("+ - * /");

  TokenBuffer lexer;
  // Read tokens and check their values
  EXPECT_EQ(lexer.getTok(), Token::tok_operator);
  EXPECT_EQ(lexer.getOperator(), '+');
  EXPECT_EQ(lexer.getTok(), Token::tok_operator);
  EXPECT_EQ(lexer.getOperator(), '-');
  EXPECT_EQ(lexer.getTok(), Token::tok_operator);
  EXPECT_EQ(lexer.getOperator(), '*');
  EXPECT_EQ(lexer.getTok(), Token::tok_operator);
  EXPECT_EQ(lexer.getOperator(), '/');
  EXPECT_EQ(lexer.getTok(), Token::tok_eof);
}

TEST(Lexer, GetTokParentheses) {
  // Test the lexer with an operator input
  stdin = get_input_stream("( )");

  TokenBuffer lexer;
  // Read tokens and check their values
  EXPECT_EQ(lexer.getTok(), Token::tok_parentheses);
  EXPECT_EQ(lexer.getParentheses(), '(');
  EXPECT_EQ(lexer.getTok(), Token::tok_parentheses);
  EXPECT_EQ(lexer.getParentheses(), ')');
}

TEST(Lexer, GetFullSentence) {
  // Test the lexer with an operator input
  stdin = get_input_stream("def fib(x)");

  TokenBuffer lexer;
  // Read tokens and check their values
  EXPECT_EQ(lexer.getTok(), Token::tok_def);
  EXPECT_EQ(lexer.getTok(), Token::tok_identifier);
  EXPECT_EQ(lexer.getIdentifier(), "fib");
  EXPECT_EQ(lexer.getTok(), Token::tok_parentheses);
  EXPECT_EQ(lexer.getParentheses(), '(');
  EXPECT_EQ(lexer.getTok(), tok_identifier);
  EXPECT_EQ(lexer.getIdentifier(), "x");
  EXPECT_EQ(lexer.getTok(), Token::tok_parentheses);
  EXPECT_EQ(lexer.getParentheses(), ')');
}
