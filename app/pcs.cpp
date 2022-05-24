#include "runner/runner.h"
#include "examples/merge/merge.h"
#include "pcs/system/system.h"
#include "examples/experimental/experimental.h"
#include "examples/experimental/experimental2.h"


int main() {
	#if (defined _DEBUG == 1)
		spdlog::set_level(spdlog::level::trace);
	#endif

	//MergeExample(4);

	RunnerOpts opts{ .incremental_topology = false, .generate_images = false, .only_highlighted_topology_image = true };
	//Run("pad", opts);
	pcs::System machine;
	machine = RunReturnMachine("adaptive", opts);
	Run("hinge",  opts);

	AddResourceAdaptive("resources", "adaptive", opts, machine);
}