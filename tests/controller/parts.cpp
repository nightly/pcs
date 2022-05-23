#include "pcs/controller/parts.h"
#include <gtest/gtest.h>


TEST(Parts, Allocate) {
	pcs::Parts parts(5);
	std::pair<size_t, std::string> add_t(0, "add_op");
	parts.Add(add_t, std::vector<std::string>({"p1", "p2", "p3"}));
	std::pair<size_t, std::string> allocate_t(0, "allocate_op");
	EXPECT_EQ(parts.AtResource(0), std::vector<std::string>({"p1", "p2", "p3"}));

	bool allocate = parts.Allocate(allocate_t, std::vector<std::string>({"p1", "p2", "p3"}));
	EXPECT_EQ(allocate, true);
	EXPECT_EQ(parts.AtResource(0), std::vector<std::string>());
}

TEST(Parts, AllocateMissingParts) {
	pcs::Parts parts(5);
	std::pair<size_t, std::string> add_t(0, "add_op");
	parts.Add(add_t, std::vector<std::string>({ "p1", "p3" }));
	std::pair<size_t, std::string> allocate_t(0, "allocate_op");
	bool allocate = parts.Allocate(allocate_t, std::vector<std::string>({ "p1", "p2", "p3" }));
	EXPECT_EQ(allocate, false);
}

