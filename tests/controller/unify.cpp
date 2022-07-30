#include "pcs/controller/unify.h"
#include <gtest/gtest.h>

#include <vector>
#include <unordered_map>

#include "pcs/operation/observable.h"
#include "pcs/operation/parameters.h"
#include "pcs/operation/parameterized_op.h"

TEST(Unify, Match) {
	pcs::Parameters resource_params(std::unordered_map<std::string, std::string>{ {"speed", "50"}, { "rotation", "90"} });
	pcs::Parameters recipe_params(std::unordered_map<std::string, std::string>{ { "speed", "50" }, { "rotation", "90" } });
	pcs::Observable op("engrave");
	EXPECT_EQ(pcs::Unify(resource_params, recipe_params, op), true);
}


TEST(Unify, MatchWithConstant) {
	pcs::Parameters resource_params(std::unordered_map<std::string, std::string>{ {"speed", "50"}, { "rotation", "90"}, {"magnitude", ""} });
	pcs::Parameters recipe_params(std::unordered_map<std::string, std::string>{ { "speed", "50" }, { "rotation", "90" }, {"magnitude", "90"} });
	pcs::Observable op("engrave");
	EXPECT_EQ(pcs::Unify(resource_params, recipe_params, op), true);
}

TEST(Unify, MatchWithBidirectionalUnknowns) {
	pcs::Parameters resource_params(std::unordered_map<std::string, std::string>{ {"speed", "50"}, { "rotation", "90"}, {"magnitude", ""} });
	pcs::Parameters recipe_params(std::unordered_map<std::string, std::string>{ { "speed", "" }, { "rotation", "90" }, {"magnitude", "90"} });
	pcs::Observable op("engrave");
	EXPECT_EQ(pcs::Unify(resource_params, recipe_params, op), true);
}

TEST(Unify, MismatchNumber) {
	pcs::Parameters resource_params(std::unordered_map<std::string, std::string>{ {"speed", "50"} });
	pcs::Parameters recipe_params(std::unordered_map<std::string, std::string>{ { "speed", "50" }, { "rotation", "90" } });
	pcs::Observable op("engrave");
	EXPECT_EQ(pcs::Unify(resource_params, recipe_params, op), false);
}

TEST(Unify, MismatchConstant) {
	pcs::Parameters resource_params(std::unordered_map<std::string, std::string>{ {"speed", "50"}, { "rotation", "90"}, {"magnitude", "120"} });
	pcs::Parameters recipe_params(std::unordered_map<std::string, std::string>{ { "speed", "50" }, { "rotation", "90" }, {"magnitude", "90"} });
	pcs::Observable op("engrave");
	EXPECT_EQ(pcs::Unify(resource_params, recipe_params, op), false);
}