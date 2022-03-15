#include "pcs/lts/parser.h"
#include <gtest/gtest.h>

#include <string>
#include <filesystem>

#include "pcs/lts/lts.h"
#include "pcs/lts/merge.h"

TEST(ParseLTS, LTS1) {
	pcs::lts::LabelledTransitionSystem got;
	pcs::lts::ReadFromFile(got, "../../../../data/lts1.txt");
	
	std::vector<pcs::lts::LabelledTransitionSystem> expectedvector;
	pcs::lts::LabelledTransitionSystem expected;
	pcs::lts::State s0("s0");
	s0.AddTransistion("a1", "s1");
	expected.SetInitialState("s0");
	expected.AddState(s0);

	pcs::lts::State s1("s1");
	s1.AddTransistion("a2", "s2");
	expected.AddState(s1);
	
	pcs::lts::State s2("s2");
	s2.AddTransistion("a3", "s0");
	expected.AddState(s2);

	pcs::lts::LabelledTransitionSystem expected2;
	pcs::lts::State ss0("s0");
	ss0.AddTransistion("b1", "s2");
	expected2.SetInitialState("s0");
	expected2.AddState(ss0);

	pcs::lts::State ss1("s1");
	ss1.AddTransistion("b2", "s3");
	expected2.AddState(ss1);

	pcs::lts::State ss2("s2");
	ss2.AddTransistion("b3", "s3");
	expected2.AddState(ss2);

	pcs::lts::State ss3("s3");
	ss3.AddTransistion("b4", "s0");
	expected2.AddState(ss3);

	pcs::lts::LabelledTransitionSystem expected3;
	pcs::lts::State sss0("s0");
	sss0.AddTransistion("c1", "s2");
	expected3.SetInitialState("s0");
	expected3.AddState(sss0);

	pcs::lts::State sss1("s1");
	sss1.AddTransistion("a2", "s3");
	expected3.AddState(sss1);

	pcs::lts::State sss2("s2");
	sss2.AddTransistion("a3", "s3");
	expected3.AddState(sss2);

	pcs::lts::State sss3("s3");
	sss3.AddTransistion("a4", "s2");
	expected3.AddState(sss3);

	expectedvector.push_back(expected);
	expectedvector.push_back(expected2);
	//expectedvector.push_back(expected3);


	pcs::lts::merge mer;
	mer.merger(expectedvector);

	
	//ASSERT_EQ(got, expected);
}