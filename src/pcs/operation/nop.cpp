#include "pcs/operation/nop.h"

#include "pcs/operation/operation.h"

namespace pcs {

	Nop::Nop()
		: name_("nop") {}

	const std::string& Nop::name() const {
		return name_;
	}
}