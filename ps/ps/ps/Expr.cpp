#include "Expr.h"

#include <variant>

namespace {
// helper type for the visitor #4
template <class... Ts>
struct overloaded : Ts... {
  using Ts::operator()...;
};
// explicit deduction guide (not needed as of C++20)
template <class... Ts>
overloaded(Ts...) -> overloaded<Ts...>;

}  // namespace

ps::ExprWithChildren::ExprWithChildren(std::vector<Expr> children) : mChildren(std::move(children)) {}

ps::ExprAssign::ExprAssign(ExprVariable lhs, Expr rhs) : ExprWithChildren({std::move(lhs), std::move(rhs)}) {}
ps::ExprVariable const& ps::ExprAssign::variable() const noexcept { return std::get<ps::ExprVariable>(mChildren[0]); }
ps::Expr const& ps::ExprAssign::rhs() const noexcept { return mChildren[1]; }

ps::ExprVariable::ExprVariable(std::string name) : mName(std::move(name)) {}
std::string const& ps::ExprVariable::name() const noexcept { return mName; }

ps::ExprGlobal::ExprGlobal(std::string name) : mName(std::move(name)) {}
std::string const& ps::ExprGlobal::name() const noexcept { return mName; }

ps::ExprAdd::ExprAdd(Expr lhs, Expr rhs) : ExprWithChildren({std::move(lhs), std::move(rhs)}) {}
ps::Expr const& ps::ExprAdd::lhs() const noexcept { return mChildren[0]; }
ps::Expr const& ps::ExprAdd::rhs() const noexcept { return mChildren[1]; }

ps::ExprSubtract::ExprSubtract(Expr lhs, Expr rhs) : ExprWithChildren({std::move(lhs), std::move(rhs)}) {}
ps::Expr const& ps::ExprSubtract::lhs() const noexcept { return mChildren[0]; }
ps::Expr const& ps::ExprSubtract::rhs() const noexcept { return mChildren[1]; }

ps::ExprDouble::ExprDouble(double value) : mValue(value) {}
double ps::ExprDouble::value() const noexcept { return mValue; }

std::string ps::getTypeName(Expr const& expr) {
  using namespace std::string_literals;
  return std::visit(overloaded{
                        [](auto const&) { return "Unknown expression"s; },
                        [](ExprAssign const&) { return "ExprAssign"s; },
                        [](ExprVariable const&) { return "ExprVariable"s; },
                        [](ExprGlobal const&) { return "ExprGlobal"s; },
                        [](ExprAdd const&) { return "ExprAdd"s; },
                        [](ExprSubtract const&) { return "ExprSubtract"s; },
                        [](ExprDouble const&) { return "ExprDouble"s; }
                    },
                    expr);
}
