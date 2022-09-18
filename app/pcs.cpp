#include "runner/runner.h"
#include "examples/merge/merge.h"
#include "examples/experimental/experimental.h"

#include "pcs/common/log.h"

using namespace pcs;

int main() {

#if (defined _DEBUG == 1)
	spdlog::set_level(spdlog::level::debug);
#endif

	// MergeExample(2);

	RunnerOpts opts{ .solver = SolverOpt::BestCost, .incremental_topology = false, 
		             .generate_images = true, .only_highlighted_topology_image = false, .skeleton_topology_image = true,
		             .recipe_name = "recipe.json"};
	Run("best", opts);

	return 0;
}