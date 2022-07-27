#include <gtest/gtest.h>
#include "pcs/operation/parsers/parameterized_op.h"

#include <string>
#include <memory>
#include <vector>

#include "lts/parsers/transition.h"
#include "pcs/operation/parameter.h"

TEST(ParameterizedOpParser, ParametersVec) {
	std::string label = "speed=50 rotation=90 magnitude= force=5 direction=";
	std::vector<pcs::Parameter> expected({ { "speed", "50" }, { "rotation", "90" }, { "magnitude", "" }, {"force", "5"}, {"direction", ""} });
	std::vector<pcs::Parameter> got = pcs::StringToParameters(label);
	ASSERT_EQ(got, expected);
}

TEST(ParameterizedOpParser, Transition) {
	std::string label = "drill()";
	pcs::ParameterizedOp p_op = nightly::ParseTransitionString<pcs::ParameterizedOp>(label);
	ASSERT_EQ(p_op.operation().name(), "drill");
}

TEST(ParameterizedOpParser, TransitionWithParams) {
	std::string label = "drill(speed=50 rotation=90 magnitude= direction=west)";
	pcs::ParameterizedOp p_op = nightly::ParseTransitionString<pcs::ParameterizedOp>(label);
	ASSERT_EQ(p_op.operation().name(), "drill");
	ASSERT_EQ(p_op.parameters(), std::vector<pcs::Parameter>({ {"speed", "50"}, {"rotation", "90"}, {"magnitude", ""}, {"direction", "west"} }));
}