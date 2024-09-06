#include "testScripts.h"
#include <gtest/gtest.h>
#include <ps/Scanner.h>
//
#include <ranges>

namespace {

using namespace std::string_literals;

auto types(auto const& rng) {
  return rng | std::views::transform([](auto const& token) { return token.type(); }) | std::ranges::to<std::vector>();
}

TEST(ps_scanner, tokenizeSpaces) {
  auto const tokens = ps::scanner::scanTokens("   ");
  ASSERT_EQ(types(tokens), (std::vector{ps::scanner::TokenType::EndOfFile}));
}

TEST(ps_scanner, tokenizeNumber) {
  auto const tokens = ps::scanner::scanTokens("123.456");
  ASSERT_EQ(types(tokens), (std::vector{ps::scanner::TokenType::Double, ps::scanner::TokenType::EndOfFile}));
}

TEST(ps_scanner, tokenizeIdentifier) {
  auto const tokens = ps::scanner::scanTokens("test");
  ASSERT_EQ(types(tokens), (std::vector{ps::scanner::TokenType::Identifier, ps::scanner::TokenType::EndOfFile}));
}

TEST(ps_scanner, tokenizeGlobal) {
  auto const tokens = ps::scanner::scanTokens("$test");
  ASSERT_EQ(types(tokens), (std::vector{ps::scanner::TokenType::Global, ps::scanner::TokenType::EndOfFile}));
}

TEST(ps_scanner, tokenizeVanillaCall) {
  auto const tokens = ps::scanner::scanTokens(scripts::vanillaCall);

  auto const isGlobal = [](auto const& token) { return token.type() == ps::scanner::TokenType::Global; };
  auto const isIdentifier = [](auto const& token) { return token.type() == ps::scanner::TokenType::Identifier; };
  auto const getLiteral = &ps::scanner::Token::literal;

  auto const globals = tokens | std::views::filter(isGlobal) | std::views::transform(getLiteral) | std::ranges::to<std::vector>();
  auto const identifiers = tokens | std::views::filter(isIdentifier) | std::views::transform(getLiteral) | std::ranges::to<std::vector>();

  ASSERT_EQ(globals, (std::vector<std::string>{"$base_ccy", "$term_ccy", "$notional_amt", "$strike", "$expiry_date"}));
  ASSERT_EQ(identifiers, (std::vector<std::string>{"base", "ccy", "term", "ccy", "notional", "base", "strike", "term", "base", 
    "expiry", "datetime", "s", "spot", "base", "term", "cashflow", "expiry", "notional", "max", "s", "strike"}));
}

}  // namespace
