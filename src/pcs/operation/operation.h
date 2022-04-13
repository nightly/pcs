#pragma once

#include <string>
#include <vector>
#include <span>

namespace pcs {

	class IOperation {
	public:
		virtual ~IOperation() = default;
	};

	class Operation : public IOperation {
	public:
		std::string name_;
	public:
		Operation() = default;
		Operation(const std::string& name);

		bool operator==(const Operation& other) const;
		friend std::ostream& operator<<(std::ostream& os, const Operation& operation);
	};
}