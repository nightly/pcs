#include "pcs/controller/plan_transition.h"

#include <vector>
#include <string>

namespace pcs {

	/*
	 * @brief: A tuple-like way of representing the controller's planned transitions whilst evaluating CompositeOperations 
	 */
	PlanTransition::PlanTransition(const TopologyState* from, const std::vector<std::string>& label, const TopologyState* to) 
		: from(from), label(label), to(to) {}

	PlanTransition::PlanTransition(const TopologyState* from, std::vector<std::string>&& label, const TopologyState* to)
		: from(from), label(std::move(label)), to(to) {}

	PlanTransition::PlanTransition(const PlanTransition& other) 
		:from(other.from), label(other.label), to(other.to) {}

	PlanTransition& PlanTransition::operator=(const PlanTransition& other) {
		from = other.from;
		label = other.label;
		to = other.to;
		return *this;
	}
	
	PlanTransition::PlanTransition(PlanTransition&& other) noexcept 
		: label(std::move(other.label)) {
		from = other.from;
		other.from = nullptr;
		to = other.to;
		other.to = nullptr;
	}
	
	PlanTransition& PlanTransition::operator=(PlanTransition&& other) noexcept {
		from = other.from;
		other.from = nullptr;
		label = std::move(other.label);
		to = other.to;
		other.to = nullptr;
		return *this;
	}

}