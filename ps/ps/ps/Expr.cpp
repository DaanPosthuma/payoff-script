#include "Expr.h"

ExprWithChildren::ExprWithChildren(std::vector<Expr> children) : children(std::move(children)) {}
ExprAdd::ExprAdd(Expr lhs, Expr rhs) : ExprWithChildren({std::move(lhs), std::move(rhs)}) {}
ExprSubtract::ExprSubtract(Expr lhs, Expr rhs) : ExprWithChildren({std::move(lhs), std::move(rhs)}) {}
ExprDouble::ExprDouble(double value) : mValue(value) {}
ExprGlobal::ExprGlobal(std::string name) : mName(std::move(name)) {}

