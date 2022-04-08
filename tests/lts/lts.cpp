#include <gtest/gtest.h>
#include "pcs/lts/lts.h"

#include "pcs/lts/state.h"

TEST(LTS, Equality) {
	pcs::LabelledTransitionSystem got;
	pcs::LabelledTransitionSystem expected;
	got.AddTransition("s0", "a1", "s1");
	expected.AddTransition("s0", "a1", "s1");
	ASSERT_EQ(got, expected);
}