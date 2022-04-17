#include "pcs/topology/resource.h"

#include <string>
#include <vector>

#include "pcs/common/strings.h"

namespace pcs {

	/* 
	 * @brief Identifies the current resource being acted upon based on oldand new states
	 */
	size_t ActingOnResource(const std::string& old_state, const std::string& new_state) {
		// @Cleanup: knowing which resource a topology transition is acting upon should probably be embedded within the transition,	
		// i.e. std::pair<std::size_t, std::string|pcs::IOperation>
		std::vector<std::string> old_ve = StringToVector(old_state);
		std::vector<std::string> new_ve = StringToVector(new_state);
		for (size_t i = 0; i < new_ve.size(); ++i) {
			if (old_ve[i] != new_ve[i])
				return i;
		}
	}

}