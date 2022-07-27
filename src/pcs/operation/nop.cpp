#include "pcs/operation/nop.h"

#include "pcs/operation/operation.h"

namespace pcs {

	Nop::Nop()
		: name_("nop") {}

	const std::string& Nop::name() const {
		return name_;
	}

	bool Nop::operator==(const Nop& other) const {
		return name_ == other.name_;
	}

	Nop* Nop::clone() const {
		return new Nop(*this);
	}
}