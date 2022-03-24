#pragma once

#include <string>
#include <span>
#include <vector>

#include "pcs/lts/state.h"
#include "pcs/lts/lts.h"

namespace pcs::topology {
	using LabelledTransitionSystem = pcs::lts::LabelledTransitionSystem<std::string>;
	using State = pcs::lts::State<std::string>;

	LabelledTransitionSystem Combine(const std::span<LabelledTransitionSystem>& ltss);
	void CombineRecursive(const std::span<LabelledTransitionSystem>& ltss, std::vector<std::string>& states_vec,
		std::unordered_map<std::string, bool>& visited, LabelledTransitionSystem& combined_lts);
	std::string StateVectorToString(const std::span<std::string>& vec);

	void RemoveResources(LabelledTransitionSystem& topology, std::initializer_list<size_t> resources);
	void AddResources(LabelledTransitionSystem& topology, const std::span<LabelledTransitionSystem>& ltss);
}