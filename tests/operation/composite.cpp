#include <gtest/gtest.h>
#include "pcs/operation/composite.h"

#include <string>

TEST(Operation, DISABLED_Composite1) {
	std::string str(";load()(f) ; separate(f)(p,h) ; applyglue(p)(p) ; engrave(h)(h) ; insert(p,h)(h2) ; getimages(h2)(h2) || par(x)()");
	pcs::CompositeOperation got(str), expected;

	ASSERT_EQ(got, expected);
}

TEST(Operation, DISABLED_Composite2) {
	std::string str("check(h2) ; getforce(h2)(h2)");
	pcs::CompositeOperation got(str), expected;


	ASSERT_EQ(got, expected);
}