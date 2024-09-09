#pragma once


#include "Expr.h"
//
#include <vector>

namespace ps::scanner {
class Token;
}

namespace ps::parser {

std::vector<ps::Expr> parse(std::vector<ps::scanner::Token> const& tokens);

}
