#pragma once

#include <vector>
#include <variant>
#include <string>

class ExprAdd;
class ExprSubtract;
class ExprDouble;
class ExprGlobal;

using Expr = std::variant<ExprAdd, ExprSubtract, ExprDouble, ExprGlobal>;

class ExprWithChildren {
 public:
  ExprWithChildren(std::vector<Expr> children);
 private:
  std::vector<Expr> children;
};

class ExprAdd : public ExprWithChildren {
  ExprAdd(Expr lhs, Expr rhs);
};

class ExprSubtract : public ExprWithChildren {
  ExprSubtract(Expr lhs, Expr rhs);
};

class ExprDouble {
public:
  ExprDouble(double value);

private:
  double mValue;
};

class ExprGlobal {
public:
  ExprGlobal(std::string name);

private:
  std::string mName;
};
