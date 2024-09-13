#pragma once

#include <string>

namespace ps::tests::scripts {

using namespace std::string_literals;

auto const vanillaCall = R"(base = ccy($base_ccy)
                            term = ccy($term_ccy)
                            notional = $notional_amt [base]
                            strike = $strike [term/base]
                            expiry = datetime($expiry_date)
                            s = spot(base,term)
                            cashflow(expiry, notional * max(s - strike)))"s;

}
