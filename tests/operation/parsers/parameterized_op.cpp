#include <gtest/gtest.h>
#include "pcs/operation/parsers/parameterized_op.h"

#include <string>
#include <memory>
#include <vector>

#include "lts/parsers/transition.h"
#include "pcs/operation/parameters.h"

TEST(ParameterizedOpParser, ParametersVec) {
	std::string label = "speed=50 rotation=90 magnitude= force=5 direction=";
	pcs::Parameters expected({ { "speed", "50" }, { "rotation", "90" }, { "magnitude", "" }, {"force", "5"}, {"direction", ""} });
	pcs::Parameters got = pcs::StringToParameters(label);
	ASSERT_EQ(got, expected);
}

TEST(ParameterizedOpParser, Transition) {
	std::string label = "drill()";
	pcs::ParameterizedOp p_op = nightly::ParseTransitionString<pcs::ParameterizedOp>(label);
	ASSERT_EQ(p_op.operation(), "drill");
}

TEST(ParameterizedOpParser, TransitionWithParams) {
	std::string label = "drill(speed=50 rotation=90 magnitude= direction=west)";
	pcs::ParameterizedOp p_op = nightly::ParseTransitionString<pcs::ParameterizedOp>(label);
	ASSERT_EQ(p_op.operation(), "drill");
	ASSERT_EQ(p_op.parameters(), pcs::Parameters({ {"speed", "50"}, {"rotation", "90"}, {"magnitude", ""}, {"direction", "west"} }));
}