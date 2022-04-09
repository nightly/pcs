#pragma once

#include <vector>
#include <span>
#include <filesystem>

#include "pcs/lts/lts.h"
#include "pcs/product/recipe.h"

namespace pcs {

	class Machine {
	private:
		std::vector<LabelledTransitionSystem<std::string, std::string>> resources_;
		LabelledTransitionSystem<std::string, std::string> topology_;
	public:
		Machine() = default;
		Machine(const std::span<LabelledTransitionSystem<std::string, std::string>>& resources, bool compute_topology);
		Machine(std::vector<LabelledTransitionSystem<std::string, std::string>>&& resources, bool compute_topology);

		const std::vector<LabelledTransitionSystem<std::string, std::string>>& resources() const;
		const LabelledTransitionSystem<std::string, std::string>& topology() const;

		size_t NumOfResources() const;
		size_t NumOfTopologyStates() const;

		void ComputeTopology();
		void ComputeTopology(std::initializer_list<size_t> resources);
		void ComputeTopology(const Recipe& recipe);
		void AddResource(const std::filesystem::path& filepath, bool is_json);
		void AddResource(const LabelledTransitionSystem<std::string>& resource);
		void AddResource(LabelledTransitionSystem<std::string>&& resource);
		void RemoveResource();
	};
}