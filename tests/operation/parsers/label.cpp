#include <gtest/gtest.h>
#include "pcs/operation/parsers/label.h"

#include <string>
#include <memory>

#include "pcs/operation/operation.h"
#include "pcs/operation/transfer.h"
#include "pcs/operation/nop.h"
#include "pcs/operation/observable.h"

TEST(OperationLabelParser, SyncIn) {
	std::string label = "in:5";
	pcs::TransferOperation expected(pcs::TransferType::in, 5);
	std::unique_ptr<pcs::IOperation> got = pcs::StringToOperation(label);
	pcs::TransferOperation* got_op = dynamic_cast<pcs::TransferOperation*>(got.get());
	ASSERT_EQ(*got_op, expected);
}

TEST(OperationLabelParser, SyncOut) {
	std::string label = "out:3";
	pcs::TransferOperation expected(pcs::TransferType::out, 3);
	std::unique_ptr<pcs::IOperation> got = pcs::StringToOperation(label);
	pcs::TransferOperation* got_op = dynamic_cast<pcs::TransferOperation*>(got.get());
	ASSERT_EQ(*got_op, expected);
}

TEST(OperationLabelParser, Observable) {
	std::string label = "drill";
	pcs::Observable expected("drill");
	std::unique_ptr<pcs::IOperation> got = pcs::StringToOperation(label);
	pcs::Observable* got_op = dynamic_cast<pcs::Observable*>(got.get());
	ASSERT_EQ(*got_op, expected);
}

TEST(OperationLabelParser, Nop) {
	std::string label = "nop";
	pcs::Nop expected;
	std::unique_ptr<pcs::IOperation> got = pcs::StringToOperation(label);
	pcs::Nop* got_op = dynamic_cast<pcs::Nop*>(got.get());
	ASSERT_EQ(*got_op, expected);
}