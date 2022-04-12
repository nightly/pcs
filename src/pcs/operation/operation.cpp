#include "pcs/operation/operation.h"

#include <ostream>
#include <string>
#include <span>

namespace pcs {

	Operation::Operation(const std::string& name)
		: name_(name) {}

	Operation::Operation(const std::string& name, const std::span<std::string>& input, const std::span<std::string>& output)
		: name_(name) {
		input_.assign(input.begin(), input.end());
		output_.assign(output.begin(), output.end());
	}

	bool Operation::operator==(const Operation& other) const {
		return (name_ == other.name_) && (input_ == other.input_) && (output_ == other.output_);
	}

	std::ostream& operator<<(std::ostream& os, const Operation& operation) {
		os << "Operation Name: " << operation.name_ << '\n';
		if (operation.input_.size() >= 1) {
			os << "  Input parts: \n";
			for (const auto& p : operation.input_) {
				os << "    " << p << "\n";;
			}
		}
		if (operation.output_.size() >= 1) {
			os << "  Output parts: \n";
			for (const auto& p : operation.output_) {
				os << "    " << p << "\n";;
			}
		}
		return os;
	}
}