#pragma once

#include <string>
#include <variant>
#include <vector>

namespace ps {

// statements:
class ExprAssign;
// expressions:
class ExprVariable;
class ExprGlobal;
class ExprAdd;
class ExprSubtract;
class ExprDouble;
class ExprIdentifier;
class ExprFunctionCall;

using Expr = std::variant<ExprAssign, ExprVariable, ExprGlobal, ExprAdd, ExprSubtract, ExprDouble, ExprIdentifier, ExprFunctionCall>;

class ExprWithChildren {
 public:
  ExprWithChildren(std::vector<Expr> children = {});

 protected:
  std::vector<Expr> mChildren;
};

class ExprAssign : private ExprWithChildren {
 public:
  ExprAssign(ExprVariable lhs, Expr rhs);
  [[nodiscard]] ExprVariable const& variable() const noexcept;
  [[nodiscard]] Expr const& rhs() const noexcept;
};

class ExprVariable {
 public:
  ExprVariable(std::string name);
  [[nodiscard]] std::string const& name() const noexcept;

 private:
  std::string mName;
};

class ExprGlobal {
 public:
  ExprGlobal(std::string name);
  [[nodiscard]] std::string const& name() const noexcept;

 private:
  std::string mName;
};

class ExprAdd : private ExprWithChildren {
 public:
  ExprAdd(Expr lhs, Expr rhs);
  [[nodiscard]] Expr const& lhs() const noexcept;
  [[nodiscard]] Expr const& rhs() const noexcept;
};

class ExprSubtract : private ExprWithChildren {
 public:
  ExprSubtract(Expr lhs, Expr rhs);
  [[nodiscard]] Expr const& lhs() const noexcept;
  [[nodiscard]] Expr const& rhs() const noexcept;
};

class ExprDouble {
 public:
  ExprDouble(double value);
  [[nodiscard]] double value() const noexcept;

 private:
  double mValue;
};

class ExprIdentifier {
 public:
  ExprIdentifier(std::string identifier);
  [[nodiscard]] std::string const& identifier() const noexcept;

 private:
  std::string mIdentifier;
};

class ExprFunctionCall : private ExprWithChildren {
 public:
  ExprFunctionCall(std::string identifier, std::vector<ps::Expr> arguments);

};

std::string getTypeName(Expr const& expr);

}  // namespace ps
