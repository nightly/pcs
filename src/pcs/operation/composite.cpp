#include "pcs/operation/composite.h"

#include <sstream>
#include <string>

namespace pcs {
	CompositeOperation::CompositeOperation() {};	

	bool CompositeOperation::operator==(const CompositeOperation& other) const {
		return (guard == other.guard) && (sequential == other.sequential) && (parallel == other.parallel);
	}
}