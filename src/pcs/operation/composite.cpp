#include "pcs/operation/composite.h"

#include <string>

namespace pcs {

	bool CompositeOperation::operator==(const CompositeOperation& other) const {
		return (guard == other.guard) && (sequential == other.sequential) && (parallel == other.parallel);
	}
}