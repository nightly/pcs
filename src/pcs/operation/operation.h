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
		std::vector<std::string> input_;
		std::vector<std::string> output_;
	public:
		Operation() = default;
		Operation(const std::string& name);

		Operation(const std::string& name, const std::span<std::string>& input, const std::span<std::string>& output);


		bool operator==(const Operation& other) const;
		friend std::ostream& operator<<(std::ostream& os, const Operation& operation);
	};
}