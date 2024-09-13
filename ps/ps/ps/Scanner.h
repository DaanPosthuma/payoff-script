#pragma once

#include <string>
#include <vector>
#include <iosfwd>

namespace ps::scanner {

enum class TokenType {
  Identifier,
  Global,
  Plus,
  Minus,
  Star,
  Slash,
  Comma,
  LeftParen,
  RightParen,
  LeftBracket,
  RightBracket,
  Equals,
  Double,
  NewLine,
  EndOfFile
};

[[nodiscard]] std::string toString(TokenType tokenType);
void PrintTo(TokenType tokenType, std::ostream* os);

class Token {
 public:
  Token(TokenType tokenType, std::string literal, int line) noexcept : mTokenType(tokenType), mLiteral(std::move(literal)), mLine(line) {}
  [[nodiscard]] auto type() const noexcept { return mTokenType; }
  [[nodiscard]] auto const& literal() const noexcept { return mLiteral; }
  [[nodiscard]] auto line() const noexcept { return mLine; }

 private:
  TokenType mTokenType;
  std::string mLiteral;
  int mLine;
};

std::vector<Token> scanTokens(std::string_view const script);

}  // namespace ps::scanner
