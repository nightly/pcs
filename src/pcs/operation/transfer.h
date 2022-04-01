#pragma once

#include "pcs/operation/operation.h"

namespace pcs {

	struct TransferOperation : public IOperation {
	public:
		TransferOperation() = default;
		bool is_out;
		size_t n;
	};
}