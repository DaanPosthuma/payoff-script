#pragma once

#include <string>
#include <variant>
#include <vector>

namespace ps {

class ExprAdd;
class ExprSubtract;
class ExprDouble;
class ExprGlobal;

using Expr = std::variant<ExprAdd, ExprSubtract, ExprDouble, ExprGlobal>;

class ExprWithChildren {
 public:
  ExprWithChildren(std::vector<Expr> children);

 protected:
  std::vector<Expr> mChildren;
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

class ExprGlobal {
 public:
  ExprGlobal(std::string name);

 private:
  std::string mName;
};

}  // namespace ps
