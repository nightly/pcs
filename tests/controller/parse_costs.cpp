#include "pcs/controller/solvers/parse_costs.h"
#include <gtest/gtest.h>

#include <vector>
#include <filesystem>

TEST(ControllerParseCosts, 3) {
	std::filesystem::path path("../../tests/controller/testdata/best/costs_3.txt");
	std::vector<double> expected = { 100, 150, 100 };
	std::vector<double> got = pcs::ParseCosts(path, 3);
	ASSERT_EQ(got, expected);
}

TEST(ControllerParseCosts, 5) {
	std::filesystem::path path("../../tests/controller/testdata/best/costs_5.txt");
	std::vector<double> expected(5, 100);
	std::vector<double> got = pcs::ParseCosts(path, 5);
	ASSERT_EQ(expected, got);
}