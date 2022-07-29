#include "pcs/controller/unify.h"
#include <gtest/gtest.h>

#include <vector>

#include "pcs/operation/observable.h"
#include "pcs/operation/parameter.h"
#include "pcs/operation/parameterized_op.h"

TEST(Unify, Match) {
	std::vector<pcs::Parameter> resource_params({ {"speed", "50"}, { "rotation", "90"} });
	std::vector<pcs::Parameter> recipe_params({ { "speed", "50" }, { "rotation", "90" } });
	pcs::Observable op("engrave");
	EXPECT_EQ(pcs::Unify(resource_params, recipe_params, op), true);
}

TEST(Unify, MatchWithConstant) {
	std::vector<pcs::Parameter> resource_params({ {"speed", "50"}, { "rotation", "90"}, {"magnitude", ""} });
	std::vector<pcs::Parameter> recipe_params({ { "speed", "50" }, { "rotation", "90" }, {"magnitude", "90"} });
	pcs::Observable op("engrave");
	EXPECT_EQ(pcs::Unify(resource_params, recipe_params, op), true);
}

TEST(Unify, MatchWithBidirectionalUnknowns) {
	std::vector<pcs::Parameter> resource_params({ {"speed", "50"}, { "rotation", "90"}, {"magnitude", ""} });
	std::vector<pcs::Parameter> recipe_params({ { "speed", "" }, { "rotation", "90" }, {"magnitude", "90"} });
	pcs::Observable op("engrave");
	EXPECT_EQ(pcs::Unify(resource_params, recipe_params, op), true);
}

TEST(Unify, MismatchNumber) {
	std::vector<pcs::Parameter> resource_params({ {"speed", "50"} });
	std::vector<pcs::Parameter> recipe_params({{ "speed", "50" }, { "rotation", "90" }});
	pcs::Observable op("engrave");
	EXPECT_EQ(pcs::Unify(resource_params, recipe_params, op), false);
}

TEST(Unify, MismatchConstant) {
	std::vector<pcs::Parameter> resource_params({ {"speed", "50"}, { "rotation", "90"}, {"magnitude", "120"} });
	std::vector<pcs::Parameter> recipe_params({ { "speed", "50" }, { "rotation", "90" }, {"magnitude", "90"} });
	pcs::Observable op("engrave");
	EXPECT_EQ(pcs::Unify(resource_params, recipe_params, op), false);
}