#include "pcs/system/system.h"

#include "pcs/topology/topology.h"
#include "pcs/lts/parsers/string_string.h"

namespace pcs {

	System::System(std::vector<LTS<std::string, std::string>>&& resources, bool compute_topology) {
		resources_ = std::move(resources);
		if (compute_topology) {
			ComputeTopology();
		}
	}

	const std::vector<LTS<std::string, std::string>>& System::resources() const {
		return resources_;
	}

	const LTS<std::vector<std::string>, std::pair<size_t, std::string>, boost::hash<std::vector<std::string>>>& System::topology() const {
		return topology_;
	}

	size_t System::NumOfResources() const {
		return resources_.size();
	}

	size_t System::NumOfTopologyStates() const {
		return topology_.NumOfStates();
	}

	System::System(const std::span<LTS<std::string, std::string>>& resources, bool compute_topology)  {
		resources_.assign(resources.begin(), resources.end());
		if (compute_topology) {
			ComputeTopology();
		}
	}

	void System::ComputeTopology() {
		topology_ = pcs::Combine(resources_);
	}

	/*
	 * @brief Loads a LTS file and adds it to the machine, and handles recomputing the topology
	 * @param filepath: relative path to the LTS file to parse and adds it
	 * @param is_json: specifies whether the LTS is in .txt or .json format
	 * @exception Propagates std::ifstream::failure
	 */
	void System::AddResource(const std::filesystem::path& filepath, bool is_json) {
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
	void System::AddResource(const LTS<std::string, std::string>& resource) {
		if (topology_.NumOfStates() == 0) {
			resources_.emplace_back(resource);
		} else {
			// @Todo: Consider case where topology must be recomputed or modified/adapted
		}
	}

	/* 
	 * @brief Adds a resource with move semantics
	 */
	void System::AddResource(LTS<std::string, std::string>&& resource) {
		if (topology_.NumOfStates() == 0) {
			resources_.emplace_back(std::move(resource));
		} else {
			// @Todo: Consider case where topology must be recomputed or modified/adapted
		}
	}

}
