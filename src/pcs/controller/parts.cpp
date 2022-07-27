#include "pcs/controller/parts.h"

#include <vector>
#include <string>
#include <algorithm>

#include "spdlog/fmt/ostr.h"

#include "pcs/common/log.h"
#include "pcs/operation/parameterized_op.h"

namespace pcs {

	using TopologyTransition = std::pair<size_t, ParameterizedOp>;

	// ==========================
	// Constructors & destructor
	// ==========================

	Parts::Parts(size_t num_resources) {
		parts_.resize(num_resources, std::unordered_set<std::string>());
	}

	Parts::Parts(const Parts& other) 
		: parts_(other.parts_) {}
	
	Parts& Parts::operator=(const Parts& other) {
		parts_ = other.parts_;
		return *this;
	}

	Parts::Parts(Parts&& other) noexcept 
		: parts_(std::move(other.parts_)) {}

	Parts& Parts::operator=(Parts&& other) noexcept {
		parts_ = std::move(other.parts_);
		return *this;
	}

	// ==========================
	// Member functions
	// ==========================

	const std::unordered_set<std::string>& Parts::AtResource(size_t resource) const {
		return parts_[resource];
	}

	// Add all operation output parts to the transition resource where the operation took place
	void Parts::Add(const TopologyTransition& transition, const std::vector<std::string>& output) {
		PCS_INFO(fmt::format(fmt::fg(fmt::color::coral),  "[Parts] Adding parts [{}] to resource {}",
			fmt::join(output, ","), transition.first));
		
		for (const auto& part : output) {
			parts_[transition.first].emplace(part);
		}
	}

	// Move all parts from the operation input possible to the [in] transition
	bool Parts::Synchronize(size_t in, size_t out, const std::unordered_set<std::string>& input) {
		if (parts_[out].empty()) {
			return false; // No parts present at the resource
		}
		
		for (const auto& p : input) {
			if (parts_[out].contains(p)) {
				parts_[out].erase(p);
				parts_[in].emplace(p);
			}
		}
		return true;
	}

	// Consume parts at the resource from the operation input set at the specified resource
	bool Parts::Allocate(const TopologyTransition& transition, const std::unordered_set<std::string>& input) {
		for (const auto& p : input) {
			if (parts_[transition.first].contains(p)) {
				parts_[transition.first].erase(p);
			} else {
				PCS_TRACE(fmt::format(fmt::fg(fmt::color::coral), "[Parts] Unable to consume part {}, expected at resource {} for operation {}",
					p, transition.first, transition.second));
				return false;
			}
		}

		return true;
	}

	bool Parts::operator==(const Parts& other) const {
		return parts_ == other.parts_;
	}

	std::ostream& operator<<(std::ostream& os, const Parts& parts) {
		for (size_t i = 0; i < parts.parts_.size(); ++i) {
			if (parts.parts_[i].empty()) {
				continue;
			}
			os << "Parts at Resource " << (i) << ':';
			os << "  ";
			for (const auto& p : parts.parts_[i]) {
				os << p << " ";
			}
			os << "\n";
		}
		return os;
	}

}