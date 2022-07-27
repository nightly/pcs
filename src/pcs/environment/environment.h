#pragma once

#include <vector>
#include <span>
#include <filesystem>
#include <memory>

#include <boost/container_hash/hash.hpp>

#include "lts/lts.h"
#include "pcs/product/recipe.h"
#include "pcs/topology/topology.h"
#include "pcs/topology/complete.h"
#include "pcs/topology/incremental.h"

namespace pcs {

	class Environment {
	private:
		std::vector<nightly::LTS<std::string, ParameterizedOp>> resources_;
		std::unique_ptr<ITopology> topology_;
	public:
		Environment() = default;
		Environment(const std::span<nightly::LTS<std::string, ParameterizedOp>>& resources, bool compute_topology);
		Environment(std::vector<nightly::LTS<std::string, ParameterizedOp>>&& resources, bool compute_topology);

		const std::vector<nightly::LTS<std::string, ParameterizedOp>>& resources() const;
		const ITopology* topology() const;
	    ITopology* topology();

		size_t NumOfResources() const;
		size_t NumOfTopologyStates() const;

		void Complete();
		void Incremental();

		/* @Todo */
		void ComputeTopology(std::initializer_list<size_t> resources);
		void ComputeTopology(const Recipe& recipe);
		void AddResource(const std::filesystem::path& filepath, bool is_json);
		void AddResource(const nightly::LTS<std::string, pcs::ParameterizedOp>& resource);
		void AddResource(nightly::LTS<std::string, pcs::ParameterizedOp>&& resource);
		void RemoveResource();
	};
}