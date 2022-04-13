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

	class Operation : public IOperation {
	public:
		std::string name_;
	public:
		Operation() = default;
		Operation(const std::string& name);
		~Operation() = default;

		const std::string& name() const override;

		bool operator==(const Operation& other) const;
		friend std::ostream& operator<<(std::ostream& os, const Operation& operation);
	};
}