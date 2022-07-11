#pragma once

#include <vector>
#include <string>

namespace pcs {

	struct PlanTransition {
	public:
		using TopologyState = std::vector<std::string>;

		const std::string* recipe_state;
		const TopologyState* from;
		std::vector<std::string> label;
		const TopologyState* to;
	public:

		PlanTransition() = delete;
		PlanTransition(const std::string& recipe_state, const TopologyState* from, const std::vector<std::string>& label, const TopologyState* to);
		PlanTransition(const std::string& recipe_state, const TopologyState* from, std::vector<std::string>&& label, const TopologyState* to);

		PlanTransition(const PlanTransition& other);
		PlanTransition& operator=(const PlanTransition& other);
		PlanTransition(PlanTransition&& other) noexcept;
		PlanTransition& operator=(PlanTransition&& other) noexcept;
	};

}