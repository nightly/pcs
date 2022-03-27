#pragma once

#include <string>
#include <vector>

namespace pcs {

	class IOperation {
	public:
		virtual ~IOperation();
	};

	class Operation : public IOperation {
	public:
		std::string name;
		std::vector<std::string> input;
		std::vector<std::string> output;
	public:
		Operation();
		Operation(std::string&& name, std::vector<std::string>&& input, std::vector<std::string>&& output);

		bool operator==(const Operation& other) const;
	};
}