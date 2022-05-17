#pragma once

#include <vector>
#include <span>
#include <filesystem>
#include <memory>

#include <boost/container_hash/hash.hpp>

#include "pcs/lts/lts.h"
#include "pcs/product/recipe.h"
#include "pcs/topology/topology.h"
#include "pcs/topology/complete.h"
#include "pcs/topology/incremental.h"

namespace pcs {

	class System {
	private:
		std::vector<LTS<std::string, std::string>> resources_;
		std::unique_ptr<ITopology> topology_;
	public:
		System() = default;
		System(const std::span<LTS<std::string, std::string>>& resources, bool compute_topology);
		System(std::vector<LTS<std::string, std::string>>&& resources, bool compute_topology);

		const std::vector<LTS<std::string, std::string>>& resources() const;
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
		void AddResource(const LTS<std::string>& resource);
		void AddResource(LTS<std::string>&& resource);
		void RemoveResource();
	};
}