#include "pcs/operation/guard.h"

#include <ostream>

namespace pcs {

	Guard::Guard(const std::string& name, const std::span<std::string>& input_parts)
		: name_(name) {
		input_.assign(input_parts.begin(), input_parts.end());
	}


	bool Guard::operator==(const Guard& other) const {
		return (name_ == other.name_) && (input_ == other.input_);
	}

	std::ostream& operator<<(std::ostream& os, const Guard& guard) {
		os << "Operation Name: " << guard.name_ << '\n';
		if (guard.input_.size() >= 1) {
			os << "  Input parts: \n";
			for (const auto& p : guard.input_) {
				os << "    " << p << "\n";
			}
		}
		return os;
	}

}