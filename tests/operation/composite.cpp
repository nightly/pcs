#include <gtest/gtest.h>
#include "pcs/operation/composite.h"

#include <vector>
#include <string>
#include <unordered_set>

#include "pcs/operation/parameter.h"

TEST(CompositeOperation, LastOp) {
	pcs::CompositeOperation co;
	pcs::Observable co_seq;
	co_seq.name_ = "seq1";
	co.sequential.emplace_back(co_seq, std::unordered_set<std::string>(), std::vector<pcs::Parameter>(), std::vector<std::string>({"f"}));

	ASSERT_EQ(co.IsLastOp(1), true);
}

TEST(CompositeOperation, LastOpWithGuard) {

}