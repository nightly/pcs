#include "pcs/operation/operation.h"

namespace pcs {

	Operation::Operation() {};

	bool Operation::operator==(const Operation& other) const {
		return (name == other.name) && (input == other.input) && (output == other.output);
	}

}