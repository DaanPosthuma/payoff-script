#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <ps/StaticAnalysis.h>

#include "testScripts.h"
//
#include <print>

namespace {

TEST(ps_static_analysis, resolveTypesDouble) {
  auto const expr = ps::ExprAssign(ps::ExprVariable("test"), ps::ExprDouble(10.0));
  auto const variableTypeMap = ps::static_analysis::resolveTypes({expr});

  EXPECT_THAT(variableTypeMap, ::testing::Contains(std::pair{"test", ps::static_analysis::Type::Double}));
}

TEST(ps_static_analysis, resolveTypesSumOfDoubles) {
  auto const expr = ps::ExprAssign(ps::ExprVariable("test"), ps::ExprAdd(ps::ExprDouble(10.0), ps::ExprDouble(20.0)));
  auto const variableTypeMap = ps::static_analysis::resolveTypes({expr});

  EXPECT_THAT(variableTypeMap, ::testing::Contains(std::pair{"test", ps::static_analysis::Type::Double}));
}

TEST(ps_static_analysis, resolveTypesDate) {
  auto const assign = ps::ExprAssign(ps::ExprVariable("expiry"), ps::ExprGlobal("$EXPIRY"));
  auto const use = ps::ExprFunctionCall("FunctionThatExpectsADate", {ps::ExprVariable("expiry")});
  auto const variableTypeMap = ps::static_analysis::resolveTypes({assign, use});

  EXPECT_THAT(variableTypeMap, ::testing::Contains(std::pair{"expiry", ps::static_analysis::Type::Date}));
}

}  // namespace
