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

	void ApplyTransition(const PlanTransition& plan_t, 
		                 nightly::LTS<std::pair<std::string, std::vector<std::string>>, std::vector<std::string>, 
		                 boost::hash<std::pair<std::string, std::vector<std::string>>>>& controller);

	void ApplyAllTransitions(const std::span<PlanTransition>& plan_transitions, 
		                    nightly::LTS<std::pair<std::string, std::vector<std::string>>, std::vector<std::string>, 
		                    boost::hash<std::pair<std::string, std::vector<std::string>>>>& controller);

	bool NopizeObservable(const std::vector<nightly::LTS<std::string, ParameterizedOp>>& ltss,
		                  const std::vector<std::string>& states_vec, size_t resource,
						  const std::string& op_str);

	bool NopizeSync(const std::vector<nightly::LTS<std::string, ParameterizedOp>>& ltss,
		            const std::vector<std::string>& states_vec, size_t in, size_t out,
				    const std::string& op_str);

}