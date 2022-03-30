#include "pcs/operation/guard.h"

namespace pcs {

	/* 
	 * @brief Guard empty constructor. Guard is the first operation performed as a test before proceeding with the rest, taking only input parameters.
	 */
	Guard::Guard() {}

	bool Guard::operator==(const Guard& other) const {
		return (name_ == other.name_) && (input_ == other.input_);
	}

}