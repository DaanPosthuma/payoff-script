#include "StaticAnalysis.h"

#include <ps/exception.h>

#include <algorithm>
#include <expected>
#include <format>
#include <functional>
#include <ostream>
#include <print>
#include <unordered_map>
#include <utility>
#include <variant>
#include <vector>

namespace {

template <class>
struct dependent_false : std::false_type {};

using namespace ps::static_analysis;

struct TypeCollector {
 public:
  explicit TypeCollector(std::vector<ps::Expr> const& exprs) noexcept {
    for (auto const& expr : exprs) {
      auto rt = getReturnType(expr);
      if (!rt) {
        std::println("Error: {}", rt.error());
      }
    }
  }

  [[nodiscard]] std::unordered_map<std::string, Type> variableTypes() const noexcept { return mVariableTypes; }

 private:
  using XType = std::expected<Type, std::string>;
  XType getReturnType(ps::Expr const& expr) {
    return std::visit([this](auto const& concrete) { return getReturnTypeOfConcreteExpr(concrete); }, expr);
  }

  template <class T>
  XType getReturnTypeOfConcreteExpr(T const& expr) noexcept {
    static_assert(dependent_false<T>::value, "Unsupported type. (Did you add a new type to Expr and forget to add it to TypeCollector?)");
  }

  XType getReturnTypeOfConcreteExpr(ps::ExprAssign const& expr) noexcept {
    auto const& name = expr.variable().name();
    auto const xrhs = getReturnType(expr.rhs());

    return xrhs.and_then([&](Type rhs) -> XType {
      if (mVariableTypes.contains(name) && mVariableTypes[name] != rhs)
        return std::unexpected(std::format("Cannot assign type '{}' to variable '{}' with type '{}'!",
                                           toString(rhs), name, toString(mVariableTypes[name])));

      return mVariableTypes[name] = rhs;
    });
  }

  XType getReturnTypeOfConcreteExpr(ps::ExprVariable const& expr) noexcept {
    return Type::Unknown;
  }

  XType getReturnTypeOfConcreteExpr(ps::ExprGlobal const& expr) noexcept {
    return Type::Unknown;
  }

  XType getReturnTypeOfConcreteExpr(ps::ExprAdd const& expr) noexcept {
    auto const xlhs = getReturnType(expr.lhs());
    auto const xrhs = getReturnType(expr.rhs());
    return xlhs.and_then([&xrhs](Type lhs) -> XType {
      return xrhs.and_then([lhs](Type rhs) -> XType {
        if (lhs == rhs) return lhs;
        if (lhs == Type::Unknown) return rhs;
        if (rhs == Type::Unknown) return lhs;
        return std::unexpected(std::format("ExprAdd lhs and rhs types not equal. lhs: {}, rhs: {}", toString(lhs), toString(rhs)));
      });
    });
  }

  XType getReturnTypeOfConcreteExpr(ps::ExprSubtract const& expr) noexcept {
    auto const xlhs = getReturnType(expr.lhs());
    auto const xrhs = getReturnType(expr.rhs());
    return xlhs.and_then([&xrhs](Type lhs) -> XType {
      return xrhs.and_then([lhs](Type rhs) -> XType {
        if (lhs == rhs) return lhs;
        if (lhs == Type::Unknown) return rhs;
        if (rhs == Type::Unknown) return lhs;
        return std::unexpected(std::format("ExprSubtract lhs and rhs types not equal. lhs: {}, rhs: {}", toString(lhs), toString(rhs)));
      });
    });
  }

  XType getReturnTypeOfConcreteExpr(ps::ExprDouble const& expr) noexcept {
    return Type::Double;
  }

  XType getReturnTypeOfConcreteExpr(ps::ExprIdentifier const& expr) noexcept {
    return Type::Unknown;
  }

  XType getReturnTypeOfConcreteExpr(ps::ExprFunctionCall const& expr) noexcept {
    for (auto const& arg : expr.arguments()) {
      getReturnType(arg);
    }
    return Type::Unknown;  // todo
  }

 private:
  std::unordered_map<std::string, Type> mVariableTypes;
  // std::vector<std::string> mErrors;
};

}  // namespace

std::string ps::static_analysis::toString(Type type) {
  switch (type) {
    case Type::Double:
      return "Double";
    case Type::Date:
      return "Date";
    case Type::Nothing:
      return "Nothing";
    case Type::Unknown:
      return "Unknown";
    case Type::Error:
      return "Error";
    default:
      throw ps::exception("Unknown token type: {}", std::to_underlying(type));
  }
}

void ps::static_analysis::PrintTo(Type type, std::ostream* os) {
  *os << toString(type);
}

std::unordered_map<std::string, ps::static_analysis::Type> ps::static_analysis::resolveTypes(std::vector<ps::Expr> const& exprs) {
  auto variableTypeMap = std::unordered_map<std::string, ps::static_analysis::Type>{};

  // mock implementation:
  // variableTypeMap["test"] = Type::Double;
  // variableTypeMap["expiry"] = Type::Date;
  // return variableTypeMap;

  auto const tc = TypeCollector(exprs);
  return tc.variableTypes();
}
