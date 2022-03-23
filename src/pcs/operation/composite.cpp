#include "pcs/operation/composite.h"

#include <sstream>
#include <string>

namespace pcs {
	CompositeOperation::CompositeOperation() {};

	CompositeOperation::CompositeOperation(const std::string& str) {
		// 1. Delimit the string based on (";" & "||")
		// 2. Further delimit for (x...)(y...)
		// 3. Parse into CompositeOperation
	}

	bool CompositeOperation::operator==(const CompositeOperation& other) const {
		return (guard == other.guard) && (sequential_ops == other.sequential_ops) && (parallel_ops == other.parallel_ops);
	}
}