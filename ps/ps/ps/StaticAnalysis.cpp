#include "StaticAnalysis.h"

#include <stdexcept>
#include <format>
#include <ostream>
#include <utility>

std::string ps::static_analysis::toString(Type type) {
  switch (type) {
    case Type::Double:
      return "Double";
    case Type::Date:
      return "Date";
    default:
      throw std::runtime_error(std::format("Unknown token type: {}", std::to_underlying(type)));
  }
}

void ps::static_analysis::PrintTo(Type type, std::ostream* os) {
    *os << toString(type);
}

std::unordered_map<std::string, ps::static_analysis::Type> ps::static_analysis::resolveTypes(std::vector<ps::Expr> const& exprs) {
    auto variableTypeMap = std::unordered_map<std::string, ps::static_analysis::Type>{};

    // TODO: actual implementation

    // mock implementation:
    variableTypeMap["test"] = Type::Double;
    variableTypeMap["expiry"] = Type::Date;

    return variableTypeMap;
}
