#pragma once

#include <ps/Expr.h>

#include <unordered_map>
#include <vector>
#include <iosfwd>

namespace ps::static_analysis {

enum class Type {
  Double,
  Date
};

[[nodiscard]] std::string toString(Type type);
void PrintTo(Type type, std::ostream* os);

std::unordered_map<std::string, Type> resolveTypes(std::vector<ps::Expr> const& exprs);

}  // namespace ps::static_analysis
