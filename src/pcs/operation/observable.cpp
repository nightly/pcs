#include "pcs/operation/observable.h"

#include <ostream>
#include <string>
#include <span>

namespace pcs {

	Observable::Observable(const std::string& name)
		: name_(name) {}

	bool Observable::operator==(const Observable& other) const {
		return (name_ == other.name_);
	}

	const std::string& Observable::name() const {
		return name_;
	}

	std::ostream& operator<<(std::ostream& os, const Observable& operation) {
		os << "Operation Name: " << operation.name_ << '\n';
		return os;
	}
}