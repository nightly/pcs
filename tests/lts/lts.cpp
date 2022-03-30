#include <gtest/gtest.h>
#include "pcs/lts/lts.h"

#include "pcs/lts/state.h"

TEST(LTS, Equality) {
	pcs::State s0("s0");
	s0.AddTransistion("a1", "s1");
	pcs::LabelledTransitionSystem got;
	pcs::LabelledTransitionSystem expected;
	got.AddState(s0);
	expected.AddState(s0);
	ASSERT_EQ(got, expected);
}