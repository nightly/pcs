#include "pcs/controller/parts.h"

#include <vector>
#include <string>

namespace pcs {
	Parts::Parts(size_t num_resources) {
		vector_.resize(num_resources, std::vector<std::string>());
	}

	Parts::Parts(const Parts& other) 
		: vector_(other.vector_) {}
	
	Parts& Parts::operator=(const Parts& other) {
		vector_ = other.vector_;
		return *this;
	}

	Parts::Parts(Parts&& other) noexcept 
		: vector_(std::move(other.vector_)) {}

	Parts& Parts::operator=(Parts&& other) noexcept {
		vector_ = std::move(other.vector_);
		return *this;
	}

}


// @Todo 
/*

	bool Controller::TransferParts(size_t resource, std::vector<std::vector<std::string>>& parts) {
		auto& [op, input, output] = *seq_tuple_; // Our current SequentialOperation with op, input, output -- probably shouldn't be a class variable
		std::vector<size_t> removals;

		if (!input.empty() && !parts[resource].empty()) {
			for (size_t i = 0; i < parts[resource].size(); ++i) {
				// Does our current part in the resource coincide with any part in our input we're looking for?
				if (std::find(input.begin(), input.end(), parts[resource][i]) != input.end()) {
					removals.emplace_back(i);
				}
			}
		}

		if (removals.size() != input.size()) { // If the size of input parts doesn't match removals size, we haven't found everything, ret false
			// PCS_WARN("Recipe is not reliasable, input parts cannot be found at resource index {}. Missing parts = {}", resource, fmt::join(input, ","));
		}

		for (const auto& i : removals) { // Now we can remove everything the recipe has consumed
			// parts_[resource].erase(parts_[resource].begin() + i);
		}


		for (const auto& s : output) {
			// parts_[resource].emplace_back(s);
		}

		return true;
	}


*/