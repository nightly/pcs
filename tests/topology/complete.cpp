#include <gtest/gtest.h>
#include "pcs/topology/complete.h"

#include <array>
#include <string>

#include "pcs/lts/lts.h"
#include "pcs/lts/state.h"
#include "pcs/lts/parsers/string_string.h"
#include "pcs/lts/writers.h"

TEST(LTSMerge, DISABLED_SelfMerge) {
	pcs::LTS<std::vector<std::string>, std::pair<size_t, std::string>, boost::hash<std::vector<std::string>>> expected, got;
	std::array<pcs::LTS<std::string, std::string>, 2> ltss;
	pcs::ReadFromFile(ltss[0], "../../tests/lts/testdata/lts1.txt");
	pcs::ReadFromFile(ltss[1], "../../tests/lts/testdata/lts1.txt");
	// pcs::ReadFromFile(expected, "../../tests/topology/testdata/lts_1_self_merge.txt"); 

	pcs::CompleteTopology ct(ltss);
	got = ct.lts();
	ASSERT_EQ(expected, got);
}

TEST(LTSMerge, DISABLED_Merge1And2) {
	pcs::LTS<std::vector<std::string>, std::pair<size_t, std::string>, boost::hash<std::vector<std::string>>> expected, got;
	std::array<pcs::LTS<std::string, std::string>, 2> ltss;
	pcs::ReadFromFile(ltss[0], "../../tests/lts/testdata/lts1.txt");
	pcs::ReadFromFile(ltss[1], "../../tests/lts/testdata/lts2.txt");
	//pcs::ReadFromFile(expected, "../../tests/topology/testdata/lts_1_2_merge.txt");

	pcs::CompleteTopology ct(ltss);
	got = ct.lts();
	ASSERT_EQ(expected, got);
}

TEST(LTSMerge, DISABLED_MergeMany) {
	pcs::LTS<std::vector<std::string>, std::pair<size_t, std::string>, boost::hash<std::vector<std::string>>> expected, got;
	std::array<pcs::LTS<std::string, std::string>, 4> ltss;
	pcs::ReadFromFile(ltss[0], "../../tests/lts/testdata/lts1.txt");
	pcs::ReadFromFile(ltss[1], "../../tests/lts/testdata/lts2.txt");
	pcs::ReadFromFile(ltss[2], "../../tests/lts/testdata/lts3.txt");
	pcs::ReadFromFile(ltss[3], "../../tests/lts/testdata/lts4.txt");
	// pcs::ReadFromFile(expected, "../../tests/topology/testdata/lts_1_2_3_4_merge.txt");

	pcs::CompleteTopology ct(ltss);
	got = ct.lts();
	ASSERT_EQ(expected, got);
}

TEST(LTSMerge, DISABLED_MergeWithTransferOps) {
	pcs::LTS<std::vector<std::string>, std::pair<size_t, std::string>, boost::hash<std::vector<std::string>>> expected, got;
	std::array<pcs::LTS<std::string, std::string>, 2> ltss;
	pcs::ReadFromFile(ltss[0], "../../data/pad/Resource1.txt");
	pcs::ReadFromFile(ltss[1], "../../data/pad/Resource2.txt");
	// pcs::ReadFromFile(expected, "../../tests/topology/testdata/r1_5_merge.txt");

	pcs::CompleteTopology ct(ltss);
	got = ct.lts();
	ASSERT_EQ(expected, got);
}