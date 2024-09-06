#include <gtest/gtest.h>
#include <ps/Parser.h>
#include <ps/Scanner.h>

#include "testScripts.h"
//
#include <ranges>

namespace {

using namespace ps::scanner;
auto static const tPlus = Token(TokenType::Plus, "+");
auto static const tMinus = Token(TokenType::Minus, "-");

auto static const t0 = Token(TokenType::Double, "0.0");
auto static const t1 = Token(TokenType::Double, "1.0");
auto static const t2 = Token(TokenType::Double, "2.0");
auto static const t3 = Token(TokenType::Double, "3.0");
auto static const t4 = Token(TokenType::Double, "4.0");

auto static const id = Token(TokenType::Identifier, "test");
auto static const global = Token(TokenType::Global, "$global");

auto static const tEof = Token(TokenType::EndOfFile, "");

TEST(ps_parser, threePlusFour) {
  auto const expr = ps::parser::parse({t3, tPlus, t4, tEof});
  ASSERT_TRUE(std::holds_alternative<ps::ExprAdd>(expr));

  auto const& exprAdd = std::get<ps::ExprAdd>(expr);
  auto const& lhs = exprAdd.lhs();
  auto const& rhs = exprAdd.rhs();
  ASSERT_TRUE(std::holds_alternative<ps::ExprDouble>(lhs));
  ASSERT_TRUE(std::holds_alternative<ps::ExprDouble>(rhs));
  EXPECT_EQ(std::get<ps::ExprDouble>(lhs).value(), 3.0);
  EXPECT_EQ(std::get<ps::ExprDouble>(rhs).value(), 4.0);
}

}  // namespace
