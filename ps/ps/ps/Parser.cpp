#include "Parser.h"

#include <expected>
#include <format>
#include <optional>
#include <span>
#include <stdexcept>
#include <print>

#include "Scanner.h"

namespace {

using namespace ps;
using namespace ps::scanner;

class Parser {
 public:
  struct ParseError {
    Token token;
    std::string message;
  };

  explicit Parser(std::span<const ps::scanner::Token> tokens) noexcept : mTokens(tokens) {}

  [[nodiscard]] Expr parseOne() {
    return parseAssignment();
  }

  [[nodiscard]] std::vector<Expr> parse() {
    auto ret = std::vector<Expr>{};

    while (!isAtEnd()) {
      ret.push_back(parseOne());
    }
    return ret;
  }

 private:
  Expr parseAssignment() {
    auto expr = parseBinary();
    
    if (std::holds_alternative<ExprVariable>(expr) && match<ps::scanner::TokenType::Equals>()) {
      auto rhs = parseBinary();
      return ExprAssign(std::get<ExprVariable>(std::move(expr)), std::move(rhs));
    }

    return expr;
  }

  Expr parseBinary() {
    auto expr = parseLeaf();

    while (match<ps::scanner::TokenType::Minus, ps::scanner::TokenType::Plus>()) {
        auto const operatr = previous();
        auto const right = parseLeaf();
        if (operatr.literal() == "+") return ExprAdd(std::move(expr), std::move(right));
        else return ExprSubtract(std::move(expr), std::move(right));
    }

    return expr;
  }

  Expr parseLeaf() {
    
    if (match<ps::scanner::TokenType::Double>()) {
      auto token = previous();
      return ExprDouble(std::stoi(token.literal()));
    } else if (match<ps::scanner::TokenType::Global>()) {
      auto token = previous();
      return ExprGlobal(token.literal());
    } else if (match<ps::scanner::TokenType::Identifier>()) {
      auto token = previous();
      return ExprVariable(token.literal());
    }

    throw ParseError(peek(), "Expect leaf.");
  }

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
  [[nodiscard]] bool match() noexcept {
    if (check<T>()) {
      advance();
      return true;
    }
    return false;
  }

  template <TokenType First, TokenType Second, TokenType... Rest>
  [[nodiscard]] bool match() noexcept {
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

std::vector<ps::Expr> ps::parser::parse(std::vector<ps::scanner::Token> const& tokens) {
  try {
    return Parser(std::span(tokens)).parse();
  } catch (Parser::ParseError const& err) {
    auto const& message = err.message;
    auto const& token = err.token;
    throw std::runtime_error(std::format("Parser error. Message: '{}'. Token ({}): '{}'. Line: {}", message, toString(token.type()), token.literal(), token.line()));
  }
}
