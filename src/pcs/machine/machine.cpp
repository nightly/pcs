#include "pcs/machine/machine.h"

#include "pcs/controller/topology.h"
#include "pcs/lts/parsers.h"

namespace pcs {

	Machine::Machine(std::vector<LabelledTransitionSystem<>>&& resources, bool compute_topology) {
		resources_ = std::move(resources);
		if (compute_topology) {
			ComputeTopology();
		}
	}

	Machine::Machine(const std::span<LabelledTransitionSystem<>>& resources, bool compute_topology)  {
		resources_.assign(resources.begin(), resources.end());
		if (compute_topology) {
			ComputeTopology();
		}
	}

	void Machine::ComputeTopology() {
		topology_ = pcs::Combine(resources_);
	}

	/*
	 * @brief Loads a LTS file and adds it to the machine, and handles recomputing the topology
	 * @param filepath: relative path to the LTS file to parse and adds it
	 * @param is_json: specifies whether the LTS is in .txt or .json format
	 * @exception Propagates std::ifstream::failure
	 */
	void Machine::AddResource(const std::filesystem::path& filepath, bool is_json) {
		LabelledTransitionSystem<std::string> lts;
		try {
			if (is_json) {
				ReadFromFile(lts, filepath);
			}
			else {
				ReadFromJsonFile(lts, filepath);
			}
		} catch (const std::ifstream::failure& e) {
			throw;
		}
		AddResource(std::move(lts));
	}

	/*
	 * @brief Adds a LTS<std::string> resource to the machine & handles the implications on the topology 
	 */
	void Machine::AddResource(const LabelledTransitionSystem<std::string>& resource) {
		if (topology_.NumberOfStates() == 0) {
			resources_.emplace_back(resource);
		} else {
			// @Todo: Consider case where topology must be recomputed or modified/adapted
		}
	}

	/* 
	 * @brief Adds a resource with move semantics
	 */
	void Machine::AddResource(LabelledTransitionSystem<std::string>&& resource) {
		if (topology_.NumberOfStates() == 0) {
			resources_.emplace_back(std::move(resource));
		} else {
			// @Todo: Consider case where topology must be recomputed or modified/adapted
		}
	}

}
