#include "pcs/operation/guard.h"

#include <ostream>

#include "pcs/operation/operation.h"

namespace pcs {

	Guard::Guard(const std::string& name)
		: name_(name) {}

	const std::string& Guard::name() const {
		return name_;
	}

	bool Guard::operator==(const Guard& other) const {
		return (name_ == other.name_);
	}

	std::ostream& operator<<(std::ostream& os, const Guard& guard) {
		os << "Guard name: " << guard.name_ << '\n';
		return os;
	}

}