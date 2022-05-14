#pragma once

#include <string>
#include <vector>
#include <span>

namespace pcs {

	class IOperation {
	public:
		virtual ~IOperation() = default;
		virtual const std::string& name() const = 0;
	};
}