#include "pcs/operation/composite.h"

#include <string>
#include <ostream>
#include <fstream>
#include <span>
#include <vector>
#include <unordered_set>

#include "pcs/common/strings.h"

namespace pcs {

	bool CompositeOperation::operator==(const CompositeOperation& other) const {
		return (guard == other.guard) && (sequential == other.sequential) && (parallel == other.parallel);
	}

	bool CompositeOperation::HasGuard() const {
		if (guard.operation().name() != "") {
			return true;
		}
		return false;
	}

	std::ostream& operator<<(std::ostream& os, const CompositeOperation& co) {
		os << "\n";
		if (co.HasGuard()) {
			os << "     Guard operation: \n";
			os << "       " << co.guard.operation() << "(" << USetToString(co.guard.input()) << ")" << std::endl;
		}
		if (co.sequential.size() >= 1) {
			os << "     Sequential operations:\n";
			for (const auto& task : co.sequential) {
				os << "       " << task << "\n";
			}
		}
		if (co.parallel.size() >= 1) {
			os << "     Parallel operations:\n";
			for (const auto& task : co.parallel) {
				os << "       " << task << "\n";
			}
		}
		return os;
	}

	std::ofstream& operator<<(std::ofstream& os, const CompositeOperation& co) {
		if (co.HasGuard()) {
			os << "{ " << co.guard.operation().name() << "(" << USetToString(co.guard.input()) << ") }; ";
		}
		for (size_t i = 0; i < co.sequential.size(); i++) {
			os << co.sequential[i];
			if (i != co.sequential.size() - 1) {
				os << "; ";
			}
		}
		if (co.parallel.size() >= 1) {
			os << "|| ";
			for (size_t i = 0; i < co.parallel.size(); i++) {
				os << co.sequential[i];
				if (i != co.parallel.size() - 1) {
					os << "|| ";
				}
			}
		}
		return os;
	}
}