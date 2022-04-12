#pragma once

#include "pcs/operation/operation.h"

namespace pcs {

	enum class Transfer { in, out };

	struct TransferOperation : public IOperation {
	public:
		Transfer type;
		size_t n;
	public:
		TransferOperation() = default;
		TransferOperation(Transfer type, size_t n);
		TransferOperation Inverse() const;
	};
}