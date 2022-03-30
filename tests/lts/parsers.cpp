#include "pcs/lts/parsers.h"
#include <gtest/gtest.h>

#include <string>
#include <filesystem>

#include "pcs/lts/lts.h"

TEST(ParseLTS, LTS1) {
	pcs::LabelledTransitionSystem got;
	pcs::ReadFromFile(got, "../../tests/lts/testdata/lts1.txt");

	pcs::LabelledTransitionSystem expected;
	pcs::State s0("s0");
	s0.AddTransistion("a1", "s1");
	expected.AddState(s0);
	expected.SetInitialState("s0", false);

	pcs::State s1("s1");
	s1.AddTransistion("a2", "s2");
	expected.AddState(s1);

	pcs::State s2("s2");
	expected.AddState(s2);

	ASSERT_EQ(got, expected);
}

TEST(ParseJsonLTS, LTS1) {
	pcs::LabelledTransitionSystem got;
	pcs::ReadFromJsonFile(got, "../../tests/lts/testdata/lts1.json");

	pcs::LabelledTransitionSystem expected;
	pcs::State s0("s0");
	s0.AddTransistion("a1", "s1");
	expected.AddState(s0);
	expected.SetInitialState("s0", false);

	pcs::State s1("s1");
	s1.AddTransistion("a2", "s2");
	expected.AddState(s1);

	pcs::State s2("s2");
	expected.AddState(s2);

	ASSERT_EQ(got, expected);
}