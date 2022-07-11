#include "runner/runner.h"
#include "examples/merge/merge.h"
#include "examples/experimental/experimental.h"

using namespace pcs;

int main() {

#if (defined _DEBUG == 1)
	spdlog::set_level(spdlog::level::debug);
#endif

	// MergeExample(2);

	RunnerOpts opts{ .incremental_topology = true, .generate_images = true, .only_highlighted_topology_image = false };
	Run("pad", opts);

	return 0;
}