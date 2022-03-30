#include "pcs/machine/machine.h"

#include "pcs/controller/topology.h"

namespace pcs {

	Machine::Machine(std::vector<LabelledTransitionSystem<>>&& resources) {
		resources_ = std::move(resources);
	}

	Machine::Machine(const std::span<LabelledTransitionSystem<>>& resources)  {
		resources_.assign(resources.begin(), resources.end());
	}

	void Machine::ComputeTopology() {
		topology_ = pcs::Combine(resources_);
	}

}
