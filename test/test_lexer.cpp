#include "lexer.h"
#include "test_utils.h"
#include "token.h"

#include <gtest/gtest.h>

TEST(Lexer, GetTokNumber) {
  // Test the lexer with a numeric input
  TokenBuffer lexer(get_input_stream("42.0"));

  // Read tokens and check their values
  EXPECT_EQ(lexer.getTok(), Token::tok_number);
  EXPECT_EQ(lexer.getNumVal(), 42.0);

  EXPECT_EQ(lexer.getTok(), Token::tok_eof);
}

TEST(Lexer, GetTokIdentifier) {
  // Test the lexer with an identifier input
  TokenBuffer lexer(get_input_stream("foo"));
  // Read tokens and check their values
  auto tok = lexer.getTok();
  EXPECT_EQ(tok, Token::tok_identifier)
      << "Expected tok_identifier"
      << ", got " << getTokenEnumerationString(tok);
  EXPECT_EQ(lexer.getIdentifier(), "foo");
}

TEST(Lexer, GetTokDef) {
  // Test the lexer with a def input
  TokenBuffer lexer(get_input_stream("def"));

  // Read tokens and check their values
  auto tok = lexer.getTok();
  EXPECT_EQ(tok, Token::tok_def) << "Expected tok_def"
                                 << ", got " << getTokenEnumerationString(tok);
}

TEST(Lexer, GetTokExtern) {
  // Test the lexer with an extern input
  TokenBuffer lexer(get_input_stream("extern"));

  // Read tokens and check their values
  auto tok = lexer.getTok();
  // Read tokens and check their values
  EXPECT_EQ(tok, Token::tok_extern)
      << "Expected tok_extern"
      << ", got " << getTokenEnumerationString(tok);
}

TEST(Lexer, GetTokComment) {
  // Test the lexer with a comment input
  TokenBuffer lexer(get_input_stream("# This is a comment\n"));
  // Read tokens and check their values
  EXPECT_EQ(lexer.getTok(), Token::tok_eof);
}

TEST(Lexer, GetTokOperator) {
  // Test the lexer with an operator input
  TokenBuffer lexer(get_input_stream("+ - * /"));
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
  TokenBuffer lexer(get_input_stream("( )"));
  // Read tokens and check their values
  EXPECT_EQ(lexer.getTok(), Token::tok_parentheses);
  EXPECT_EQ(lexer.getParentheses(), '(');
  EXPECT_EQ(lexer.getTok(), Token::tok_parentheses);
  EXPECT_EQ(lexer.getParentheses(), ')');
}

TEST(Lexer, GetTokSemicolons) {
  // Test the lexer with an operator input
  TokenBuffer lexer(get_input_stream("; ,"));
  // Read tokens and check their values
  EXPECT_EQ(lexer.getTok(), Token::tok_semicolons);
  EXPECT_EQ(lexer.getSemicolons(), ';');
  EXPECT_EQ(lexer.getTok(), Token::tok_semicolons);
  EXPECT_EQ(lexer.getSemicolons(), ',');
}

TEST(Lexer, GetFullSentence) {
  // Test the lexer with an operator input
  TokenBuffer lexer(get_input_stream("def fib(x)"));
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
