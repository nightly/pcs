#pragma once

#include <string>
#include <memory>
#include <optional>

#include "pcs/operation/operation.h"
#include "pcs/operation/nop.h"
#include "pcs/operation/transfer.h"

namespace pcs {

	std::unique_ptr<IOperation> StringToOperation(const std::string& label);
	std::optional<TransferOperation> StringToTransfer(const std::string& label);
}