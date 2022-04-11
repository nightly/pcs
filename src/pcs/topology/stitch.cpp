#include "pcs/topology/stitch.h"

namespace pcs {


	/*
	 * @brief RemoveResources facilitates the removal of resources from an existing topology without recomputing entirely
	 * @param topology: the pre-existing/current LTS topology
	 * @param resources: the resource to be removed relative to which position it is in the topology (_,x,_), x = 1,
	 *		  which coincides with the order of ltss' specified in the original span container of combine()
	 */
	void RemoveResources(LabelledTransitionSystem<>& topology, std::initializer_list<size_t> resources) {

	}

	/*
	 * @brief AddResources allows the addition of new resources to be added to an existing topology without recomputing entirely
	 * @param topology: the pre-existing/current LTS topology
	 * @param ltss: the new LTSS to add to be merged with the existing topology
	 */
	void AddResources(LabelledTransitionSystem<>& topology, const std::span<LabelledTransitionSystem<>>& ltss) {

	}

}