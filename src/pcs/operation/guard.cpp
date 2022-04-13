#include "pcs/operation/guard.h"

#include <ostream>

namespace pcs {

	Guard::Guard(const std::string& name)
		: name_(name) {}


	bool Guard::operator==(const Guard& other) const {
		return (name_ == other.name_);
	}

	std::ostream& operator<<(std::ostream& os, const Guard& guard) {
		os << "Guard name: " << guard.name_ << '\n';
		return os;
	}

}