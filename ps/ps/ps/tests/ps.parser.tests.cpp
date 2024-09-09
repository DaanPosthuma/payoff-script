#include <gtest/gtest.h>
#include <ps/Parser.h>
#include <ps/Scanner.h>

#include "testScripts.h"
//
#include <expected>
#include <print>
#include <ranges>


#define ASSERT_HOLDS_ALTERNATIVE(var, ExpectedT)       \
  ASSERT_TRUE(std::holds_alternative<ExpectedT>(var)) \
      << "Variant '" << #var << "' does not have expected contents. Expected " << typeid(ExpectedT).name() << ", actual: " << getTypeName(var)

#define ASSERT_SIZE_EQ(rng, expectedSize) \
  ASSERT_EQ(size(rng), expectedSize)      \
      << "Container '" << #rng << "' has incorrect size. Expected: " << expectedSize << ", actual: " << size(rng)

namespace {

using namespace ps::scanner;
auto static const tPlus = Token(TokenType::Plus, "+", 0);
auto static const tMinus = Token(TokenType::Minus, "-", 0);
auto static const tEquals = Token(TokenType::Equals, "=", 0);

auto static const t0 = Token(TokenType::Double, "0.0", 0);
auto static const t1 = Token(TokenType::Double, "1.0", 0);
auto static const t2 = Token(TokenType::Double, "2.0", 0);
auto static const t3 = Token(TokenType::Double, "3.0", 0);
auto static const t4 = Token(TokenType::Double, "4.0", 0);

auto static const tIdent = Token(TokenType::Identifier, "test", 0);
auto static const tGlobal = Token(TokenType::Global, "$global", 0);

auto static const tEof = Token(TokenType::EndOfFile, "", 0);

TEST(ps_parser, doubleTokenProducesExprDouble) {
  auto const exprs = ps::parser::parse({t1, tEof});
  ASSERT_SIZE_EQ(exprs, 1);
  auto const& expr = exprs[0];
  ASSERT_HOLDS_ALTERNATIVE(expr, ps::ExprDouble);
  auto const& exprDouble = std::get<ps::ExprDouble>(expr);
  EXPECT_EQ(exprDouble.value(), 1.0);
}

TEST(ps_parser, identifierTokenProducesExprVariable) {
  auto const exprs = ps::parser::parse({tIdent, tEof});
  ASSERT_SIZE_EQ(exprs, 1);
  auto const& expr = exprs[0];
  ASSERT_HOLDS_ALTERNATIVE(expr, ps::ExprVariable);
  auto const& exprVariable = std::get<ps::ExprVariable>(expr);
  EXPECT_EQ(exprVariable.name(), tIdent.literal());
}

TEST(ps_parser, globalTokenProducesExprGlobal) {
  auto const exprs = ps::parser::parse({tGlobal, tEof});
  ASSERT_SIZE_EQ(exprs, 1);
  auto const& expr = exprs[0];
  ASSERT_HOLDS_ALTERNATIVE(expr, ps::ExprGlobal);
  auto const& exprGlobal = std::get<ps::ExprGlobal>(expr);
  EXPECT_EQ(exprGlobal.name(), tGlobal.literal());
}

TEST(ps_parser, plusTokenProducesExprAdd) {
  auto const exprs = ps::parser::parse({t3, tPlus, t4, tEof});
  ASSERT_SIZE_EQ(exprs, 1);
  auto const& expr = exprs[0];
  ASSERT_HOLDS_ALTERNATIVE(expr, ps::ExprAdd);
  auto const& exprAdd = std::get<ps::ExprAdd>(expr);
  auto const& lhs = exprAdd.lhs();
  auto const& rhs = exprAdd.rhs();
  ASSERT_HOLDS_ALTERNATIVE(lhs, ps::ExprDouble);
  ASSERT_HOLDS_ALTERNATIVE(rhs, ps::ExprDouble);
  EXPECT_EQ(std::get<ps::ExprDouble>(lhs).value(), 3.0);
  EXPECT_EQ(std::get<ps::ExprDouble>(rhs).value(), 4.0);
}

TEST(ps_parser, equalsTokenProducesExprAssign) {
  auto const exprs = ps::parser::parse({tIdent, tEquals, t4, tEof});
  ASSERT_SIZE_EQ(exprs, 1);
  auto const& expr = exprs[0];
  ASSERT_HOLDS_ALTERNATIVE(expr, ps::ExprAssign);
  auto const& exprAssign = std::get<ps::ExprAssign>(expr);
  auto const& variable = exprAssign.variable();
  EXPECT_EQ(variable.name(), tIdent.literal());
  auto const& rhs = exprAssign.rhs();
  ASSERT_HOLDS_ALTERNATIVE(rhs, ps::ExprDouble);
  EXPECT_EQ(std::get<ps::ExprDouble>(rhs).value(), 4.0);
}

}  // namespace
