#pragma once

#include <vector>
#include <span>

#include "pcs/lts/lts.h"
#include "pcs/product/recipe.h"

namespace pcs {

	class Machine {
	private:
		std::vector<LabelledTransitionSystem<>> resources_;
		LabelledTransitionSystem<> topology_;
	public:
		Machine() = default;
		Machine(const std::span<LabelledTransitionSystem<>>& resources);
		Machine(std::vector<LabelledTransitionSystem<>>&& resources);
		void ComputeTopology();
		void ComputeTopology(std::initializer_list<size_t> resources);
		void ComputeTopology(const Recipe& recipe);
		void RemoveResource();
		void AddResource();
	};
}