#include <gtest/gtest.h>
#include "pcs/product/parser.h"

#include "pcs/lts/lts.h"
#include "pcs/operation/composite.h"

TEST(ProductParser, DISABLED_Recipe1) {
	pcs::LabelledTransitionSystem<pcs::CompositeOperation> got, expected;
	pcs::ReadFromJsonFile(got, "../../tests/product/testdata/recipe_1.json");

	// ASSERT_EQ(got, expected);
}