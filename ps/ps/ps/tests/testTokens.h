#pragma once

#include <ps/Scanner.h>

namespace ps::tests::tokens {
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

}  // namespace ps::tests::tokens
