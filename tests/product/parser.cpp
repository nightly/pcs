#include <gtest/gtest.h>
#include "pcs/product/parser.h"

#include <array>
#include <vector>

#include "pcs/lts/lts.h"
#include "pcs/operation/composite.h"
#include "pcs/operation/guard.h"

using namespace pcs;

TEST(ProductParser, Recipe1) {
	LabelledTransitionSystem<CompositeOperation> got, expected;
	ReadFromJsonFile(got, "../../tests/product/testdata/recipe1.json");

	expected.SetInitialState("A", false);

	CompositeOperation co1;
	Operation co1_seq1, co1_seq2, co1_seq3;
	co1.guard.name_ = "check";
	co1.guard.input_ = std::vector<std::string>({"c"});
	co1_seq1.name_ = "load";
	co1_seq1.output_ = std::vector<std::string>({"f"});
	co1_seq2.name_ = "separate";
	co1_seq2.input_ = std::vector<std::string>({"f"});
	co1_seq2.output_ = std::vector<std::string>({"p","h"});
	co1_seq3.name_ = "applyglue";
	co1_seq3.input_ = std::vector<std::string>({"p"});
	co1_seq3.output_ = std::vector<std::string>({"p"});
	co1.sequential.insert(co1.sequential.end(), { co1_seq1, co1_seq2, co1_seq3 });
	expected.AddTransition("A", co1, "B", true);

	CompositeOperation co2;
	Operation co2_seq1, co2_par1;
	co2_seq1.name_ = "rem";
	co2_seq1.input_ = std::vector<std::string>({"h2"});
	co2_seq1.output_ = std::vector<std::string>({"h2"});
	co2_par1.name_ = "store";
	co2_par1.input_ = std::vector<std::string>({"p0"});
	co2.sequential.emplace_back(co2_seq1);
	co2.parallel.emplace_back(co2_par1);
	expected.AddTransition("A", co2, "E", true);

	ASSERT_EQ(got, expected);
}