#include "Expr.h"


ps::ExprWithChildren::ExprWithChildren(std::vector<Expr> children) : mChildren(std::move(children)) {}

ps::ExprAdd::ExprAdd(Expr lhs, Expr rhs) : ExprWithChildren({std::move(lhs), std::move(rhs)}) {}
ps::Expr const& ps::ExprAdd::lhs() const noexcept { return mChildren[0]; }
ps::Expr const& ps::ExprAdd::rhs() const noexcept { return mChildren[1]; }

ps::ExprSubtract::ExprSubtract(Expr lhs, Expr rhs) : ExprWithChildren({std::move(lhs), std::move(rhs)}) {}
ps::Expr const& ps::ExprSubtract::lhs() const noexcept { return mChildren[0]; }
ps::Expr const& ps::ExprSubtract::rhs() const noexcept { return mChildren[1]; }

ps::ExprDouble::ExprDouble(double value) : mValue(value) {}
double ps::ExprDouble::value() const noexcept { return mValue; }

ps::ExprGlobal::ExprGlobal(std::string name) : mName(std::move(name)) {}
