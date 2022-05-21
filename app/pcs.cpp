#include "examples/merge/merge.h"
#include "examples/pad/pad.h"
#include "examples/experimental/experimental.h"
#include "examples/experimental/experimental2.h"
#include "examples/hinge/hinge.h"


int main() {
	#if (defined _DEBUG == 1)
		spdlog::set_level(spdlog::level::trace);
	#endif

	// MergeExample(4);
	// Experimental2();

	// PadExample(false, false); 
	HingeExample(true, true);
}