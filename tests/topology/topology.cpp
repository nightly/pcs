#include <gtest/gtest.h>
#include "pcs/topology/topology.h"

#include <array>
#include <string>

#include "pcs/lts/lts.h"
#include "pcs/lts/state.h"
#include "pcs/lts/parsers/string_string.h"

TEST(LTSMerge, SelfMerge) {
	pcs::LabelledTransitionSystem<std::string> expected, got;
	std::array<pcs::LabelledTransitionSystem<std::string>, 2> ltss;
	pcs::ReadFromFile(ltss[0], "../../tests/lts/testdata/lts1.txt");
	pcs::ReadFromFile(ltss[1], "../../tests/lts/testdata/lts1.txt");
	pcs::ReadFromFile(expected, "../../tests/topology/testdata/lts_1_self_merge.txt"); 

	got = pcs::Combine(ltss);
	ASSERT_EQ(expected, got);
}

TEST(LTSMerge, Merge1And2) {
	pcs::LabelledTransitionSystem<std::string> expected, got;
	std::array<pcs::LabelledTransitionSystem<std::string>, 2> ltss;
	pcs::ReadFromFile(ltss[0], "../../tests/lts/testdata/lts1.txt");
	pcs::ReadFromFile(ltss[1], "../../tests/lts/testdata/lts2.txt");
	pcs::ReadFromFile(expected, "../../tests/topology/testdata/lts_1_2_merge.txt");

	got = pcs::Combine(ltss);
	ASSERT_EQ(expected, got);
}

TEST(LTSMerge, MergeMany) {
	pcs::LabelledTransitionSystem<std::string> expected, got;
	std::array<pcs::LabelledTransitionSystem<std::string>, 4> ltss;
	pcs::ReadFromFile(ltss[0], "../../tests/lts/testdata/lts1.txt");
	pcs::ReadFromFile(ltss[1], "../../tests/lts/testdata/lts2.txt");
	pcs::ReadFromFile(ltss[2], "../../tests/lts/testdata/lts3.txt");
	pcs::ReadFromFile(ltss[3], "../../tests/lts/testdata/lts4.txt");
	pcs::ReadFromFile(expected, "../../tests/topology/testdata/lts_1_2_3_4_merge.txt");

	got = pcs::Combine(ltss);
	ASSERT_EQ(expected, got);
}

TEST(LTSMerge, DISABLED_MergeWithTransferOps) {
	pcs::LabelledTransitionSystem<std::string> expected, got;
	std::array<pcs::LabelledTransitionSystem<std::string>, 2> ltss;
	pcs::ReadFromFile(ltss[0], "../../data/pad/Resource1.txt");
	pcs::ReadFromFile(ltss[1], "../../data/pad/Resource2.txt");
	pcs::ReadFromFile(expected, "../../tests/topology/testdata/r1_5_merge.txt");

	got = pcs::Combine(ltss);
	ASSERT_EQ(expected, got);
}