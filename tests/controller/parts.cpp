#include "pcs/controller/parts.h"
#include <gtest/gtest.h>

#include <vector>
#include <unordered_set>

TEST(Parts, Add) {
	pcs::Parts parts(2);
	std::pair<size_t, std::string> add_t(0, "add_op");
	parts.Add(add_t, std::vector<std::string>{"p1", "p2", "p3"});
	EXPECT_EQ(parts.AtResource(0), std::vector<std::string>({ "p1", "p2", "p3" }));
}

TEST(Parts, Allocate) {
	pcs::Parts parts(5);
	std::pair<size_t, std::string> add_t(0, "add_op");
	parts.Add(add_t, std::vector<std::string>{"p1", "p2", "p3"});

	std::pair<size_t, std::string> allocate_t(0, "allocate_op");
	bool allocate = parts.Allocate(allocate_t, std::vector<std::string>{"p1", "p2", "p3"});
	EXPECT_EQ(allocate, true);
	EXPECT_EQ(parts.AtResource(0), std::vector<std::string>());
}

TEST(Parts, AllocateMissingParts) {
	pcs::Parts parts(5);
	std::pair<size_t, std::string> add_t(0, "add_op");
	parts.Add(add_t, std::vector<std::string>{ "p1", "p3" });
	std::pair<size_t, std::string> allocate_t(0, "allocate_op");
	bool allocate = parts.Allocate(allocate_t, std::vector<std::string>{ "p1", "p2", "p3" });
	EXPECT_EQ(allocate, false);
}

TEST(Parts, Synchronize) {
	pcs::Parts parts(10), expected_parts(10);
	std::pair<size_t, std::string> add_t(0, "add_op");
	parts.Add(add_t, std::vector<std::string>{ "p1", "p3" });
	bool sync = parts.Synchronize(6, 0, std::vector<std::string>{"p1", "p3"});
	ASSERT_EQ(sync, true);

	std::pair<size_t, std::string> add_t_expected(6, "add_op");
	expected_parts.Add(add_t_expected, std::vector<std::string>{ "p1", "p3" });
	ASSERT_EQ(parts, expected_parts);
}

TEST(Parts, SynchronizeMissingParts) {
	pcs::Parts parts(10);
	std::pair<size_t, std::string> add_t(0, "add_op");
	parts.Add(add_t, std::vector<std::string>{ "p1"});
	bool sync = parts.Synchronize(6, 0, std::vector<std::string>{"p1", "p3"});
	ASSERT_EQ(sync, false);
}

