#include <gtest/gtest.h>
#include "pcs/controller/topology.h"

#include <array>

#include "pcs/lts/lts.h"
#include "pcs/lts/state.h"
#include "pcs/lts/parser.h"

TEST(LTSMerge, SelfMerge) {
	pcs::lts::LabelledTransitionSystem expected, got;
	std::array<pcs::lts::LabelledTransitionSystem, 2> ltss;
	pcs::lts::ReadFromFile(ltss[0], "../../tests/lts/testdata/lts_1.txt");
	pcs::lts::ReadFromFile(ltss[1], "../../tests/lts/testdata/lts_1.txt");
	pcs::lts::ReadFromFile(expected, "../../tests/lts/testdata/lts_1_self_merge_expected.txt"); 

	got = pcs::topology::Combine(ltss);
	ASSERT_EQ(expected, got);
}

TEST(LTSMerge, Merge1And2) {
	pcs::lts::LabelledTransitionSystem expected, got;
	std::array<pcs::lts::LabelledTransitionSystem, 2> ltss;
	pcs::lts::ReadFromFile(ltss[0], "../../tests/lts/testdata/lts_1.txt");
	pcs::lts::ReadFromFile(ltss[1], "../../tests/lts/testdata/lts_2.txt");
	pcs::lts::ReadFromFile(expected, "../../tests/lts/testdata/lts_2_1_expected.txt");

	got = pcs::topology::Combine(ltss);
	ASSERT_EQ(expected, got);
}

TEST(LTSMerge, MergeMany) {

}