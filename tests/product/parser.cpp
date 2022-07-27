#include <gtest/gtest.h>
#include "pcs/product/parsers/recipe.h"

#include <array>
#include <vector>

#include "lts/lts.h"
#include "pcs/operation/composite.h"
#include "pcs/operation/guard.h"
#include "lts/writers.h"
#include "pcs/operation/parameter.h"

using namespace pcs;

TEST(ProductParser, Recipe1) {
	nightly::LTS<std::string, CompositeOperation> got, expected;
	ReadFromJsonFile(got, "../../tests/product/testdata/recipe1.json");

	expected.set_initial_state("A");

	CompositeOperation co1;
	Observable co1_seq1, co1_seq2, co1_seq3;
	std::vector<Parameter> empty_parameters;

	co1.guard.set_operation(pcs::Observable("check"));
	co1.guard.set_input(std::unordered_set<std::string>({"c"}));

	co1_seq1.name_ = "load";
	co1.sequential.emplace_back(co1_seq1, std::unordered_set<std::string>(), empty_parameters, std::vector<std::string>({"f"}));
	
	co1_seq2.name_ = "separate";
	co1.sequential.emplace_back(co1_seq2, std::unordered_set<std::string>({ "f" }), empty_parameters, std::vector<std::string>({ "p","h" }));

	co1_seq3.name_ = "applyglue";
	co1.sequential.emplace_back(co1_seq3, std::unordered_set<std::string>({ "p" }), empty_parameters, std::vector<std::string>({ "p" }));
	
	expected.AddTransition("A", co1, "B");

	CompositeOperation co2;
	Observable co2_seq1, co2_par1;
	co2_seq1.name_ = "rem";
	co2.sequential.emplace_back(co2_seq1, std::unordered_set<std::string>({ "h2" }), empty_parameters, std::vector<std::string>({ "h2" }));
	
	co2_par1.name_ = "store";
	std::vector<Parameter> co2_par1_params;
	co2_par1_params.emplace_back("parameter1", "constant1");
	co2_par1_params.emplace_back("parameter2", "");
	co2.parallel.emplace_back(co2_par1, std::unordered_set<std::string>({ "p0" }), co2_par1_params, std::vector<std::string>());
	
	expected.AddTransition("A", co2, "E");

	ASSERT_EQ(got, expected);
}