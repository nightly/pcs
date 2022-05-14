#include <gtest/gtest.h>
#include "pcs/product/parsers/recipe.h"

#include <array>
#include <vector>

#include "pcs/lts/lts.h"
#include "pcs/operation/composite.h"
#include "pcs/operation/guard.h"
#include "pcs/lts/writers.h"

using namespace pcs;

TEST(ProductParser, Recipe1) {
	LTS<std::string, CompositeOperation> got, expected;
	ReadFromJsonFile(got, "../../tests/product/testdata/recipe1.json");

	expected.set_initial_state("A", false);

	CompositeOperation co1;
	Observable co1_seq1, co1_seq2, co1_seq3;
	co1.guard.first.name_ = "check";
	co1.guard.second = std::vector<std::string>({"c"});

	co1_seq1.name_ = "load";
	co1.sequential.emplace_back(co1_seq1, std::vector<std::string>(), std::vector<std::string>({"f"}));
	
	co1_seq2.name_ = "separate";
	co1.sequential.emplace_back(co1_seq2, std::vector<std::string>({ "f" }), std::vector<std::string>({ "p","h" }));

	co1_seq3.name_ = "applyglue";
	co1.sequential.emplace_back(co1_seq3, std::vector<std::string>({ "p" }), std::vector<std::string>({ "p" }));
	
	expected.AddTransition("A", co1, "B", true);

	CompositeOperation co2;
	Observable co2_seq1, co2_par1;
	co2_seq1.name_ = "rem";
	co2.sequential.emplace_back(co2_seq1, std::vector<std::string>({ "h2" }), std::vector<std::string>({ "h2" }));
	
	co2_par1.name_ = "store";
	co2.parallel.emplace_back(co2_par1, std::vector<std::string>({ "p0" }), std::vector<std::string>());
	
	expected.AddTransition("A", co2, "E", true);

	ASSERT_EQ(got, expected);
}