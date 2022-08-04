#include "pcs/controller/core.h"

#include <span>
#include <vector>

#include <spdlog/fmt/bundled/color.h>
#include <spdlog/fmt/ranges.h>

#include "lts/lts.h"
#include "pcs/controller/plan_transition.h"
#include "pcs/common/log.h"
#include "pcs/operation/task_expression.h"
#include "pcs/operation/composite.h"

namespace pcs {

	using ControllerType = nightly::LTS<std::pair<std::string, std::vector<std::string>>, std::vector<std::string>,
		                                 boost::hash<std::pair<std::string, std::vector<std::string>>>>;

	void ApplyTransition(const PlanTransition& plan_t, ControllerType& controller) {
		controller.AddTransition({ *plan_t.recipe_state, *plan_t.from }, plan_t.label, { *plan_t.recipe_state, *plan_t.to });
		PCS_INFO(fmt::format(fmt::fg(fmt::color::royal_blue) | fmt::emphasis::bold,
			"Adding controller transition from {} with label ({}) to {}", fmt::join(*plan_t.from, ","),
			fmt::join(plan_t.label, ","), fmt::join(*plan_t.to, ",")));
	}

	void ApplyAllTransitions(const std::span<PlanTransition>& plan_transitions, ControllerType& controller) {
		for (const auto& t : plan_transitions) {
			ApplyTransition(t, controller);
		}
	}

	/*
	 * Ensures the rest of the states are in a Nop state when applying an observable (operation/nop) transition.
	 */
	bool NopizeObservable(const std::vector<nightly::LTS<std::string, ParameterizedOp>>& ltss,
		const std::vector<std::string>& states_vec, size_t resource, const std::string& op_str) {
		for (size_t i = 0; i < states_vec.size(); ++i) {
			if (i == resource) {
				continue;
			}
			bool found_nop = false;
			for (const auto& t : ltss[i].at(states_vec[i]).transitions()) {
				if (t.label().operation() == "nop") {
					found_nop = true;
					break;
				}
			}
			if (!found_nop) {
				PCS_DEBUG(fmt::format(fmt::fg(fmt::color::slate_blue), "[Nop] Unable to find nop transition at resource {} when processing {}. States vec = {}", 
					i, op_str, fmt::join(states_vec, ",")));
				return false;
			}
		}
		return true;
	}

	/*
	 * Ensures the rest of the states are in a Nop state when applying a transfer transition.
	 */
	bool NopizeSync(const std::vector<nightly::LTS<std::string, ParameterizedOp>>& ltss, 
		            const std::vector<std::string>& states_vec, size_t in, size_t out,
	                const std::string& op_str) {
		for (size_t i = 0; i < states_vec.size(); ++i) {
			if (i == in || i == out) {
				continue;
			}
			bool found_nop = false;
			for (const auto& t : ltss[i].at(states_vec[i]).transitions()) {
				if (t.label().operation() == "nop") {
					found_nop = true;
					break;
				}
			}
			if (!found_nop) {
				PCS_DEBUG(fmt::format(fmt::fg(fmt::color::slate_blue), "[Nop] Unable to find nop transition at resource {} when processing sync({},{}) for {}. States vec = {}", 
					i, in, out, op_str, fmt::join(states_vec, ",")));
				return false;
			}
		}
		return true;
	}

}