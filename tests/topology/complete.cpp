#include <gtest/gtest.h>
#include "pcs/topology/complete.h"

#include <array>
#include <string>

#include "lts/lts.h"
#include "lts/state.h"
#include "lts/parsers/parsers.h"
#include "lts/writers.h"


TEST(CompleteTopology, IterativeRecursive) {
	std::vector<nightly::LTS<std::string, std::string>> ltss;
	ltss.resize(5);
	nightly::ReadFromFile(ltss[0], "../../data/pad/Resource1.txt");
	nightly::ReadFromFile(ltss[1], "../../data/pad/Resource2.txt");
	nightly::ReadFromFile(ltss[2], "../../data/pad/Resource3.txt");
	nightly::ReadFromFile(ltss[3], "../../data/pad/Resource4.txt");
	nightly::ReadFromFile(ltss[4], "../../data/pad/Resource5.txt");

	pcs::CompleteTopology it(ltss);
	pcs::CompleteTopology rt(ltss);
	const auto& iterative = it.lts();
	const auto& recursive = rt.lts();

	ASSERT_EQ(iterative, recursive);
}

TEST(CompleteTopology, SelfMerge) {
	nightly::LTS<std::vector<std::string>, std::pair<size_t, std::string>, boost::hash<std::vector<std::string>>> expected, got;
	std::vector<nightly::LTS<std::string, std::string>> ltss;
	ltss.resize(2);
	nightly::ReadFromFile(ltss[0], "../../tests/topology/testdata/lts1.txt");
	nightly::ReadFromFile(ltss[1], "../../tests/topology/testdata/lts1.txt");
	nightly::ReadFromFile(expected, "../../tests/topology/testdata/expected/lts_1_self_merge.txt"); 

	pcs::CompleteTopology ct(ltss);
	got = ct.lts();
	ASSERT_EQ(expected, got);
}

TEST(CompleteTopology, Merge1And2) {
	nightly::LTS<std::vector<std::string>, std::pair<size_t, std::string>, boost::hash<std::vector<std::string>>> expected, got;
	std::vector<nightly::LTS<std::string, std::string>> ltss;
	ltss.resize(2);
	nightly::ReadFromFile(ltss[0], "../../tests/topology/testdata/lts1.txt");
	nightly::ReadFromFile(ltss[1], "../../tests/topology/testdata/lts2.txt");
	nightly::ReadFromFile(expected, "../../tests/topology/testdata/expected/lts_1_2_merge.txt");

	pcs::CompleteTopology ct(ltss);
	got = ct.lts();
	ASSERT_EQ(expected, got);
}

TEST(CompleteTopology, MergeMany) {
	nightly::LTS<std::vector<std::string>, std::pair<size_t, std::string>, boost::hash<std::vector<std::string>>> expected, got;
	std::vector<nightly::LTS<std::string, std::string>> ltss;
	ltss.resize(4);
	nightly::ReadFromFile(ltss[0], "../../tests/topology/testdata/lts1.txt");
	nightly::ReadFromFile(ltss[1], "../../tests/topology/testdata/lts2.txt");
	nightly::ReadFromFile(ltss[2], "../../tests/topology/testdata/lts3.txt");
	nightly::ReadFromFile(ltss[3], "../../tests/topology/testdata/lts4.txt");
	nightly::ReadFromFile(expected, "../../tests/topology/testdata/expected/lts_1_2_3_4_merge.txt");

	pcs::CompleteTopology ct(ltss);
	got = ct.lts();
	ASSERT_EQ(expected, got);
}

TEST(CompleteTopology, MergeWithTransferOps) {
	nightly::LTS<std::vector<std::string>, std::pair<size_t, std::string>, boost::hash<std::vector<std::string>>> expected, got;
	std::vector<nightly::LTS<std::string, std::string>> ltss;
	ltss.resize(2);
	nightly::ReadFromFile(ltss[0], "../../data/pad/Resource1.txt");
	nightly::ReadFromFile(ltss[1], "../../data/pad/Resource5.txt");
	nightly::ReadFromFile(expected, "../../tests/topology/testdata/expected/r1_5_merge.txt");

	pcs::CompleteTopology ct(ltss);
	got = ct.lts();
	ASSERT_EQ(expected, got);
}