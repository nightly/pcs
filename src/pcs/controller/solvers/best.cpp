#include "pcs/controller/solvers/best.h"

#include <optional>
#include <vector>
#include <span>
#include <string>
#include <algorithm>
#include <unordered_map>
#include <unordered_set>
#include <limits>
#include <queue>

#include <spdlog/fmt/bundled/color.h>
#include <spdlog/fmt/ranges.h>

#include "pcs/lts/lts.h"
#include "pcs/controller/core.h"
#include "pcs/common/log.h"
#include "pcs/common/strings.h"
#include "pcs/operation/parsers/label.h"

namespace pcs {

	using TopologyTransition = std::pair<size_t, std::string>;
	using TopologyState = std::vector<std::string>;

	using ControllerTransition = std::vector<std::string>;
	using ControllerState = std::pair<std::string, std::vector<std::string>>;
	using ControllerType = LTS<std::pair<std::string, std::vector<std::string>>, std::vector<std::string>,
		boost::hash<std::pair<std::string, std::vector<std::string>>>>;

	BestController::BestController(const Environment* machine, ITopology* topology, const Recipe* recipe) 
		: machine_(machine), recipe_(recipe), topology_(topology), num_of_resources_(machine_->NumOfResources()) {}

	struct Stage {
		const TopologyState& start_state;
		Parts parts;
		const std::string& recipe_state;
	public:
		Stage() = delete;
		Stage(const TopologyState& start_state, const Parts& parts, const std::string& recipe_state)
			: start_state(start_state), parts(parts), recipe_state(recipe_state) {}
	};

	struct Candidate {
		ControllerType controller;
		size_t num; // number of either resources or synchronization transitions used
		std::queue<Stage> descendants; // what needs to be processed. A controller is complete when there are no more descendants to process.
	};

	// Min order comparator for Candidates
	struct CandidateComparator {
	public:
		CandidateComparator() {}

		bool operator () (const Candidate& a, const Candidate& b) {
			return a.num > b.num;
		}
	};

	std::optional<ControllerType> BestController::Generate(MinimizeOpt opt) {
		size_t best = std::numeric_limits<size_t>::max();
		const Candidate* best_candidate;
		std::priority_queue<Candidate, std::vector<Candidate>, CandidateComparator> pq;
		std::unordered_set<size_t> used_resources;

		while (!pq.empty() && best >= pq.top().num) {
			 Candidate cand = pq.top();
			 pq.pop();

			 


			 if (cand.num < best) {
				 best = cand.num;
				 best_candidate = &cand;
			 }
		}

		return { best_candidate->controller };
	}

}