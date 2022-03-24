#pragma once

#include <vector>
#include <span>

#include "pcs/lts/lts.h"

namespace pcs {

	using LabelledTransitionSystem = pcs::lts::LabelledTransitionSystem;
	using State = pcs::lts::State;

	class Machine {
	private:
		std::vector<LabelledTransitionSystem> resources_;
		LabelledTransitionSystem topology_;
	public:
		Machine();
		Machine(const std::span<LabelledTransitionSystem>& resources);
		Machine(std::vector<LabelledTransitionSystem>&& resources);
		void ComputeTopology();
		void ComputeTology(std::initializer_list<size_t> resources);
		void RemoveResource();
		void AddResource();
	};
}