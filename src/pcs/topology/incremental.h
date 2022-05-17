#pragma once

#include "pcs/topology/topology.h"

#include <vector>
#include <string>

#include <boost/container_hash/hash.hpp>

#include "pcs/lts/lts.h"
#include "pcs/topology/core.h"

namespace pcs {

	class IncrementalTopology : public ITopology {
	private:
		LTS<std::vector<std::string>, std::pair<size_t, std::string>, boost::hash<std::vector<std::string>>> topology_;
		
		const std::span<LTS<std::string, std::string>>& ltss_;
		std::unordered_set<std::vector<std::string>, boost::hash<std::vector<std::string>>> visited_;
	public:
		IncrementalTopology(const std::span<LTS<std::string, std::string>>& ltss);
		const LTS<std::vector<std::string>, std::pair<size_t, std::string>, boost::hash<std::vector<std::string>>>& lts() const override;
		const State<std::vector<std::string>, std::pair<size_t, std::string>>& at(const std::vector<std::string>& key) override;
		const std::vector<std::string>& initial_state() const override;
	private:
		void ExpandState(const std::vector<std::string>& key);

	};


}