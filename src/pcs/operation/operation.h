#pragma once

#include <string>
#include <vector>
#include <span>
#include <memory>

namespace pcs {

	class IOperation {
	public:
		virtual ~IOperation() = default;
		virtual const std::string& name() const = 0;
		virtual IOperation* clone() const = 0;
	};
}