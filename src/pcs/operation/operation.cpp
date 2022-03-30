#include "pcs/operation/operation.h"

namespace pcs {

	bool Operation::operator==(const Operation& other) const {
		return (name_ == other.name_) && (input_ == other.input_) && (output_ == other.output_);
	}

}