#pragma once

#include <optional>
#include <span>
#include <string>
#include <vector>

#include <boost/container_hash/hash.hpp>

#include "pcs/environment/environment.h"
#include "pcs/product/recipe.h"
#include "pcs/lts/lts.h"
#include "pcs/operation/transfer.h"
#include "pcs/controller/plan_transition.h"
#include "pcs/controller/parts.h"

namespace pcs {

	/*
	 * @brief Whether the best controller solution should minimize the number of transitions or resources
	 */
	enum class MinimizeOpt {
		Transitions,
		Resources
	};

	class BestController {
	public:
		using TopologyTransition = std::pair<size_t, std::string>;
		using TopologyState = std::vector<std::string>;

		using ControllerTransition = std::vector<std::string>;
		using ControllerState = std::pair<std::string, std::vector<std::string>>;
		using ControllerType = LTS<std::pair<std::string, std::vector<std::string>>, std::vector<std::string>,
			boost::hash<std::pair<std::string, std::vector<std::string>>>>;
	private:
		ControllerType controller_;
		const Environment* machine_;
		const Recipe* recipe_;
		ITopology* topology_;

		size_t num_of_resources_;
	public:
		BestController(const Environment* machine, ITopology* topology, const Recipe* recipe);
		std::optional<ControllerType> Generate(MinimizeOpt opt);
	private:
	};

}