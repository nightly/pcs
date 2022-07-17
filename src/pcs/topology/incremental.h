#pragma once
#include "pcs/topology/topology.h"

#include <vector>
#include <string>

#include <boost/container_hash/hash.hpp>

#include "lts/lts.h"
#include "pcs/topology/core.h"

namespace pcs {

	class IncrementalTopology : public ITopology {
	private:
		nightly::LTS<std::vector<std::string>, std::pair<size_t, std::string>, boost::hash<std::vector<std::string>>> topology_;
		std::unordered_set<std::vector<std::string>, boost::hash<std::vector<std::string>>> visited_;
		const std::vector<nightly::LTS<std::string, std::string>>& ltss_;
	public:
		IncrementalTopology(const std::vector<nightly::LTS<std::string, std::string>>& ltss);
		const nightly::LTS<std::vector<std::string>, std::pair<size_t, std::string>, boost::hash<std::vector<std::string>>>& lts() const override;
		operator const nightly::LTS<std::vector<std::string>, std::pair<size_t, std::string>, boost::hash<std::vector<std::string>>>& () const override;
		const std::vector<std::string>& initial_state() const override;
		const nightly::State<std::vector<std::string>, std::pair<size_t, std::string>>& at(const std::vector<std::string>& key) override;
	private:
		void ExpandState(const std::vector<std::string>& key);

	};


}