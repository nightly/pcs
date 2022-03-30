#pragma once

#include <string>
#include <vector>

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
		Operation(std::string&& name, std::vector<std::string>&& input, std::vector<std::string>&& output);

		bool operator==(const Operation& other) const;
	};
}