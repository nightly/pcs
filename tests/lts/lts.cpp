#include <gtest/gtest.h>
#include "pcs/lts/lts.h"

#include "pcs/lts/state.h"

TEST(LTS, Equality) {
	pcs::LTS got;
	pcs::LTS expected;
	got.AddTransition("s0", "a1", "s1");
	expected.AddTransition("s0", "a1", "s1");
	ASSERT_EQ(got, expected);
}