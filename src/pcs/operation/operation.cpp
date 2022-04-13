#include "pcs/operation/operation.h"

#include <ostream>
#include <string>
#include <span>

namespace pcs {

	Operation::Operation(const std::string& name)
		: name_(name) {}

	bool Operation::operator==(const Operation& other) const {
		return (name_ == other.name_);
	}
	
	const std::string& Operation::name() const {
		return name_;
	}

	std::ostream& operator<<(std::ostream& os, const Operation& operation) {
		os << "Operation Name: " << operation.name_ << '\n';
		return os;
	}
}