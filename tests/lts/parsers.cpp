#include "pcs/lts/parsers/string_string.h"
#include <gtest/gtest.h>

#include <string>
#include <filesystem>

#include "pcs/lts/lts.h"

TEST(ParseLTS, LTS1) {
	pcs::LTS got;
	pcs::ReadFromFile(got, "../../tests/lts/testdata/lts1.txt");

	pcs::LTS expected;
	expected.set_initial_state("s0", true);
	expected.AddTransition("s0", "a1", "s1");
	expected.AddTransition("s1", "a2", "s2");

	ASSERT_EQ(got, expected);
}

TEST(ParseJsonLTS, LTS1) {
	pcs::LTS got;
	pcs::ReadFromJsonFile(got, "../../tests/lts/testdata/lts1.json");

	pcs::LTS expected;
	expected.set_initial_state("s0", true);
	expected.AddTransition("s0", "a1", "s1");
	expected.AddTransition("s1", "a2", "s2");

	ASSERT_EQ(got, expected);
}