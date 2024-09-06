#include "Parser.h"

#include <span>

#include "Scanner.h"

namespace {

using namespace ps;
using namespace ps::scanner;

class Parser {
 private:
  struct ParseError {
    Token token;
    std::string message;
  };

 public:
  explicit Parser(std::span<const ps::scanner::Token> tokens) noexcept : mTokens(tokens) {}

  [[nodiscard]] Expr parse() {
    return ExprAdd(ExprDouble(9.0), ExprDouble(7.0));
  }

 private:
  Token const& advance() noexcept;
  [[nodiscard]] bool isAtEnd() const noexcept;
  [[nodiscard]] Token const& peek() const noexcept;
  [[nodiscard]] Token const& previous() const noexcept;

  template <TokenType T>
  [[nodiscard]] bool check() const noexcept {
    if (isAtEnd()) return false;
    return peek().type() == T;
  }

  template <TokenType T>
  [[nodiscard]] bool match() const noexcept {
    if (check<T>()) {
      advance();
      return true;
    }
    return false;
  }

  template <TokenType First, TokenType Second, TokenType... Rest>
  [[nodiscard]] bool match() const noexcept {
    return match<First>() || match<Second, Rest...>();
  }

  template <TokenType T>
  [[nodiscard]] Token const& consume(std::string const& message) {
    if (check<T>()) return advance();
    throw ParseError(peek(), message);
  }

  std::span<const ps::scanner::Token> mTokens;
  size_t mCurrent = 0;
};

Token const& Parser::advance() noexcept {
  if (!isAtEnd()) mCurrent++;
  return previous();
}

bool Parser::isAtEnd() const noexcept {
  return peek().type() == TokenType::EndOfFile;
}

Token const& Parser::peek() const noexcept {
  return mTokens[mCurrent];
}

Token const& Parser::previous() const noexcept {
  return mTokens[mCurrent - 1];
}

}  // namespace

ps::Expr ps::parser::parse(std::vector<ps::scanner::Token> const& tokens) {
  return Parser(std::span(tokens)).parse();
}
