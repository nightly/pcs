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
		if (guard.name_ != "") {
			return true;
		}
		return false;
	}

	std::ostream& operator<<(std::ostream& os, const CompositeOperation& co) {
		if (co.HasGuard()) {
			os << "Guard operation: \n";
			os << "  " << co.guard;
		}
		if (co.sequential.size() >= 1) {
			os << "  Sequential operations:\n";
			for (const auto& o : co.sequential) {
				os << "  " << o << "\n";
			}
		}
		if (co.parallel.size() >= 1) {
			os << "  Parallel operations:\n";
			for (const auto& o : co.parallel) {
				os << "  " << o << "\n";
			}
		}
		return os;
	}

	std::ofstream& operator<<(std::ofstream& os, const CompositeOperation& co) {
		if (co.HasGuard()) {
			os << co.guard.name_ << "(" << VectorToString(co.guard.input_) << ") ; ";
		}
		for (size_t i = 0; i < co.sequential.size(); i++) {
			os << co.sequential[i].name_ << "(" << VectorToString(co.sequential[i].input_) << ")" <<
				"(" << VectorToString(co.sequential[i].output_) << ")";
			if (i != co.sequential.size() - 1) {
				os << " ;";
			}
		}
		if (co.parallel.size() >= 1) {
			os << "|| ";
			for (size_t i = 0; i < co.parallel.size(); i++) {
				os << co.parallel[i].name_ << "(" << VectorToString(co.parallel[i].input_) << ")" <<
					"(" << VectorToString(co.parallel[i].output_) << ")";
				if (i != co.parallel.size() - 1) {
					os << " ;";
				}
			}
		}
		return os;
	}
}