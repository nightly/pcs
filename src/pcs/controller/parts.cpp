#include "pcs/controller/parts.h"

#include <vector>
#include <string>
#include <algorithm>

#include "pcs/common/log.h"

namespace pcs {

	// ==========================
	// Constructors & destructor
	// ==========================

	Parts::Parts(size_t num_resources) {
		parts_.resize(num_resources, std::vector<std::string>());
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

	const std::vector<std::string>& Parts::AtResource(size_t resource) const {
		return parts_[resource];
	}

	/**
	 * @brief Introduces parts into the accompanying resource following a successful operation completion.  
	 * @param transition: a Topology Transition in the form of std::pair<size_t, std::string>. Excludes the "to" part.
	 * @param output: the output parts to introduce, e.g. spring1, retainer3.
	 */
	void Parts::Add(const TopologyTransition& transition, const std::vector<std::string>& output) {
		PCS_INFO(fmt::format(fmt::fg(fmt::color::coral),  "[Parts] Adding parts [{}] to resource {}",
			fmt::join(output, ","), transition.first));
		
		for (const auto& part : output) {
			parts_[transition.first].emplace_back(part);
		}
	}

	/**
	 * @brief Returns whether or not parts can be successfully synchronized.
	 * @param TopologyTransition in, out: follows the form of std::pair<size_t, std::string>
	 * @param input: the input parts that must be synchronized over, corresponding with the current operation.
	 *	      Note that input parts must follow a 1-1 string mapping of the parts in the resources to the recipe.
	 * @returns Whether or not the parts could be synchronized. This will fail when parts to be synchronized don't exist.
	 */
	bool Parts::Synchronize(const TopologyTransition& in, const TopologyTransition& out, const std::vector<std::string>& input) {
		size_t count = 0;
		size_t input_size = input.size();
		size_t from_resource = out.first;
		size_t sync_resource = in.first;

		auto end = std::remove_if(parts_[from_resource].begin(), parts_[from_resource].end(),
			[&](const auto& i) {
				if (std::find(input.begin(), input.end(), i) != input.end()) {
					count++;
					parts_[sync_resource].emplace_back(i);
					return true;
				}
				else {
					return false;
				}
			});

		parts_[from_resource].erase(end, parts_[from_resource].end());

		if (count != input_size) {
			PCS_WARN(fmt::format(fmt::fg(fmt::color::light_yellow) | fmt::emphasis::underline, 
				"[Parts Sync] Not all parts were found at resource {} from set: {}", from_resource, fmt::join(input, ",")));
			return false;
		}

		return true;
	}

	/*
	 * @brief Returns whether or not parts can be successfully allocated to the resource
	 * @param transition: Topology Transition in the form of std::pair<size_t, std::pair>
	 * @param input: the input parts for that resource. Note: the resource will consume these parts.
	 * @returns True if parts at present at resource and can be depleted, false otherwise.
	 */
	bool Parts::Allocate(const TopologyTransition& transition, const std::vector<std::string>& input) {
		size_t count = 0;
		size_t input_size = input.size();
		size_t resource = transition.first;

		auto end = std::remove_if(parts_[resource].begin(), parts_[resource].end(),
			[&](const auto& i) {
				if (std::find(input.begin(), input.end(), i) != input.end()) {
					PCS_INFO(fmt::format(fmt::fg(fmt::color::coral), "[Parts] Removing part {} from resource {}", resource, i));
					count++;
					return true;
				} else {
					return false;
				}
			});

		parts_[resource].erase(end, parts_[resource].end());

		if (count != input_size) {
			PCS_WARN(fmt::format(fmt::fg(fmt::color::light_yellow) | fmt::emphasis::underline, "[Parts] Not all parts were found at resource {} from set: {}", resource,
				fmt::join(input, ",")));
			return false;
		}

		return true;
	}


}

/*
 * @Cleanup: input parts in operations should probably use unordered_set instead of vector 
 */