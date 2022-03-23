#include <gtest/gtest.h>
#include "pcs/operation/composite.h"

#include <string>

TEST(Operation, DISABLED_Composite1) {
	std::string str(";load()(f) ; separate(f)(p,h) ; applyglue(p)(p) ; engrave(h)(h) ; insert(p,h)(h2) ; getimages(h2)(h2) || par(x)()");
	pcs::CompositeOperation got(str), expected;

	expected.sequential_ops.emplace_back("load", std::vector<std::string>{}, std::vector<std::string>{"f"});
	expected.sequential_ops.emplace_back("separate", std::vector<std::string>{"f"}, std::vector<std::string>{"p","h"});
	expected.sequential_ops.emplace_back("applyglue", std::vector<std::string>{"p"}, std::vector<std::string>{"p"});
	expected.sequential_ops.emplace_back("engrave", std::vector<std::string>{"h"}, std::vector<std::string>{"h"});
	expected.sequential_ops.emplace_back("insert", std::vector<std::string>{"p","h"}, std::vector<std::string>{"h2"});
	expected.sequential_ops.emplace_back("getimages", std::vector<std::string>{"h2"}, std::vector<std::string>{"h2"});

	expected.parallel_ops.emplace_back("par", std::vector<std::string>{"x"}, std::vector<std::string>{});

	ASSERT_EQ(got, expected);
}

TEST(Operation, DISABLED_Composite2) {
	std::string str("check(h2) ; getforce(h2)(h2)");
	pcs::CompositeOperation got(str), expected;

	expected.guard = std::make_pair("check", std::vector<std::string>{"h2"});
	expected.sequential_ops.emplace_back("getforce", std::vector<std::string>{"h2"}, std::vector<std::string>{"h2"});

	ASSERT_EQ(got, expected);
}