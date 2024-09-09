#include "Scanner.h"

#include <cctype>
#include <iostream>
#include <optional>
#include <print>
#include <stdexcept>
#include <utility>
#include <vector>

namespace {

using namespace ps::scanner;

class Scanner {
 public:
  Scanner(std::string_view script) : script(script) {}

  [[nodiscard]] std::vector<Token> scanTokens() {
    std::vector<Token> tokens;
    while (!endOfFile()) {
      if (auto token = scanToken()) {
        tokens.push_back(*token);
      }
    }
    tokens.emplace_back(TokenType::EndOfFile, "", line);
    return tokens;
  }

 private:
  char advance() noexcept {
    return script[curr++];
  }

  [[nodiscard]] char peek() const noexcept {
    if (endOfFile()) return '\0';
    return script[curr];
  }

  [[nodiscard]] bool endOfFile() const noexcept {
    return curr >= script.size();
  }

  [[nodiscard]] Token scanNumber() {
    while (std::isdigit(peek())) {
      advance();
    }
    if (peek() == '.') {
      advance();
      while (std::isdigit(peek())) {
        advance();
      }
    }
    return emit(TokenType::Double);
  }

  [[nodiscard]] Token scanIdentifierOrGlobal(TokenType tokenType) {
    while (std::isalpha(peek()) || peek() == '_') {
      advance();
    }
    return emit(tokenType);
  }

  [[nodiscard]] Token scanIdentifier() {
    return scanIdentifierOrGlobal(TokenType::Identifier);
  }

  [[nodiscard]] Token scanGlobal() {
    return scanIdentifierOrGlobal(TokenType::Global);
  }

  [[nodiscard]] std::optional<Token> scanToken() {
    start = curr;
    auto const c = advance();
    switch (c) {
      case ' ':
        break;
      case '\r':
        break;
      case '\t':
        break;
      case '\n':
        ++line;
        return emit(TokenType::NewLine);
      case '-':
        return emit(TokenType::Minus);
      case '+':
        return emit(TokenType::Plus);
      case '*':
        return emit(TokenType::Star);
      case '/':
        return emit(TokenType::Slash);
      case ',':
        return emit(TokenType::Comma);
      case '(':
        return emit(TokenType::LeftParen);
      case ')':
        return emit(TokenType::RightParen);
      case '[':
        return emit(TokenType::LeftBracket);
      case ']':
        return emit(TokenType::RightBracket);
      case '=':
        return emit(TokenType::Equals);
      case '$':
        return scanGlobal();
      default:
        if (std::isdigit(c)) {
          return scanNumber();
        } else if (std::isalpha(c)) {
          return scanIdentifier();
        } else {
          throw std::runtime_error(std::format("Unknown character ({}) on line {}", c, line));
        }
    }

    return {};
  }

  auto emit(TokenType type) const {
    auto s = std::string(script.substr(start, curr - start));
    //std::println("emitting: {} ({})", toString(type), s);
    return Token(type, std::string(script.substr(start, curr - start)), line);
  }

  size_t start = 0;
  size_t curr = 0;
  size_t line = 0;
  std::string_view script;
};

}  // namespace

std::string ps::scanner::toString(TokenType tokenType) {
  switch (tokenType) {
    case TokenType::Identifier:
      return "Identifier";
    case TokenType::Global:
      return "Global";
    case TokenType::Plus:
      return "Plus";
    case TokenType::Minus:
      return "Minus";
    case TokenType::Star:
      return "Star";
    case TokenType::Slash:
      return "Slash";
    case TokenType::Comma:
      return "Comma";
    case TokenType::LeftParen:
      return "LeftParen";
    case TokenType::RightParen:
      return "RightParen";
    case TokenType::LeftBracket:
      return "LeftBracket";
    case TokenType::RightBracket:
      return "RightBracket";
    case TokenType::Equals:
      return "Equals";
    case TokenType::Double:
      return "Double";
    case TokenType::NewLine:
      return "NewLine";
    case TokenType::EndOfFile:
      return "EndOfFile";
    default:
      throw std::runtime_error(std::format("Unknown token type: {}", std::to_underlying(tokenType)));
  }
}

void ps::scanner::PrintTo(TokenType tokenType, std::ostream* os) {
  *os << toString(tokenType);
}

std::vector<Token> ps::scanner::scanTokens(std::string_view const script) {
  return Scanner(script).scanTokens();
}
