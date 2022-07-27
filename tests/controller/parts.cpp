#include "pcs/controller/parts.h"
#include <gtest/gtest.h>

#include <vector>
#include <unordered_set>
#include <memory>

TEST(Parts, Add) {
	pcs::Parts parts(2);
	pcs::ParameterizedOp add_op = pcs::StringToParameterizedOp("add");
	std::pair<size_t, pcs::ParameterizedOp> add_t = std::make_pair(0, add_op);
	
	parts.Add(add_t, std::vector<std::string>{"p1", "p2", "p3"});
	EXPECT_EQ(parts.AtResource(0), std::unordered_set<std::string>({ "p1", "p2", "p3" }));
}

TEST(Parts, Allocate) {
	pcs::Parts parts(5);
	pcs::ParameterizedOp add_op = pcs::StringToParameterizedOp("add");
	std::pair<size_t, pcs::ParameterizedOp> add_t = std::make_pair(0, add_op);
	parts.Add(add_t, std::vector<std::string>{"p1", "p2", "p3"});

	pcs::ParameterizedOp allocate_op = pcs::StringToParameterizedOp("allocate");
	std::pair<size_t, pcs::ParameterizedOp> allocate_t = std::make_pair(0, allocate_op);
	bool allocate = parts.Allocate(allocate_t, std::unordered_set<std::string>{"p1", "p2", "p3"});
	EXPECT_EQ(allocate, true);
	EXPECT_EQ(parts.AtResource(0), std::unordered_set<std::string>());
}

TEST(Parts, AllocateMissingParts) {
	pcs::Parts parts(5);
	pcs::ParameterizedOp add_op = pcs::StringToParameterizedOp("add");
	std::pair<size_t, pcs::ParameterizedOp> add_t = std::make_pair(0, add_op);
	parts.Add(add_t, std::vector<std::string>{ "p1", "p3" });
	
	pcs::ParameterizedOp allocate_op = pcs::StringToParameterizedOp("allocate");
	std::pair<size_t, pcs::ParameterizedOp> allocate_t = std::make_pair(0, allocate_op);
	bool allocate = parts.Allocate(allocate_t, std::unordered_set<std::string>{ "p1", "p2", "p3" });
	EXPECT_EQ(allocate, false);
}

TEST(Parts, Synchronize) {
	pcs::Parts parts(10), expected_parts(10);
	pcs::ParameterizedOp add_op = pcs::StringToParameterizedOp("add");
	std::pair<size_t, pcs::ParameterizedOp> add_t = std::make_pair(0, add_op);
	parts.Add(add_t, std::vector<std::string>{ "p1", "p3" });

	bool sync = parts.Synchronize(6, 0, std::unordered_set<std::string>{"p1", "p3"});
	ASSERT_EQ(sync, true);

	pcs::ParameterizedOp add_op_expected = pcs::StringToParameterizedOp("add");
	std::pair<size_t, pcs::ParameterizedOp> add_t_expected = std::make_pair(6, add_op_expected);
	expected_parts.Add(add_t_expected, std::vector<std::string>{ "p1", "p3" });
	ASSERT_EQ(parts, expected_parts);
}

TEST(Parts, SynchronizeNothing) {
	pcs::Parts parts(10);
	bool sync = parts.Synchronize(6, 0, std::unordered_set<std::string>{"p1", "p3"});
	ASSERT_EQ(sync, false);
}

TEST(Parts, SynchronizeMissingParts) {
	pcs::Parts parts(10), expected_parts(10);
	pcs::ParameterizedOp add_op = pcs::StringToParameterizedOp("add");
	std::pair<size_t, pcs::ParameterizedOp> add_t = std::make_pair(0, add_op);
	parts.Add(add_t, std::vector<std::string>{ "p1"});

	bool sync = parts.Synchronize(6, 0, std::unordered_set<std::string>{"p1", "p3"});
	ASSERT_EQ(sync, true);

	pcs::ParameterizedOp add_op_expected = pcs::StringToParameterizedOp("add_op");
	std::pair<size_t, pcs::ParameterizedOp> add_t_expected = std::make_pair(6, add_op_expected);
	expected_parts.Add(add_t_expected, std::vector<std::string>{ "p1" });
	ASSERT_EQ(parts, expected_parts);
}

TEST(Parts, SynchronizeSomeParts) {
	pcs::Parts parts(10);
	pcs::ParameterizedOp add_op = pcs::StringToParameterizedOp("add");
	std::pair<size_t, pcs::ParameterizedOp> add_t = std::make_pair(0, add_op);
	parts.Add(add_t, std::vector<std::string>{ "p1", "p3"});

	bool sync = parts.Synchronize(6, 0, std::unordered_set<std::string>{"p1"});
	ASSERT_EQ(sync, true);

	EXPECT_EQ(parts.AtResource(6), std::unordered_set<std::string>({ "p1" }));
	EXPECT_EQ(parts.AtResource(0), std::unordered_set<std::string>({ "p3"}));
}

