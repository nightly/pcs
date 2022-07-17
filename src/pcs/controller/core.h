#pragma once

#include <span>
#include <vector>
#include <string>

#include <boost/container_hash/hash.hpp>

#include "pcs/controller/plan_transition.h"
#include "lts/lts.h"
#include "pcs/operation/task_expression.h"
#include "pcs/operation/composite.h"


namespace pcs {

	const TaskExpression& CurrentTask(const CompositeOperation& co, size_t seq_id);
	bool IsLastOp(const CompositeOperation& co, size_t seq_id);

	void ApplyTransition(const PlanTransition& plan_t, 
		                 nightly::LTS<std::pair<std::string, std::vector<std::string>>, std::vector<std::string>, 
		                 boost::hash<std::pair<std::string, std::vector<std::string>>>>& controller);

	void ApplyAllTransitions(const std::span<PlanTransition>& plan_transitions, 
		                     nightly::LTS<std::pair<std::string, std::vector<std::string>>, std::vector<std::string>, 
		                         boost::hash<std::pair<std::string, std::vector<std::string>>>>& controller);

	void SynchronizeOperation();
	void ObservableOperation();
	bool Nopize();

}