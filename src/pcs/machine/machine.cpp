#include "pcs/machine/machine.h"

#include "pcs/topology/topology.h"
#include "pcs/lts/parsers/string_string.h"

namespace pcs {

	Machine::Machine(std::vector<LTS<std::string, std::string>>&& resources, bool compute_topology) {
		resources_ = std::move(resources);
		if (compute_topology) {
			ComputeTopology();
		}
	}

	const std::vector<LTS<std::string, std::string>>& Machine::resources() const {
		return resources_;
	}

	const LTS<std::string, std::string>& Machine::topology() const {
		return topology_;
	}

	size_t Machine::NumOfResources() const {
		return resources_.size();
	}

	size_t Machine::NumOfTopologyStates() const {
		return topology_.NumOfStates();
	}

	Machine::Machine(const std::span<LTS<std::string, std::string>>& resources, bool compute_topology)  {
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
		LTS<std::string> lts;
		try {
			if (is_json) {
				ReadFromJsonFile(lts, filepath);
			} else {
				ReadFromFile(lts, filepath);
			}
		} catch (const std::ifstream::failure& e) {
			throw;
		}
		AddResource(std::move(lts));
	}

	/*
	 * @brief Adds a LTS<std::string> resource to the machine & handles the implications on the topology 
	 */
	void Machine::AddResource(const LTS<std::string, std::string>& resource) {
		if (topology_.NumOfStates() == 0) {
			resources_.emplace_back(resource);
		} else {
			// @Todo: Consider case where topology must be recomputed or modified/adapted
		}
	}

	/* 
	 * @brief Adds a resource with move semantics
	 */
	void Machine::AddResource(LTS<std::string, std::string>&& resource) {
		if (topology_.NumOfStates() == 0) {
			resources_.emplace_back(std::move(resource));
		} else {
			// @Todo: Consider case where topology must be recomputed or modified/adapted
		}
	}

}
