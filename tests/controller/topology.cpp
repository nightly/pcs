#include <gtest/gtest.h>
#include "pcs/controller/topology.h"

#include <array>

#include "pcs/lts/lts.h"
#include "pcs/lts/state.h"
#include "pcs/lts/parsers.h"

TEST(LTSMerge, SelfMerge) {
	pcs::LabelledTransitionSystem expected, got;
	std::array<pcs::LabelledTransitionSystem<>, 2> ltss;
	pcs::ReadFromFile(ltss[0], "../../tests/lts/testdata/lts1.txt");
	pcs::ReadFromFile(ltss[1], "../../tests/lts/testdata/lts1.txt");
	pcs::ReadFromFile(expected, "../../tests/controller/testdata/lts_1_self_merge.txt"); 

	got = pcs::Combine(ltss);
	ASSERT_EQ(expected, got);
}

TEST(LTSMerge, Merge1And2) {
	pcs::LabelledTransitionSystem expected, got;
	std::array<pcs::LabelledTransitionSystem<>, 2> ltss;
	pcs::ReadFromFile(ltss[0], "../../tests/lts/testdata/lts1.txt");
	pcs::ReadFromFile(ltss[1], "../../tests/lts/testdata/lts2.txt");
	pcs::ReadFromFile(expected, "../../tests/controller/testdata/lts_1_2_merge.txt");

	got = pcs::Combine(ltss);
	ASSERT_EQ(expected, got);
}

TEST(LTSMerge, MergeMany) {
	pcs::LabelledTransitionSystem expected, got;
	std::array<pcs::LabelledTransitionSystem<>, 4> ltss;
	pcs::ReadFromFile(ltss[0], "../../tests/lts/testdata/lts1.txt");
	pcs::ReadFromFile(ltss[1], "../../tests/lts/testdata/lts2.txt");
	pcs::ReadFromFile(ltss[2], "../../tests/lts/testdata/lts3.txt");
	pcs::ReadFromFile(ltss[3], "../../tests/lts/testdata/lts4.txt");
	pcs::ReadFromFile(expected, "../../tests/controller/testdata/lts_1_2_3_4_merge.txt");

	got = pcs::Combine(ltss);
	ASSERT_EQ(expected, got);
}