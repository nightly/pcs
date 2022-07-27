#include "pcs/environment/environment.h"

#include "pcs/topology/complete.h"
#include "pcs/topology/incremental.h"
#include "lts/parsers/parsers.h"
#include "pcs/common/log.h"

namespace pcs {

	Environment::Environment(std::vector<nightly::LTS<std::string, ParameterizedOp>>&& resources, bool compute_complete) {
		resources_ = std::move(resources);
		if (compute_complete) {
			Complete();
		}
	}

	Environment::Environment(const std::span<nightly::LTS<std::string, ParameterizedOp>>& resources, bool compute_complete) {
		resources_.assign(resources.begin(), resources.end());
		if (compute_complete) {
			Complete();
		}
	}

	const std::vector<nightly::LTS<std::string, ParameterizedOp>>& Environment::resources() const {
		return resources_;
	}

	const ITopology* Environment::topology() const {
		return topology_.get();
	}

	ITopology* Environment::topology() {
		return topology_.get();
	}

	size_t Environment::NumOfResources() const {
		return resources_.size();
	}

	size_t Environment::NumOfTopologyStates() const {
		return topology_->lts().NumOfStates();
	}

	void Environment::Complete() {
		topology_ = std::make_unique<CompleteTopology>(resources_);
	}

	void Environment::Incremental() {
		topology_ = std::make_unique<IncrementalTopology>(resources_);
	}

	/*
	 * @brief Loads a LTS file and adds it to the machine, and handles recomputing the topology
	 * @param filepath: relative path to the LTS file to parse and adds it
	 * @param is_json: specifies whether the LTS is in .txt or .json format
	 * @exception Propagates std::ifstream::failure
	 */
	void Environment::AddResource(const std::filesystem::path& filepath, bool is_json) {
		nightly::LTS<std::string, pcs::ParameterizedOp> lts;
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
	void Environment::AddResource(const nightly::LTS<std::string, pcs::ParameterizedOp>& resource) {
		resources_.emplace_back(resource);

		//if (topology_.NumOfStates() == 0) {
		//	resources_.emplace_back(resource);
		//} else {
		//	// @Todo: Consider case where topology must be recomputed or modified/adapted
		//}
	}

	/* 
	 * @brief Adds a resource with move semantics
	 */
	void Environment::AddResource(nightly::LTS<std::string, pcs::ParameterizedOp>&& resource) {
		resources_.emplace_back(std::move(resource));

		//if (topology_.NumOfStates() == 0) {
		//	resources_.emplace_back(std::move(resource));
		//} else {
		//	// @Todo: Consider case where topology must be recomputed or modified/adapted
		//}
	}

}
