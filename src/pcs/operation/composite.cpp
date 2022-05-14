#include "pcs/operation/composite.h"

#include <string>
#include <ostream>
#include <fstream>
#include <span>

#include "pcs/common/strings.h"

namespace pcs {

	bool CompositeOperation::operator==(const CompositeOperation& other) const {
		return (guard == other.guard) && (sequential == other.sequential) && (parallel == other.parallel);
	}

	bool CompositeOperation::HasGuard() const {
		if (guard.first.name_ != "") {
			return true;
		}
		return false;
	}

	std::ostream& operator<<(std::ostream& os, const CompositeOperation& co) {
		// @Todo: parts
		if (co.HasGuard()) {
			os << "Guard operation: \n";
			os << "  " << co.guard.first;
		}
		if (co.sequential.size() >= 1) {
			os << "  Sequential operations:\n";
			for (const auto& o : co.sequential) {
				auto& [op, input, output] = o;
				os << "  " << op << "\n";
			}
		}
		if (co.parallel.size() >= 1) {
			os << "  Parallel operations:\n";
			for (const auto& o : co.parallel) {
				auto& [op, input, output] = o;
				os << "  " << op << "\n";
			}
		}
		return os;
	}

	std::ofstream& operator<<(std::ofstream& os, const CompositeOperation& co) {
		if (co.HasGuard()) {
			os << co.guard.first.name_ << "(" << VectorToString(co.guard.second) << "); ";
		}
		for (size_t i = 0; i < co.sequential.size(); i++) {
			auto& [op, input, output] = co.sequential[i];
			os << op.name_ << "(" << VectorToString(input) << ")" <<
				"(" << VectorToString(output) << ")";
			if (i != co.sequential.size() - 1) {
				os << "; ";
			}
		}
		if (co.parallel.size() >= 1) {
			os << "|| ";
			for (size_t i = 0; i < co.parallel.size(); i++) {
				auto& [op, input, output] = co.parallel[i];
				os << op.name_ << "(" << VectorToString(input) << ")" <<
					"(" << VectorToString(output) << ")";
				if (i != co.parallel.size() - 1) {
					os << "; ";
				}
			}
		}
		return os;
	}
}

/*
	@Todo: correct writers with proper spacing & ostream with parts
*/
