#include "runner/runner.h"
#include "examples/merge/merge.h"
#include "examples/experimental/experimental.h"
#include "examples/experimental/experimental2.h"


int main() {
	#if (defined _DEBUG == 1)
		spdlog::set_level(spdlog::level::trace);
	#endif

	// MergeExample(6);

	RunnerOpts opts{ .incremental_topology = true, .generate_images = true, .only_highlighted_topology_image = true };
	Run("hinge",  opts);
}