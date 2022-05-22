#include "pcs/operation/parsers/label.h"

#include <string>
#include <memory>
#include <optional>

#include "pcs/operation/observable.h"
#include "pcs/operation/transfer.h"
#include "pcs/operation/nop.h"

namespace pcs {

	std::unique_ptr<IOperation> StringToOperation(const std::string& label) {
		if (label.find("in:") != std::string::npos) {
			return std::make_unique<TransferOperation>(TransferType::in, stoull(label.substr(3)));
		}
		else if (label.find("out:") != std::string::npos) {
			return std::make_unique<TransferOperation>(TransferType::out, stoull(label.substr(4)));
		}
		else if (label == "nop") {
			return std::make_unique<Nop>();
		}
		return std::make_unique<Observable>(label);
	}

	std::optional<TransferOperation> StringToTransfer(const std::string& label) {
		if (label.find("in:") != std::string::npos) {
			return TransferOperation(TransferType::in, stoull(label.substr(3)));
		} else if (label.find("out:") != std::string::npos) {
			return TransferOperation(TransferType::out, stoull(label.substr(4)));
		}
		return {};
	}

}