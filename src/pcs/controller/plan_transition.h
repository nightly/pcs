#pragma once

#include <vector>
#include <string>

namespace pcs {

	struct PlanTransition {
		using TopologyState = std::vector<std::string>;

		const TopologyState* from;
		std::vector<std::string> label;
		const TopologyState* to;

		PlanTransition(const TopologyState* from, const std::vector<std::string>& label, const TopologyState* to);
		PlanTransition(const TopologyState* from, std::vector<std::string>&& label, const TopologyState* to);

		PlanTransition(const PlanTransition& other);
		PlanTransition& operator=(const PlanTransition& other);
		PlanTransition(PlanTransition&& other) noexcept;
		PlanTransition& operator=(PlanTransition&& other) noexcept;
	};

}