#include "runner/runner.h"
#include "examples/merge/merge.h"
#include "examples/experimental/experimental.h"
#include "examples/experimental/experimental2.h"


int main() {
	#if (defined _DEBUG == 1)
		spdlog::set_level(spdlog::level::trace);
	#endif

	// MergeExample(4);
	// Experimental2();

	// Run("pad", 5, true, true);
	Run("hinge", 5, true, true);

}