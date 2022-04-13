#pragma once

#include "pcs/operation/operation.h"

namespace pcs {

	struct Nop : public IOperation {
	public:
		std::string name_;
	public:
		Nop();
		~Nop() = default;

		const std::string& name() const;
	};
}