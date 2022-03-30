#pragma once

#include <vector>
#include <span>
#include <filesystem>

#include "pcs/lts/lts.h"
#include "pcs/product/recipe.h"

namespace pcs {

	class Machine {
	private:
		std::vector<LabelledTransitionSystem<>> resources_;
		LabelledTransitionSystem<> topology_;
	public:
		Machine() = default;
		Machine(const std::span<LabelledTransitionSystem<>>& resources, bool compute_topology);
		Machine(std::vector<LabelledTransitionSystem<>>&& resources, bool compute_topology);

		void ComputeTopology();
		void ComputeTopology(std::initializer_list<size_t> resources);
		void ComputeTopology(const Recipe& recipe);
		void AddResource(const std::filesystem::path& filepath, bool is_json);
		void AddResource(const LabelledTransitionSystem<std::string>& resource);
		void AddResource(LabelledTransitionSystem<std::string>&& resource);
		void RemoveResource();
	};
}