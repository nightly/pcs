#pragma once

#include <vector>
#include <span>
#include <filesystem>

#include <boost/container_hash/hash.hpp>

#include "pcs/lts/lts.h"
#include "pcs/product/recipe.h"

namespace pcs {

	class Machine {
	private:
		std::vector<LTS<std::string, std::string>> resources_;
		LTS<std::vector<std::string>, std::pair<size_t, std::string>, boost::hash<std::vector<std::string>>> topology_;
	public:
		Machine() = default;
		Machine(const std::span<LTS<std::string, std::string>>& resources, bool compute_topology);
		Machine(std::vector<LTS<std::string, std::string>>&& resources, bool compute_topology);

		const std::vector<LTS<std::string, std::string>>& resources() const;
		const LTS<std::vector<std::string>, std::pair<size_t, std::string>, boost::hash<std::vector<std::string>>>& topology() const;

		size_t NumOfResources() const;
		size_t NumOfTopologyStates() const;

		void ComputeTopology();
		void ComputeTopology(std::initializer_list<size_t> resources);
		void ComputeTopology(const Recipe& recipe);
		void AddResource(const std::filesystem::path& filepath, bool is_json);
		void AddResource(const LTS<std::string>& resource);
		void AddResource(LTS<std::string>&& resource);
		void RemoveResource();
	};
}